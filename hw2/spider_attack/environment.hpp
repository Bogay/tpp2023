#pragma once

#include "base.hpp"
#include "entity.hpp"
#include <cstddef>
#include <vector>
#include <iostream>

namespace spider_attack
{
    class Environment
    {
    private:
        const int map_x = 17630;
        const int map_y = 9000;

        Base my_base_;
        Base opponent_base_;
        std::size_t heroes_per_player_;
        std::vector<Entity> entities_;
        std::vector<Hero> my_heroes_;
        std::vector<Monster> monsters_;

    public:
        Environment();
        static Environment read(std::istream &input = std::cin);
        void update(std::istream &input = std::cin);
        void debug(std::ostream &output = std::cerr) const;

        const Base &get_my_base() const;
        const Base &get_opponent_base() const;
        const std::size_t get_heroes_per_player() const;

        std::size_t get_entity_count() const;
        const Entity &get_entity(std::size_t index) const;
        const Hero &get_my_hero(std::size_t index) const;

        std::vector<Hero>::iterator begin();
        std::vector<Hero>::iterator end();
        std::vector<Hero>::const_iterator begin() const;
        std::vector<Hero>::const_iterator end() const;

        // Return copies of underlying entities
        const std::vector<Entity> get_entities() const;
        const std::vector<Hero> get_my_heroes() const;
        const std::vector<Monster> get_monsters() const;

        void write_actions(std::ostream &output = std::cout);
    };
}
