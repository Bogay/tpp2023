#pragma once

class Vec2
{
public:
    int get_x() const;
    int &get_x();
    int get_y() const;
    int &get_y();
    bool operator<(const Vec2 &rhs) const;
    Vec2 &operator+=(const Vec2 &rhs);
    Vec2 operator+(const Vec2 &rhs) const;
    Vec2();
    Vec2(const int x, const int y);

    static Vec2 up();
    static Vec2 down();
    static Vec2 left();
    static Vec2 right();

private:
    int x_, y_;
};
