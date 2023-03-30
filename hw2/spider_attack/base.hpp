#pragma once

#include <iostream>
#include <utility>

namespace spider_attack
{
    class Base
    {
    private:
        int health_;
        int mana_;
        int x_;
        int y_;

    public:
        void update(std::istream &input = std::cin);
        void debug(std::ostream &output = std::cerr) const;
        int get_health() const;
        int get_mana() const;
        std::pair<int, int> get_position() const;
        void set_position(std::pair<int, int> position);

        Base();
    };
} // namespace spider_attack
