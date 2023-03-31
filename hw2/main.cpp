#include <iostream>
#include <algorithm>
#include <utility>
#include <cassert>
#include <map>
#include "spider_attack/environment.hpp"
#include "spider_attack/entity.hpp"
#include "spider_attack/base.hpp"

using namespace std;

inline int sq(const int x) { return x * x; }

int dis_sq(pair<int, int> pa, pair<int, int> pb)
{
    auto dx = pa.first - pb.first;
    auto dy = pa.second - pb.second;
    return sq(dx) + sq(dy);
}

pair<int, int> add_pos(pair<int, int> pa, pair<int, int> pb)
{
    return make_pair(pa.first + pb.first, pa.second + pb.second);
}

pair<int, int> sub_pos(pair<int, int> pa, pair<int, int> pb)
{
    return make_pair(pa.first - pb.first, pa.second - pb.second);
}

const spider_attack::Monster &select_target(
    pair<int, int> my_position,
    const vector<spider_attack::Monster> &monsters,
    const spider_attack::Environment &env)
{
    int selected_idx = -1;
    int min_dis_sq = -1;
    vector<int> thresholds{3000, 6000, 20000};
    for (auto threshold : thresholds)
    {
        for (size_t i = 0, c = min(monsters.size(), 3LU); i < c; i++)
        {
            if (dis_sq(monsters[i].get_position(), env.get_my_base().get_position()) > sq(threshold))
                continue;
            int d = dis_sq(my_position, monsters[i].get_position());
            if (selected_idx == -1 || d < min_dis_sq)
            {
                selected_idx = i;
                min_dis_sq = d;
            }
        }

        if (selected_idx != -1)
            return monsters[selected_idx];
    }
    return monsters[0];
}

vector<pair<int, int>> get_default_positions(const spider_attack::Environment &env)
{
    vector<pair<int, int>> ret{
        make_pair(4200, 600),
        make_pair(4400, 3400),
        make_pair(600, 4300),
    };

    if (env.get_my_base().get_position() != make_pair(0, 0))
    {
        for (auto &p : ret)
        {
            p.first = env.map_x - p.first;
            p.second = env.map_y - p.second;
        }
    }

    return ret;
}

int main()
{
    spider_attack::Environment env = spider_attack::Environment::read();
    auto default_positions = get_default_positions(env);
    map<int, bool> hero_is_controlled;

    auto threat_level = [env](const spider_attack::Monster &mon)
    {
        auto dis = dis_sq(mon.get_position(), env.get_my_base().get_position());
        if (dis > sq(6000))
            return INT32_MIN;
        int ratio = mon.is_targeting_me() ? 10 : 1;
        if (mon.is_targeting_me() && mon.get_shield_life() > 0)
            ratio *= 4;
        if (dis < sq(3000))
            ratio *= mon.get_health();
        return -dis * ratio;
    };

    auto cmp_monster = [threat_level](
                           const spider_attack::Monster &ma,
                           const spider_attack::Monster &mb)
    { return threat_level(ma) < threat_level(mb); };

    while (1)
    {
        env.update();

        auto monsters = env.get_monsters();
        std::sort(monsters.rbegin(), monsters.rend(), cmp_monster);
        for (const auto &mon : monsters)
        {
            cerr << threat_level(mon) << ' ';
            mon.debug(cerr);
        }

        for (spider_attack::Hero &hero : env)
        {
            // this hero is controlled last round
            if (hero_is_controlled[hero.get_id()])
            {
                if (!hero.is_controlled() && hero.get_shield_life() <= 0)
                    hero.shield(hero.get_id());
            }

            if (hero.is_controlled())
                hero_is_controlled[hero.get_id()] = true;
        }

        for (spider_attack::Hero &hero : env)
        {
            if (hero.get_shield_life() > 0 && hero.will_wait())
            {
                // try to find opponent hero
                for (const auto &ent : env.get_entities())
                {
                    if (dis_sq(hero.get_position(), ent.get_position()) < sq(env.wind_range))
                    {
                        hero.wind(env.get_opponent_base().get_position());
                        break;
                    }
                }
            }
        }

        if (!monsters.empty())
        {
            vector<spider_attack::Monster> selected_monsters;
            for (spider_attack::Hero &hero : env)
            {
                if (!hero.will_wait())
                    continue;
                auto mons_copy = monsters;
                for (auto &mon : selected_monsters)
                {
                    for (auto it = mons_copy.begin(); it != mons_copy.end(); it++)
                    {
                        if (it->get_id() == mon.get_id())
                        {
                            mons_copy.erase(it);
                            break;
                        }
                    }
                }

                if (mons_copy.empty())
                    mons_copy = monsters;

                auto target = select_target(hero.get_position(), mons_copy, env);
                hero.move(target.get_position());
                selected_monsters.push_back(target);
            }
        }
        else
        {
            auto it = default_positions.begin();
            for (spider_attack::Hero &hero : env)
            {
                if (!hero.will_wait())
                    continue;
                hero.move(*it);
                it++;
            }
        }

        int current_mana = env.get_my_base().get_mana();

        // try to make monsters move away oy base
        for (const auto &mon : monsters)
        {
            if (dis_sq(mon.get_position(), env.get_my_base().get_position()) > sq(2500))
                continue;
            if (mon.get_shield_life() > 0 || mon.is_controlled())
                continue;
            if (current_mana < env.spell_cost)
                continue;
            for (auto &hero : env)
            {
                if (dis_sq(hero.get_position(), mon.get_position()) < sq(env.control_range))
                {
                    hero.control(mon.get_id(), hero.get_position());
                    current_mana -= 10;
                    break;
                }
            }
        }

        // try wind
        for (spider_attack::Hero &hero : env)
        {
            if (dis_sq(hero.get_position(), env.get_my_base().get_position()) > sq(4000))
                continue;
            if (current_mana < env.spell_cost)
                break;
            if (hero.will_cast())
                continue;
            for (const auto &mon : monsters)
            {
                if (mon.get_shield_life() > 0)
                    continue;
                if (dis_sq(hero.get_position(), mon.get_position()) <= sq(env.wind_range))
                {
                    hero.wind(add_pos(hero.get_position(), sub_pos(env.get_opponent_base().get_position(), hero.get_position())));
                    current_mana -= env.spell_cost;
                    break;
                }
            }
        }

        // try to control more monsters moving to opponent base
        for (spider_attack::Hero &hero : env)
        {
            if (current_mana < env.spell_cost)
                break;
            if (!hero.will_wait())
                continue;
            for (const auto &mon : monsters)
            {
                if (mon.get_shield_life() > 0)
                    continue;
                if (mon.is_targeting_opponent())
                    continue;
                if (dis_sq(mon.get_position(), env.get_opponent_base().get_position()) > sq(7500))
                    continue;
                hero.control(mon.get_id(), env.get_opponent_base().get_position());
                current_mana -= env.spell_cost;
            }
        }

        env.write_actions();
    }

    return 0;
}
