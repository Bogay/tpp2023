#include <iostream>
#include <cmath>
#include "phah_titu/environment.hpp"
#include "phah_titu/entity.hpp"
#include "phah_titu/base.hpp"

using namespace std;

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

int roundCnt;
int sentCnt;

double distance(const phah_titu::Entity &a, const phah_titu::Entity &b)
{
    auto [ax, ay] = a.get_position();
    auto [bx, by] = b.get_position();
    return hypot(abs(ax - bx), abs(ay - by));
} // distance

double distance(const phah_titu::Entity &a, pair<int, int> b)
{
    auto [ax, ay] = a.get_position();
    return hypot(abs(ax - b.first), abs(ay - b.second));
} // distance

void set_defender(phah_titu::Environment &env, int defenderID)
{
    cerr << "become defender " << defenderID << '\n';

    auto &defender = env.get_my_hero(defenderID);
    int standX, standY, action = MOVE, toX, toY, spellType = WIND, entityID;

    if (env.get_my_base().get_position().first != 0)
        standX = 16830, standY = 8200;
    else
        standX = 800, standY = 800;

    toX = standX, toY = standY;

    // guarding of opponents
    for (int i = 0; env.can_cast() && i < env.get_opponent_heroes().size(); i++)
    {
        if (distance(defender, env.get_opponent_heroes()[i]) <= 1200)
        {
            action = SPELL;
            spellType = WIND;
            break;
        } // if
        else if (env.estimate_mana() >= 100 && distance(defender, env.get_opponent_heroes()[i]) <= 1500)
        {
            action = SPELL;
            spellType = CONTROL;
            entityID = env.get_opponent_heroes()[i].get_id();
            tie(toX, toY) = env.get_opponent_base().get_position();
            break;
        } // else if
    }     // for i

    // guarding of monsters
    if (env.get_monsters().empty())
        action = MOVE;
    else if (env.get_monsters()[0].get_shield_life() == 0 &&
             env.can_cast() &&
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
        for (const auto o : env.get_opponent_heroes())
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

    // TODO: add convenient method to construct comment
    defender.set_comment(" D ");
    if (action == MOVE)
        defender.move(make_pair(toX, toY));
    else if (action == SPELL)
    {
        if (spellType == WIND)
            defender.wind(env.get_opponent_base().get_position());
        else if (spellType == SHIELD)
            defender.shield(entityID);
        else if (spellType == CONTROL)
            defender.control(entityID, make_pair(toX, toY));
    } // else if
} // set_defender

void set_reaper(phah_titu::Environment &env, int reaperID, int standX, int standY, bool farReaper = false)
{
    cerr << "become reaper " << reaperID << '\n';
    auto &reaper = env.get_my_hero(reaperID);

    // find closest monster w/ distToMyBase > 5000
    int targetMonsterIndex = 0, action;
    double minDist = 2e9;
    auto get_dist_to_my_base = [env](const auto &ent)
    {
        return distance(ent, env.get_my_base().get_position());
    };
    for (int i = 0; i < env.get_monsters().size(); i++)
    {
        if (get_dist_to_my_base(env.get_monsters()[i]) <= 5000 || (!farReaper && 7000 <= get_dist_to_my_base(env.get_monsters()[i])))
            continue;

        double d = distance(reaper, env.get_monsters()[i]);

        if (d < minDist)
        {
            targetMonsterIndex = i;
            minDist = d;
        } // if
    }     // for i

    if (env.get_monsters().empty())
        targetMonsterIndex = -1;

    int toX, toY;
    if (targetMonsterIndex == -1)
    {
        action = MOVE;
        if (env.get_my_base().get_position().first)
            toX = env.map_x - standX, toY = env.map_y - standY;
        else
            toX = standX, toY = standY;
    } // if
    else
    {
        action = MOVE;
        tie(toX, toY) = env.get_monsters()[targetMonsterIndex].get_position();
    } // else

    // check if opponents attacking
    int entityID, spellType;
    for (int i = 0; roundCnt > 100 && env.can_cast() && i < env.get_opponent_heroes().size(); i++)
    {
        if (get_dist_to_my_base(env.get_opponent_heroes()[i]) > 8000)
            continue;

        if (distance(reaper, env.get_opponent_heroes()[i]) < 1200)
        {
            action = SPELL;
            spellType = WIND;
            tie(toX, toY) = env.get_opponent_base().get_position();
            break;
        } // if
        else if (env.estimate_mana() >= 100 && distance(reaper, env.get_opponent_heroes()[i]) < 2100)
        {
            action = SPELL;
            spellType = CONTROL;
            entityID = env.get_opponent_heroes()[i].get_id();
            tie(toX, toY) = env.get_opponent_base().get_position();
            break;
        } // if
        else
        {
            action = MOVE;
            tie(toX, toY) = env.get_opponent_heroes()[i].get_position();
        } // else if
    }     // for i

    // adding shields
    if (roundCnt > 100 && env.can_cast() && reaper.get_shield_life() == 0)
    {
        bool opNear = false;
        for (int j = 0; j < env.get_opponent_heroes().size(); j++)
        {
            if (distance(reaper, env.get_opponent_heroes()[j]) < 2500)
            {
                opNear = true;
                break;
            } // if
        }     // for j

        if (opNear)
        {
            action = SPELL;
            spellType = SHIELD;
            entityID = reaper.get_id();
        } // if
    }     // if

    reaper.set_comment(" R ");
    if (action == MOVE)
        reaper.move(make_pair(toX, toY));
    else if (action == SPELL)
    {
        if (spellType == WIND)
            reaper.wind(make_pair(toX, toY));
        else if (spellType == SHIELD)
            reaper.shield(entityID);
        else if (spellType == CONTROL)
            reaper.control(entityID, make_pair(toX, toY));
    } // else if

    return;
} // set_reaper

void set_attacker(phah_titu::Environment &env, int attackerID)
{
    cerr << "become attacker " << attackerID << '\n';

    auto &attacker = env.get_my_hero(attackerID);
    auto target = env.get_monsters().back();

    int action = MOVE, toX, toY, spellType, entityID;
    auto [opBaseX, opBaseY] = env.get_opponent_base().get_position();
    auto get_dist_to_my_base = [env](const auto &ent)
    {
        return distance(ent, env.get_my_base().get_position());
    };

    if (env.get_my_base().get_position().first)
        toX = 7630, toY = 4500;
    else
        toX = 10000, toY = 4500;

    for (int i = env.get_monsters().size() - 1; i >= 0; i--)
    {
        target = env.get_monsters()[i];
        auto x = (double)target.get_velocity().first / ((opBaseX - target.get_position().first) + 0.001);
        auto y = (double)target.get_velocity().second / ((opBaseY - target.get_position().second) + 0.001);
        auto will_to_op_base = abs(x - y) < 0.001;

        if (get_dist_to_my_base(target) < 10000)
        {
            if (will_to_op_base)
            {
                continue;
            } // if
            else if (distance(attacker, target) < 2190 && get_dist_to_my_base(attacker) < 12000)
            {
                action = SPELL;
                spellType = CONTROL;
                entityID = target.get_id();
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
                toX = attacker.get_position().first + opBaseX - target.get_position().first,
                toY = attacker.get_position().second + opBaseY - target.get_position().second;
                break;
            } // if
            else if (target.get_shield_life() == 0 && will_to_op_base && distance(target, make_pair(opBaseX, opBaseY)) < 7000)
            {
                action = SPELL;
                spellType = SHIELD;
                entityID = target.get_id();
                break;
            } // else if
            else if (will_to_op_base)
            {
                continue;
            } // else if
            else if (distance(attacker, target) < 2190 && get_dist_to_my_base(attacker) < 12000)
            {
                action = SPELL;
                spellType = CONTROL;
                entityID = target.get_id();
                toX = opBaseX, toY = opBaseY;
                break;
            } // if
            else
            {
                action = MOVE;
                tie(toX, toY) = target.get_position();
                break;
            } // else
        }     // else
    }         // for i

    if (get_dist_to_my_base(attacker) < 7000)
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
    if (attacker.get_shield_life() == 0)
    {
        bool opNear = false;
        for (int j = 0; j < env.get_opponent_heroes().size(); j++)
        {
            if (distance(attacker, env.get_opponent_heroes()[j]) < 3000)
            {
                opNear = true;
                break;
            } // if
        }     // for j

        if (opNear || distance(attacker, make_pair(opBaseX, opBaseY)) < 6000)
        {
            action = SPELL;
            spellType = SHIELD;
            entityID = attacker.get_id();
        } // if
    }     // if

    attacker.set_comment(" A ");
    if (action == MOVE)
        attacker.move(make_pair(toX, toY));
    else if (action == SPELL)
    {
        if (spellType == WIND)
            attacker.wind(make_pair(toX, toY));
        else if (spellType == SHIELD)
            attacker.shield(entityID);
        else if (spellType == CONTROL)
            attacker.control(entityID, make_pair(toX, toY));

        if (1 || distance(attacker, make_pair(opBaseX, opBaseY)) < 8000)
        {
            for (auto m : env.get_monsters())
            {
                if (distance(attacker, m) < 2200 && m.get_health() >= 10)
                    sentCnt++;
            } // for m
        }     // if
    }         // else if

    return;
} // set_attacker

int main()
{
    phah_titu::Environment env = phah_titu::Environment::read();

    // game loop
    while (1)
    {
        roundCnt++;
        env.update();

        auto get_dist_to_my_base = [env](const auto &ent)
        {
            return distance(ent, env.get_my_base().get_position());
        };

        env.sort_monsters([](
                              const auto &env,
                              const auto &a,
                              const auto &b)
                          { 
                            auto da = distance(a, env.get_my_base().get_position());
                            auto db = distance(b, env.get_my_base().get_position());
                            return da < db; });

        bool shieldedMonster = false;
        for (const auto m : env.get_monsters())
        {
            if (m.get_shield_life() && get_dist_to_my_base(m) < 5000)
            {
                shieldedMonster = true;
                break;
            } // if
        }     // for m

        if (!shieldedMonster &&
            (env.get_monsters().empty() ||
             get_dist_to_my_base(env.get_monsters()[0]) > 2000 &&
                 !env.get_opponent_heroes().empty() &&
                 get_dist_to_my_base(env.get_monsters()[0]) > get_dist_to_my_base(env.get_opponent_heroes()[0])))
            set_reaper(env, 0, 1100, 7800);
        else
            set_defender(env, 0);

        if ((!env.get_monsters().empty() &&
             get_dist_to_my_base(env.get_monsters()[0]) < 5000 &&
             get_dist_to_my_base(env.get_my_heroes()[1]) < get_dist_to_my_base(env.get_my_heroes()[0]) &&
             get_dist_to_my_base(env.get_my_heroes()[1]) < get_dist_to_my_base(env.get_my_heroes()[2])) ||
            shieldedMonster)
            set_defender(env, 1);
        else
            set_reaper(env, 1, 5400, 5500);

        if (roundCnt < 100 || env.get_monsters().empty() || env.get_my_base().get_mana() < 20)
            set_reaper(env, 2, 7800, 1100, true);
        else
            set_attacker(env, 2);

        env.write_actions();
    } // while
} // main
