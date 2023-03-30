#pragma once

#include <vector>
#include "vec2.hpp"

class Robot
{
public:
    Robot(const Vec2 &position);
    Vec2 peek_position() const;
    Vec2 get_position() const;
    Vec2 get_velocity() const;
    void turn_right();
    void move();

private:
    void move(const Vec2 &v);

    Vec2 position_;
    Vec2 velocity_;
    size_t direction_idx;

    const std::vector<Vec2> directions{
        Vec2::up(),
        Vec2::right(),
        Vec2::down(),
        Vec2::left(),
    };
};
