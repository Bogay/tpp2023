#include <iostream>
#include <vector>

using namespace std;

size_t index(char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    if ('A' <= c && c <= 'Z')
        return c - 'A' + 10;
    return c - 'a' + 36;
}

char to_chr(size_t i)
{
    if (0 <= i && i <= 9)
        return '0' + i;
    if (10 <= i && i <= 35)
        return 'A' + (i - 10);
    return 'a' + (i - 36);
}

bool is_prime(int n)
{
    if (n < 2)
        return false;

    for (int i = 2; i <= (n / 2); i++)
        if (n % i == 0)
            return false;
    return true;
}

int main()
{
    int t;

    cin >> t;
    for (int ti = 1; ti <= t; ti++)
    {
        const int N = 62;
        string s;
        vector<int> cnt(N, 0);
        cin >> s;

        for (auto ch : s)
        {
            cnt[index(ch)] += 1;
        }

        string pf = "";
        for (size_t i = 0; i < N; i++)
        {
            if (is_prime(cnt[i]))
                pf += to_chr(i);
        }

        cout << "Case " << ti << ": ";
        if (pf.empty())
        {
            cout << "empty\n";
        }
        else
        {
            cout << pf << '\n';
        }
    }

    return 0;
}