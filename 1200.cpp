#include <iostream>
#include <vector>
#include <string>

using namespace std;

typedef char Valor;

class No
{
public:
    virtual Valor valor() = 0;
    virtual No *esquerda() = 0;
    virtual No *direita() = 0;
    virtual No *inserir(Valor v) = 0;
    virtual int altura() = 0;
    virtual bool pesquisar(Valor v) = 0;
    virtual vector<Valor> infixa() = 0;
    virtual vector<Valor> prefixa() = 0;
    virtual vector<Valor> posfixa() = 0;
};

class Arvore : public No
{
private:
    Valor valor_;
    No *esquerda_;
    No *direita_;

public:
    Arvore(Valor valor, No *esquerda, No *direita) : valor_(valor), esquerda_(esquerda), direita_(direita) {}

    Valor valor() override
    {
        return valor_;
    }

    No *esquerda() override
    {
        return esquerda_;
    }

    No *direita() override
    {
        return direita_;
    }

    No *inserir(Valor v) override
    {
        if (v < valor_)
        {
            return new Arvore(valor_, esquerda_->inserir(v), direita_);
        }
        else
        {
            return new Arvore(valor_, esquerda_, direita_->inserir(v));
        }
    }

    int altura() override
    {
        return max(esquerda_->altura(), direita_->altura()) + 1;
    }

    bool pesquisar(Valor v) override
    {
        return v == valor_ || (v < valor_ ? esquerda_->pesquisar(v) : direita_->pesquisar(v));
    }

    vector<Valor> infixa() override
    {
        vector<Valor> resultado = esquerda_->infixa();
        resultado.push_back(valor_);
        vector<Valor> direitaInfixa = direita_->infixa();
        resultado.insert(resultado.end(), direitaInfixa.begin(), direitaInfixa.end());
        return resultado;
    }

    vector<Valor> prefixa() override
    {
        vector<Valor> resultado;
        resultado.push_back(valor_);
        vector<Valor> esquerdaPrefixa = esquerda_->prefixa();
        resultado.insert(resultado.end(), esquerdaPrefixa.begin(), esquerdaPrefixa.end());
        vector<Valor> direitaPrefixa = direita_->prefixa();
        resultado.insert(resultado.end(), direitaPrefixa.begin(), direitaPrefixa.end());
        return resultado;
    }

    vector<Valor> posfixa() override
    {
        vector<Valor> resultado = esquerda_->posfixa();
        vector<Valor> direitaPosfixa = direita_->posfixa();
        resultado.insert(resultado.end(), direitaPosfixa.begin(), direitaPosfixa.end());
        resultado.push_back(valor_);
        return resultado;
    }
};

class Folha : public No
{
private:
    Valor valor_;

public:
    Folha(Valor valor) : valor_(valor) {}

    Valor valor() override
    {
        return valor_;
    }

    No *esquerda() override
    {
        return this;
    }

    No *direita() override
    {
        return this;
    }

    No *inserir(Valor v) override
    {
        return new Arvore(v, new Folha(v), new Folha(v));
    }

    int altura() override
    {
        return -1;
    }

    bool pesquisar(Valor v) override
    {
        return false;
    }

    vector<Valor> infixa() override
    {
        return vector<Valor>();
    }

    vector<Valor> prefixa() override
    {
        return vector<Valor>();
    }

    vector<Valor> posfixa() override
    {
        return vector<Valor>();
    }
};

int main()
{
    No *arvore = new Folha('\0');
    string entrada;

    while (!cin.eof())
    {
        getline(cin, entrada);
        vector<string> a;
        size_t pos = 0;
        string token;
        while ((pos = entrada.find(" ")) != string::npos)
        {
            token = entrada.substr(0, pos);
            a.push_back(token);
            entrada.erase(0, pos + 1);
        }
        a.push_back(entrada);

        if (a[0] == "I")
        {
            arvore = arvore->inserir(a[1][0]);
        }
        else if (a[0] == "P")
        {
            Valor valor = a[1][0];
            if (arvore->pesquisar(valor))
            {
                cout << valor << " existe" << endl;
            }
            else
            {
                cout << valor << " nao existe" << endl;
            }
        }
        else if (a[0] == "INFIXA")
        {
            vector<Valor> infixa = arvore->infixa();
            for (const auto &v : infixa)
            {
                cout << v << " ";
            }
            cout << endl;
        }
        else if (a[0] == "POSFIXA")
        {
            vector<Valor> posfixa = arvore->posfixa();
            for (const auto &v : posfixa)
            {
                cout << v << " ";
            }
            cout << endl;
        }
        else if (a[0] == "PREFIXA")
        {
            vector<Valor> prefixa = arvore->prefixa();
            for (const auto &v : prefixa)
            {
                cout << v << " ";
            }
            cout << endl;
        }
    }

    return 0;
}
