#include "robot.hpp"
#include "vec2.hpp"

Robot::Robot(const Vec2 &position) : position_(position), direction_idx(0)
{
    this->velocity_ = Vec2(this->directions[this->direction_idx]);
}

Vec2 Robot::peek_position() const
{
    return this->get_position() + this->get_velocity();
}

Vec2 Robot::get_position() const
{
    return this->position_;
}

Vec2 Robot::get_velocity() const
{
    return this->velocity_;
}

void Robot::turn_right()
{
    this->direction_idx = (this->direction_idx + 1) % this->directions.size();
    this->velocity_ = Vec2(this->directions[this->direction_idx]);
}

void Robot::move()
{
    this->move(this->get_velocity());
}

void Robot::move(const Vec2 &v)
{
    this->position_ += v;
}
