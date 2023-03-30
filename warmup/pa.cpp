#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
    int n;
    while (cin >> n && n)
    {
        int p = 0;
        string s;
        while (n)
        {
            s += ('0' + n % 2);
            p += n % 2;
            n >>= 1;
        }

        reverse(s.begin(), s.end());

        cout << "The parity of " << s << " is " << p << " (mod 2).\n";
    }

    return 0;
}
