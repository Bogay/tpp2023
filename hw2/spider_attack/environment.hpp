#ifndef SPIDER_ATTACKER_ENVIRONMENT_HPP
#define SPIDER_ATTACKER_ENVIRONMENT_HPP

#include "base.hpp"
#include "entity.hpp"
#include <cstddef>
#include <vector>
#include <iostream>
#include <functional>

namespace spider_attack
{
    class Environment
    {
    private:
        Base my_base_;
        Base opponent_base_;
        std::size_t heroes_per_player_;
        std::vector<Entity> opponent_heroes_;
        std::vector<Hero> my_heroes_;
        std::vector<Monster> monsters_;

    public:
        const int map_x = 17630;
        const int map_y = 9000;
        const int spell_cost = 10;
        const int wind_range = 1280;
        const int shield_range = 2200;
        const int control_range = 2200;

        Environment();
        static Environment read(std::istream &input = std::cin);
        void update(std::istream &input = std::cin);
        void debug(std::ostream &output = std::cerr) const;

        const Base &get_my_base() const;
        const Base &get_opponent_base() const;
        const std::size_t get_heroes_per_player() const;

        int estimate_mana() const;
        bool can_cast() const;

        std::size_t get_entity_count() const;
        const Entity &get_entity(std::size_t index) const;

        const Hero &get_my_hero(std::size_t index) const;
        Hero &get_my_hero(std::size_t index);
        std::vector<Hero>::iterator begin();
        std::vector<Hero>::iterator end();
        std::vector<Hero>::const_iterator begin() const;
        std::vector<Hero>::const_iterator end() const;

        void sort_monsters(std::function<bool(const Environment &, const Monster &, const Monster &)> cmp_fn);

        // Return copies of underlying entities
        const std::vector<Entity> get_opponent_heroes() const;
        const std::vector<Hero> get_my_heroes() const;
        const std::vector<Monster> get_monsters() const;

        void write_actions(std::ostream &output = std::cout);
    };
}

#endif
