#include "vec2.hpp"
#include "maze.hpp"

Maze::Maze() {}

Maze Maze::parse(
    const std::vector<std::string> &grid,
    const char obstacle_symbol)
{
    Maze ret;
    ret.size_ = Vec2(grid[0].size(), grid.size());
    ret.grid_ = grid;
    ret.obstacle_symbol_ = obstacle_symbol;
    return ret;
}

Vec2 Maze::size() const
{
    return this->size_;
}

bool Maze::is_walkable(const Vec2 &position) const
{
    return !this->is_out_of_bound(position) && !this->is_obstacle(position);
}

bool Maze::is_out_of_bound(const Vec2 &position) const
{
    return position.get_x() < 0 || position.get_x() >= this->size().get_x() || position.get_y() < 0 || position.get_y() >= this->size().get_y();
}

bool Maze::is_obstacle(const Vec2 &position) const
{
    return this->at(position) == this->get_obstacle_symbol();
}

char Maze::get_obstacle_symbol() const
{
    return this->obstacle_symbol_;
}

char Maze::at(const Vec2 &position) const
{
    // TODO: abstraction?
    return this->grid_[position.get_y()][position.get_x()];
}
