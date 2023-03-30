#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

const string ROCK = "rock";
const string PAPER = "paper";
const string SCISSORS = "scissors";

// 1: win, 0: draw, -1: lose
int result(const string &a, const string &b)
{
    if (a == ROCK)
    {
        if (b == ROCK)
            return 0;
        else if (b == PAPER)
            return -1;
        else
            return 1;
    }
    if (a == PAPER)
    {
        if (b == ROCK)
            return 1;
        else if (b == PAPER)
            return 0;
        else
            return -1;
    }
    if (a == SCISSORS)
    {
        if (b == ROCK)
            return -1;
        else if (b == PAPER)
            return 1;
        else
            return 0;
    }

    exit(10086);
}

int main()
{
    int n, k;
    bool first = true;
    while (cin >> n && n && cin >> k)
    {
        if (first)
            first = false;
        else
            cout << '\n';

        int t = k * n * (n - 1) / 2;
        // win, lose
        vector<pair<int, int>> results(n + 1);

        while (t--)
        {
            int p1, p2;
            string m1, m2;

            cin >> p1 >> m1 >> p2 >> m2;
            auto r = result(m1, m2);

            if (r > 0)
            {
                results[p1].first++;
                results[p2].second++;
            }
            else if (r < 0)
            {
                results[p1].second++;
                results[p2].first++;
            }
        }

        for (int i = 1; i <= n; i++)
        {
            auto w = results[i].first;
            auto l = results[i].second;

            if (w + l == 0)
                cout << '-';
            else
            {
                auto r = (float)w / (w + l) * 1000;
                auto rr = round(r) / 1000.;
                cout << fixed << setprecision(3) << rr;
            }
            cout << '\n';
        }
    }

    return 0;
}
