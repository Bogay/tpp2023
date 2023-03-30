#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "robot.hpp"
#include "maze.hpp"
#include "vec2.hpp"

using namespace std;

struct Input
{
    size_t w, h, n;
    vector<string> grid;
    static Input read(istream &in = cin);
};

Vec2 search_robot(const vector<string> &grid);
void output_answer(const Vec2 stop_at);
Vec2 simulate(const Maze &&maze, Robot &&robot, const size_t n);

int main()
{
    Input input = Input::read();
    Robot robot(search_robot(input.grid));
    const Maze maze = Maze::parse(input.grid);
    auto ans = simulate(move(maze), move(robot), input.n);
    output_answer(ans);

    return 0;
}

Input Input::read(istream &in)
{
    Input ret;
    string t;
    in >> ret.w >> ret.h >> ret.n;

    ret.grid.reserve(ret.h);
    for (size_t i = 0; i < ret.h; ++i)
    {
        in >> t;
        ret.grid.push_back(t);
    }

    return ret;
}

Vec2 search_robot(const vector<string> &grid)
{
    for (size_t i = 0; i < grid.size(); i++)
    {
        for (size_t j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] == TILE_ROBOT)
                return Vec2(j, i);
        }
    }

    return Vec2(-1, -1);
}

void output_answer(const Vec2 stop_at)
{
    cout << stop_at.get_x() << ' ' << stop_at.get_y() << '\n';
}

Vec2 simulate(const Maze &&maze, Robot &&robot, size_t n)
{
    vector<Vec2> visited_positions;
    map<pair<Vec2, Vec2>, size_t> last_visited_at;
    for (size_t step = 0; step < n; step++)
    {
        while (!maze.is_walkable(robot.peek_position()))
            robot.turn_right();

        auto visit_key = make_pair(
            robot.get_position(),
            robot.get_velocity());
        try
        {
            auto last_step = last_visited_at.at(visit_key);
            visited_positions.erase(
                visited_positions.begin(),
                visited_positions.begin() + last_step);
            auto cycle_len = visited_positions.size();
            return visited_positions[(n - step) % cycle_len];
        }
        catch (const out_of_range &)
        {
            last_visited_at[visit_key] = step;
        }
        visited_positions.push_back(robot.get_position());

        robot.move();
    }

    return robot.get_position();
}
