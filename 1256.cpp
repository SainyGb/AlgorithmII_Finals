#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<int> aux;
    vector<vector<int>> lista(110, aux);
    int m, n, testes, num;

    cin >> testes;

    while (testes--)
    {
        cin >> m >> n;

        for (int i = 0; i < n; i++)
        {
            cin >> num;
            // cout << num << " : " << num%m << "\n";
            lista[num % m].push_back(num);
        }

        for (int i = 0; i < m; i++)
        {
            cout << i << " -> ";

            for (int j = 0; j < lista[i].size(); j++)
            {
                cout << lista[i][j] << " -> ";
            }

            cout << "\\"
                 << "\n";
        }
        if (testes >= 1)
            cout << "\n";

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < lista[i].size(); j++)
            {
                lista[i].clear();
            }
        }
    }

    return 0;
}
