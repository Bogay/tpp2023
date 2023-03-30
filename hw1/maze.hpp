#pragma once

#include "vec2.hpp"
#include <string>
#include <vector>
#define TILE_OBSTACLE '#'
#define TILE_GROUND '.'
#define TILE_ROBOT 'O'

class Maze
{
public:
    static Maze parse(
        const std::vector<std::string> &grid,
        const char obstacle_symbol = TILE_OBSTACLE);
    Vec2 size() const;
    bool is_walkable(const Vec2 &position) const;

private:
    Maze();
    bool is_out_of_bound(const Vec2 &position) const;
    bool is_obstacle(const Vec2 &position) const;
    char get_obstacle_symbol() const;
    char at(const Vec2 &position) const;

    Vec2 size_;
    std::vector<std::string> grid_;
    char obstacle_symbol_;
};
