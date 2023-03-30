#include <utility>
#include "vec2.hpp"

int Vec2::get_x() const
{
    return this->x_;
}

int &Vec2::get_x()
{
    return this->x_;
}

int Vec2::get_y() const
{
    return this->y_;
}

int &Vec2::get_y()
{
    return this->y_;
}

Vec2 &Vec2::operator+=(const Vec2 &rhs)
{
    this->get_x() += rhs.get_x();
    this->get_y() += rhs.get_y();
    return *this;
}

Vec2 Vec2::operator+(const Vec2 &rhs) const
{
    Vec2 ret(*this);
    ret += rhs;
    return ret;
}

bool Vec2::operator<(const Vec2 &rhs) const
{
    return std::make_pair(this->get_x(), this->get_y()) < std::make_pair(rhs.get_x(), rhs.get_y());
}

Vec2::Vec2() : Vec2(0, 0) {}
Vec2::Vec2(const int x, const int y) : x_(x), y_(y) {}

Vec2 Vec2::up()
{
    return Vec2(0, -1);
}

Vec2 Vec2::down()
{
    return Vec2(0, 1);
}

Vec2 Vec2::left()
{
    return Vec2(-1, 0);
}

Vec2 Vec2::right()
{
    return Vec2(1, 0);
}
