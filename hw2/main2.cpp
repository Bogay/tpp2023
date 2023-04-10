#include <bits/stdc++.h>
#include "spider_attack/environment.hpp"
#include "spider_attack/entity.hpp"
#include "spider_attack/base.hpp"

using namespace std;

enum
{
    monster,
    myHero,
    opHero
};
enum
{
    WAIT,
    MOVE,
    SPELL
};
enum
{
    WIND,
    SHIELD,
    CONTROL
};

struct Entity
{
    int id;   // Unique identifier
    int type; // 0=monster, 1=your hero, 2=opponent hero
    int x;    // Position of this entity
    int y;
    int shield_life;   // Count down until shield spell fades
    int is_controlled; // Equals 1 when this entity is under a control spell
    int health;        // Remaining health of this monster
    int vx;            // Trajectory of this monster
    int vy;
    int near_base;  // 0=monster with no target yet, 1=monster targeting a base
    int threat_for; // Given this monster's trajectory, is it a threat to 1=your base, 2=your opponent's base, 0=neither

    double distToMyBase;
};

int myBaseX; // The corner of the map representing your base
int myBaseY;
int opBaseX;
int opBaseY;
int myHealth, myMana, opHealth, opMana;
int roundCnt;
int sentCnt;

Entity get_entity()
{
    int id;   // Unique identifier
    int type; // 0=monster, 1=your hero, 2=opponent hero
    int x;    // Position of this entity
    int y;
    int shield_life;   // Count down until shield spell fades
    int is_controlled; // Equals 1 when this entity is under a control spell
    int health;        // Remaining health of this monster
    int vx;            // Trajectory of this monster
    int vy;
    int near_base;  // 0=monster with no target yet, 1=monster targeting a base
    int threat_for; // Given this monster's trajectory, is it a threat to 1=your base, 2=your opponent's base, 0=neither
    cin >> id >> type >> x >> y >> shield_life >> is_controlled >> health >> vx >> vy >> near_base >> threat_for;
    cin.ignore();

    return (Entity){id, type, x, y, shield_life, is_controlled, health, vx, vy, near_base, threat_for, hypot(abs(x - myBaseX), abs(y - myBaseY))};
} // get_entity

double distance(spider_attack::Entity a, spider_attack::Entity b)
{
    auto [ax, ay] = a.get_position();
    auto [bx, by] = b.get_position();
    return hypot(abs(ax - bx), abs(ay - by));
} // distance

double distance(spider_attack::Entity a, pair<int, int> b)
{
    auto [ax, ay] = a.get_position();
    return hypot(abs(ax - b.first), abs(ay - b.second));
} // distance

void set_defender(spider_attack::Environment &env, int defenderID)
{
    auto defender = env.get_my_heroes()[defenderID];
    int standX, standY, action = MOVE, toX, toY, spellType = WIND, entityID;

    if (env.get_my_base().get_position().first != 0)
        standX = 16830, standY = 8200;
    else
        standX = 800, standY = 800;

    toX = standX, toY = standY;

    // guarding of opponents
    int myMana = env.get_my_base().get_mana();
    for (int i = 0; myMana >= 10 && i < env.get_entities().size(); i++)
    {
        if (distance(defender, env.get_entities()[i]) <= 1200)
        {
            action = SPELL;
            spellType = WIND;
            break;
        } // if
        else if (myMana >= 100 && distance(defender, env.get_entities()[i]) <= 1500)
        {
            action = SPELL;
            spellType = CONTROL;
            entityID = env.get_entities()[i].get_id();
            toX = opBaseX, toY = opBaseY;
            break;
        } // else if
    }     // for i

    // guarding of monsters
    if (env.get_monsters().empty())
        action = MOVE;
    else if (env.get_monsters()[0].get_shield_life() == 0 &&
             myMana >= 10 &&
             distance(defender, env.get_monsters()[0]) < 1200 &&
             distance(env.get_monsters()[0], env.get_my_base().get_position()) < 2000)
    {
        action = SPELL;
        spellType = WIND;
    } // else if
    else if (distance(env.get_monsters()[0], env.get_my_base().get_position()) < 5000)
    {
        action = MOVE;
        tie(toX, toY) = env.get_monsters()[0].get_position();
    } // else if

    if (defender.get_shield_life() == 0)
    {
        for (auto o : env.get_entities())
        {
            if (distance(defender, o) < 3000)
            {
                action = SPELL;
                spellType = SHIELD;
                entityID = defender.get_id();
                break;
            } // if
        }     // for o
    }         // if

    // adding shields to other 2
    /*for (int i = 0; i < 3; i++)
    {
        if (entities[myHero][i].id != defender.id && entities[myHero][i].shield_life == 0 && distance(defender, entities[myHero][i]) <= 2000)
        {
            action = SPELL;
            spellType = SHIELD;
            entityID = entities[myHero][i].id;
            break;
        }//if
    }//for i*/

    defender.set_comment(" D ");
    if (action == MOVE)
        // cout << "MOVE " << toX << ' ' << toY << " D " << toX << ' ' << toY << endl;
        defender.move(make_pair(toX, toY));
    else if (action == SPELL)
    {
        myMana -= 10;

        if (spellType == WIND)
            // cout << "SPELL WIND " << opBaseX << ' ' << opBaseY << " D" << endl;
            defender.wind(env.get_opponent_base().get_position());
        else if (spellType == SHIELD)
            // cout << "SPELL SHIELD " << entityID << " D" << endl;
            defender.shield(entityID);
        else if (spellType == CONTROL)
            // cout << "SPELL CONTROL " << entityID << ' ' << toX << ' ' << toY << " D" << endl;
            defender.control(entityID, make_pair(toX, toY));
    } // else if
} // set_defender

void set_reaper(vector<vector<Entity>> &entities, int reaperID, int standX, int standY, bool farReaper = false)
{
    Entity reaper = entities[myHero][reaperID];

    // find closest monster w/ distToMyBase > 5000
    int targetMonsterIndex = 0, action;
    double minDist = 2e9;
    for (int i = 0; i < entities[monster].size(); i++)
    {
        if (entities[monster][i].distToMyBase <= 5000 || (!farReaper && 7000 <= entities[monster][i].distToMyBase))
            continue;

        double d = distance(reaper, entities[monster][i]);

        if (d < minDist)
        {
            targetMonsterIndex = i;
            minDist = d;
        } // if
    }     // for i

    if (entities[monster].empty())
        targetMonsterIndex = -1;

    int toX, toY;
    if (targetMonsterIndex == -1)
    {
        action = MOVE;

        if (myBaseX)
            toX = 17630 - standX, toY = 9000 - standY;
        else
            toX = standX, toY = standY;
    } // if
    else
    {
        action = MOVE;
        tie(toX, toY) = tie(entities[monster][targetMonsterIndex].x, entities[monster][targetMonsterIndex].y);
    } // else

    // check if opponents attacking
    int entityID, spellType;
    for (int i = 0; roundCnt > 100 && myMana >= 10 && i < entities[opHero].size(); i++)
    {
        if (entities[opHero][i].distToMyBase > 8000)
            continue;

        if (distance(reaper, entities[opHero][i]) < 1200)
        {
            action = SPELL;
            spellType = WIND;
            toX = opBaseX, toY = opBaseY;
            break;
        } // if
        else if (myMana >= 100 && distance(reaper, entities[opHero][i]) < 2100)
        {
            action = SPELL;
            spellType = CONTROL;
            entityID = entities[opHero][i].id;
            toX = opBaseX, toY = opBaseY;
            break;
        } // if
        else
        {
            action = MOVE;
            tie(toX, toY) = tie(entities[opHero][i].x, entities[opHero][i].y);
        } // else if
    }     // for i

    // adding shields
    if (roundCnt > 100 && myMana >= 10 && reaper.shield_life == 0)
    {
        bool opNear = false;
        for (int j = 0; j < entities[opHero].size(); j++)
        {
            if (distance(reaper, entities[opHero][j]) < 2500)
            {
                opNear = true;
                break;
            } // if
        }     // for j

        if (opNear)
        {
            action = SPELL;
            spellType = SHIELD;
            entityID = reaper.id;
        } // if
    }     // if

    if (action == MOVE)
        cout << "MOVE " << toX << ' ' << toY << " R " << toX << ' ' << toY << endl;
    else if (action == SPELL)
    {
        myMana -= 10;

        if (spellType == WIND)
            cout << "SPELL WIND " << toX << ' ' << toY << " R " << endl;
        else if (spellType == SHIELD)
            cout << "SPELL SHIELD " << entityID << " R " << endl;
        else if (spellType == CONTROL)
            cout << "SPELL CONTROL " << entityID << ' ' << toX << ' ' << toY << " R " << endl;
    } // else if

    return;
} // set_reaper

void set_attacker(vector<vector<Entity>> &entities, int attackerID)
{
    Entity attacker = entities[myHero][attackerID], target = entities[monster].back();

    int action = MOVE, toX, toY, spellType, entityID;

    if (myBaseX)
        toX = 7630, toY = 4500;
    else
        toX = 10000, toY = 4500;

    for (int i = entities[monster].size() - 1; i >= 0; i--)
    {
        target = entities[monster][i];

        if (target.distToMyBase < 10000)
        {
            if (abs((double)target.vx / (opBaseX - target.x) - (double)target.vy / (opBaseY - target.y)) < 0.001)
            {
                continue;
            } // if
            else if (distance(attacker, target) < 2190 && attacker.distToMyBase < 12000)
            {
                action = SPELL;
                spellType = CONTROL;
                entityID = target.id;
                toX = opBaseX, toY = opBaseY;
                break;
            } // else if
        }     // if
        else
        {
            if (distance(attacker, target) < 1270)
            {
                action = SPELL;
                spellType = WIND;
                toX = attacker.x + opBaseX - target.x, toY = attacker.y + opBaseY - target.y;
                break;
            } // if
            else if (target.shield_life == 0 && abs((double)target.vx / (opBaseX - target.x) - (double)target.vy / (opBaseY - target.y)) < 0.001 && distance(target, make_pair(opBaseX, opBaseY)) < 7000)
            {
                action = SPELL;
                spellType = SHIELD;
                entityID = target.id;
                break;
            } // else if
            else if (abs((double)target.vx / (opBaseX - target.x) - (double)target.vy / (opBaseY - target.y)) < 0.001)
            {
                continue;
            } // else if
            else if (distance(attacker, target) < 2190 && attacker.distToMyBase < 12000)
            {
                action = SPELL;
                spellType = CONTROL;
                entityID = target.id;
                toX = opBaseX, toY = opBaseY;
                break;
            } // if
            else
            {
                action = MOVE;
                toX = target.x, toY = target.y;
                break;
            } // else
        }     // else
    }         // for i

    if (attacker.distToMyBase < 7000)
    {
        action = MOVE;
        toX = opBaseX;
        toY = opBaseY;
    } // if

    if (sentCnt && distance(attacker, make_pair(opBaseX, opBaseY)) > 4000)
    {
        action = MOVE;
        toX = opBaseX;
        toY = opBaseY;
    } // if

    if (distance(attacker, make_pair(opBaseX, opBaseY)) < 4000)
        sentCnt = 0;
    else
        sentCnt = max(0, sentCnt - 1);

    // adding shields
    if (attacker.shield_life == 0)
    {
        bool opNear = false;
        for (int j = 0; j < entities[opHero].size(); j++)
        {
            if (distance(attacker, entities[opHero][j]) < 3000)
            {
                opNear = true;
                break;
            } // if
        }     // for j

        if (opNear || distance(attacker, make_pair(opBaseX, opBaseY)) < 6000)
        {
            action = SPELL;
            spellType = SHIELD;
            entityID = attacker.id;
        } // if
    }     // if

    if (action == MOVE)
        cout << "MOVE " << toX << ' ' << toY << " A " << toX << ' ' << toY << endl;
    else if (action == SPELL)
    {
        myMana -= 10;

        if (spellType == WIND)
            cout << "SPELL WIND " << toX << ' ' << toY << " A" << endl;
        else if (spellType == SHIELD)
            cout << "SPELL SHIELD " << entityID << " A" << endl;
        else if (spellType == CONTROL)
            cout << "SPELL CONTROL " << entityID << ' ' << toX << ' ' << toY << " A" << endl;

        if (1 || distance(attacker, make_pair(opBaseX, opBaseY)) < 8000)
        {
            for (auto m : entities[monster])
            {
                if (distance(attacker, m) < 2200 && m.health >= 10)
                    sentCnt++;
            } // for m
        }     // if
    }         // else if

    return;
} // set_attacker

int main()
{
    cin >> myBaseX >> myBaseY;
    cin.ignore();
    int heroes_per_player; // Always 3
    cin >> heroes_per_player;
    cin.ignore();

    opBaseX = 17630 - myBaseX, opBaseY = 9000 - myBaseY;

    // game loop
    while (1)
    {
        roundCnt++;
        cin >> myHealth >> myMana >> opHealth >> opMana;
        cin.ignore();

        int entity_count; // Amount of heros and monsters you can see
        vector<vector<Entity>> entities(3, vector<Entity>());

        cin >> entity_count;
        cin.ignore();
        for (int i = 0; i < entity_count; i++)
        {
            Entity tmp = get_entity();
            entities[tmp.type].emplace_back(tmp);
        } // for i

        sort(entities[monster].begin(), entities[monster].end(), [](auto a, auto b)
             { return a.distToMyBase < b.distToMyBase; });

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        bool shieldedMonster = false;
        for (auto m : entities[monster])
        {
            if (m.shield_life && m.distToMyBase < 5000)
            {
                shieldedMonster = true;
                break;
            } // if
        }     // for m

        if (!shieldedMonster && (entities[monster].empty() || entities[monster][0].distToMyBase > 2000 && entities[monster][0].distToMyBase > entities[myHero][0].distToMyBase))
            set_reaper(entities, 0, 1100, 7800);
        else
            set_defender(entities, 0);

        if ((!entities[monster].empty() && entities[monster][0].distToMyBase < 5000 && entities[myHero][1].distToMyBase < entities[myHero][0].distToMyBase && entities[myHero][1].distToMyBase < entities[myHero][2].distToMyBase) || shieldedMonster)
            set_defender(entities, 1);
        else
            set_reaper(entities, 1, 5400, 5500);

        // if (entities[myHero][2].distToMyBase < entities[myHero][0].distToMyBase && entities[myHero][2].distToMyBase < entities[myHero][1].distToMyBase) set_defender(entities, 2);
        // if (shieldedMonster) set_defender(entities, 2);
        // if (shieldedMonster) set_reaper(entities, 2, 5400, 5500);
        if (roundCnt < 100 || entities[monster].empty() || myMana < 20)
            set_reaper(entities, 2, 7800, 1100, true);
        else
            set_attacker(entities, 2);
    } // while
} // main
