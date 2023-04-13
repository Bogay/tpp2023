#include <iostream>
#include "base.hpp"

namespace phah_titu
{
    Base::Base() {}

    void Base::update(std::istream &input)
    {
        input >> this->health_ >> this->mana_;
        input.ignore();
    }

    void Base::debug(std::ostream &output) const
    {
        output << "health: " << this->get_health() << '\n';
        output << "mana: " << this->get_mana() << '\n';
        output << "position: " << this->x_ << ", " << this->y_ << '\n';
    }

    int Base::get_health() const
    {
        return this->health_;
    }

    int Base::get_mana() const
    {
        return this->mana_;
    }

    std::pair<int, int> Base::get_position() const
    {
        return std::make_pair(this->x_, this->y_);
    }

    void Base::set_position(std::pair<int, int> position)
    {
        this->x_ = position.first;
        this->y_ = position.second;
    }
}