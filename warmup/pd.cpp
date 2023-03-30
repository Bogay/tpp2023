#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

void clear(vector<vector<char>> &world, int r, int c, char x)
{
    if (r < 0 || c < 0)
        return;
    if (r >= world.size() || c >= world[0].size())
        return;
    if (world[r][c] != x)
        return;

    world[r][c] = 0;
    clear(world, r + 1, c, x);
    clear(world, r - 1, c, x);
    clear(world, r, c + 1, x);
    clear(world, r, c - 1, x);
}

int main()
{
    int t;
    cin >> t;
    for (int ti = 1; ti <= t; ti++)
    {
        int r, c;
        cin >> r >> c;
        vector<vector<char>> world(r);
        map<char, int> cnt;

        for (int i = 0; i < r; i++)
        {
            world[i].resize(c);
            for (int j = 0; j < c; j++)
            {
                cin >> world[i][j];
            }
        }

        for (size_t i = 0; i < r; i++)
        {
            for (size_t j = 0; j < c; j++)
            {
                if (world[i][j])
                {
                    cnt[world[i][j]]++;
                    clear(world, i, j, world[i][j]);
                }
            }
        }

        vector<pair<int, char>> res;
        res.reserve(world.size());
        for (auto kv : cnt)
        {
            res.push_back(make_pair(kv.second, kv.first));
        }
        sort(res.begin(), res.end(), [](pair<int, char> &a, pair<int, char> &b)
             {
            if(a.first != b.first)
                return a.first > b.first;
            return a.second < b.second; });

        cout << "World #" << ti << '\n';
        for (auto kv : res)
        {
            cout << kv.second << ": " << kv.first << '\n';
        }
    }

    return 0;
}