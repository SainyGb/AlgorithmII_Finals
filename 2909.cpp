#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

constexpr int MAXN = 1e3 + 10;
constexpr ll mod = 1e9 + 7;
int numNetos, tamanhoGrupo[MAXN];
pii frutasNetos[MAXN];
ll resposta, fatorial[MAXN], inversoFatorial[MAXN];

struct Fracao
{
    ll numerador, denominador;
    Fracao() = default;
    Fracao(ll a, ll b) : numerador(a), denominador(b) {}
    bool operator<(const Fracao &f) const { return numerador * f.denominador < f.numerador * denominador; }
    bool operator==(const Fracao &f) const { return numerador * f.denominador == f.numerador * denominador; }
};

int tamanhoPontos;
Fracao pontos[MAXN * MAXN];
map<int, vi> intervalos[MAXN * MAXN];
vector<vi> grupos[MAXN * MAXN];

// Função para calcular a exponenciação modular (b^e % mod)
ll modpow(ll base, ll expoente)
{
    ll resultado = 1;
    while (expoente > 0)
    {
        if (expoente & 1)
            resultado = (resultado * base) % mod;
        base = (base * base) % mod;
        expoente >>= 1;
    }
    return resultado;
}

int main()
{
    // Pré-computação dos fatoriais e inversos dos fatoriais
    fatorial[0] = inversoFatorial[0] = 1;
    for (int i = 1; i < MAXN; i++)
    {
        fatorial[i] = (i * fatorial[i - 1]) % mod;
        inversoFatorial[i] = modpow(fatorial[i], mod - 2);
    }

    // Função para verificar se dois netos são intercambiáveis
    const auto naoSaoIntercambiaveis = [](int i, int j)
    {
        return (frutasNetos[j].first <= frutasNetos[i].first && frutasNetos[j].second <= frutasNetos[i].second) ||
               (frutasNetos[i].first <= frutasNetos[j].first && frutasNetos[i].second <= frutasNetos[j].second);
    };

    // Entrada do número de netos
    scanf("%d", &numNetos);
    int it = 0;
    for (int i = 0; i < numNetos; i++)
    {
        // Entrada das quantidades de frutas coletadas por cada neto
        scanf("%d%d", &frutasNetos[it].first, &frutasNetos[it].second);
        tamanhoGrupo[it] = 1;

        // Verifica se o neto atual tem as mesmas quantidades que algum neto anterior
        for (int j = 0; j < it; j++)
        {
            if (frutasNetos[j].first == frutasNetos[it].first && frutasNetos[j].second == frutasNetos[it].second)
            {
                // Se as quantidades coincidirem, mescla os netos no mesmo grupo
                tamanhoGrupo[j] += tamanhoGrupo[it];
                tamanhoGrupo[it] = 0;
                it--;
                break;
            }

            // Verifica se o neto atual e o neto anterior são intercambiáveis
            if (naoSaoIntercambiaveis(it, j))
                continue;

            // Calcula a diferença nas quantidades de frutas
            int diferencaA = frutasNetos[it].first - frutasNetos[j].first;
            int diferencaB = frutasNetos[j].second - frutasNetos[it].second;

            // Simplifica a diferença para sua forma mais simples
            int divisor = __gcd(diferencaA, diferencaB);
            diferencaA /= divisor;
            diferencaB /= divisor;

            // Armazena a fração representando a diferença nas quantidades
            pontos[tamanhoPontos++] = {diferencaB, diferencaA + diferencaB};
        }
        it++;
    }

    numNetos = it;

    // Ordena as frações em ordem crescente
    sort(pontos, pontos + tamanhoPontos);

    // Remove as duplicatas das frações
    tamanhoPontos = (int)(unique(pontos, pontos + tamanhoPontos) - pontos);

    ll total = 1;
    for (int i = 0; i < numNetos; i++)
    {
        // Calcula o fatorial total dos tamanhos de cada grupo
        total = (total * fatorial[tamanhoGrupo[i]]) % mod;
    }

    resposta = total;
    ll totalNegativo = (mod - total) % mod;

    // Constrói grupos com base nas frações que representam as diferenças nas quantidades
    for (int i = 0; i < numNetos; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (naoSaoIntercambiaveis(i, j))
                continue;

            // Calcula a diferença nas quantidades de frutas
            int diferencaA = frutasNetos[i].first - frutasNetos[j].first;
            int diferencaB = frutasNetos[j].second - frutasNetos[i].second;

            if (diferencaA < 0)
            {
                diferencaA = -diferencaA;
                diferencaB = -diferencaB;
            }

            // Simplifica a diferença para sua forma mais simples
            int divisor = __gcd(diferencaA, diferencaB);
            diferencaA /= divisor;
            diferencaB /= divisor;

            Fracao fracao{diferencaB, diferencaA + diferencaB};

            // Encontra o índice da fração no array de frações ordenadas
            int indice = (int)(lower_bound(pontos, pontos + tamanhoPontos, fracao) - pontos);

            // Calcula a função que relaciona a diferença com a posição relativa, usado para agrupar as diferenças equivalentes em um unico grupo
            int fx = diferencaB * frutasNetos[i].first + diferencaA * frutasNetos[i].second;
            vi &grupo = intervalos[indice][fx];

            // Adiciona os netos ao grupo correspondente
            grupo.push_back(i);
            grupo.push_back(j);
        }
    }

    // Constrói grupos de netos com as mesmas frações
    for (int i = 0; i < tamanhoPontos; i++)
    {
        for (auto &&par : intervalos[i])
        {
            vi &v = par.second;

            // Ordena e remove duplicatas dos netos no grupo
            sort(v.begin(), v.end());
            v.resize(unique(v.begin(), v.end()) - v.begin());

            // Adiciona o grupo ao array de grupos
            grupos[i].push_back(move(v));
        }
    }

    // Calcula a resposta final considerando todos os grupos e combinações
    for (int i = 0; i < tamanhoPontos; i++)
    {
        ll atual = total;
        // Itera sobre os grupos de netos com as mesmas frações
        for (auto &&grupo : grupos[i])
        {
            int contador = 0;
            // Itera sobre os netos no grupo atual
            for (int neto : grupo)
            {
                contador += tamanhoGrupo[neto];
                // Atualiza o valor atual multiplicando pelo inverso do fatorial do tamanho do grupo do neto
                atual = (atual * inversoFatorial[tamanhoGrupo[neto]]) % mod;
            }
            // Atualiza o valor atual multiplicando pelo fatorial do contador (soma dos tamanhos dos grupos)
            atual = (atual * fatorial[contador]) % mod;
        }
        // Adiciona o valor atual à resposta final
        resposta += atual;
        // Se a resposta final for maior ou igual a mod, é subtraído mod para manter o valor dentro do limite
        if (resposta >= mod)
            resposta -= mod;
        // É adicionado o valor total negativo à resposta final e se repete o passo anterior.
        resposta += totalNegativo;
        if (resposta >= mod)
            resposta -= mod;
    }

    // Imprime a resposta final
    printf("%lld\n", resposta);
}
