#include "sah.h"
#include <iostream>
#include <cmath>

using namespace std;

bool InRange(Koordinata k)
{
    return (k.x >= 0 && k.x < 8 && k.y >= 0 && k.y < 8);
}


Figura::Figura(Koordinata k, char boja, string slovo)
{
    this->boja = boja;
    this->k = k;
    this->slovo = slovo;
}

void Figura::namestiKoordinatu(Koordinata k)
{
    this->k = k;
}

void Figura::jede(Koordinata k, Tabla& t)
{
    t.UkloniFiguruNa(k);
}

bool Figura::MoveTo(Koordinata k, Tabla& t)
{
    if (!InRange(k)) return false;
    if (!CanMoveTo(k, t)) return false;
    jede(k, t);
    namestiKoordinatu(k);
    return true;
}



Top::Top(Koordinata k, char boja, string slovo)
    : Figura(k, boja, slovo) {
}

bool Top::CanMoveTo(Koordinata k, Tabla& t)
{
    if (k.x != this->k.x && k.y != this->k.y) return false;
    const Figura* f = t.FiguraNaPolju(k);
    if (f && this->boja == f->vratiBoju())
        return false;

    if (k.x == this->k.x)
    {
        int distanca = abs(k.y - this->k.y);
        int dir = (k.y > this->k.y) ? 1 : -1;
        for (int i = 1; i < distanca; i++)
        {
            Koordinata m = { this->k.x, this->k.y + dir * i };
            if (t.FiguraNaPolju(m))
                return false;
        }
        return true;
    }
    if (k.y == this->k.y)
    {
        int distanca = abs(k.x - this->k.x);
        int dir = (k.x > this->k.x) ? 1 : -1;
        for (int i = 1; i < distanca; i++)
        {
            Koordinata m = { this->k.x + dir * i, this->k.y };
            if (t.FiguraNaPolju(m))
                return false;
        }
        return true;
    }
    return false;
}



Lovac::Lovac(Koordinata k, char boja, string slovo)
    : Figura(k, boja, slovo) {
}

bool Lovac::CanMoveTo(Koordinata k, Tabla& t)
{
    Koordinata l = this->k;
    auto o = t.FiguraNaPolju(k);
    if (o && this->boja == o->vratiBoju())
        return false;

    int dx = (k.x > l.x) ? 1 : -1;
    int dy = (k.y > l.y) ? 1 : -1;
    int dx0 = k.x - l.x;
    int dy0 = k.y - l.y;
    if (abs(dx0) != abs(dy0)) return false;

    int n = abs(k.x - l.x);
    for (int j = 1; j < n; j++)
    {
        Koordinata m = { l.x + dx * j, l.y + dy * j };
        if (t.FiguraNaPolju(m))
            return false;
    }
    return true;
}


Konj::Konj(Koordinata k, char boja, string slovo)
    : Figura(k, boja, slovo) {
}

bool Konj::CanMoveTo(Koordinata k, Tabla& t)
{
    if (!InRange(k)) return false;

    const vector<pair<int, int>> pomeraji = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        { 1, -2}, { 1, 2}, { 2, -1}, { 2, 1}
    };

    for (int i = 0; i < 8; i++)
    {
        if (this->k.x + pomeraji[i].first == k.x &&
            this->k.y + pomeraji[i].second == k.y)
        {
            const Figura* f = t.FiguraNaPolju(k);
            if (!f || f->vratiBoju() != this->vratiBoju())
                return true;
        }
    }
    return false;
}


Pijun::Pijun(Koordinata k, char boja, string slovo)
    : Figura(k, boja, slovo) {
}

bool Pijun::CanMoveTo(Koordinata k, Tabla& t)
{
    if (!InRange(k)) return false;

    int dir = (boja == 'w') ? 1 : -1;
    int dx = k.x - this->k.x;
    int dy = k.y - this->k.y;

    const Figura* naCilju = t.FiguraNaPolju(k);

    if (dy == 0 && dx == dir && naCilju == nullptr)
        return true;

    if (dx == dir && abs(dy) == 1 && naCilju != nullptr &&
        naCilju->vratiBoju() != boja)
        return true;

    return false;
}


Kralj::Kralj(Koordinata k, char boja, string slovo)
    : Figura(k, boja, slovo) {
}

bool Kralj::CanMoveTo(Koordinata k, Tabla& t)
{
    if (!InRange(k)) return false;

    auto f = t.FiguraNaPolju(k);
    if (f && f->vratiBoju() == this->vratiBoju())
        return false;

    Koordinata pomeraji[8] = {
        { -1, 0 }, { 1, 0 },  { 0,-1 }, { 0, 1 },
        { -1,-1 }, { -1, 1 }, { 1,-1 }, { 1, 1 }
    };

    for (int i = 0; i < 8; ++i)
    {
        Koordinata p = { this->k.x + pomeraji[i].x,
                         this->k.y + pomeraji[i].y };
        if (const Figura* fig = t.FiguraNaPolju(p))
        {
            if (dynamic_cast<const Kralj*>(fig))
                return false;
        }
    }

    if (abs(this->k.x - k.x) > 1 || abs(this->k.y - k.y) > 1)
        return false;

    return true;
}


Kraljica::Kraljica(Koordinata k, char boja, string slovo)
    : Figura(k, boja, slovo) {
}

bool Kraljica::CanMoveTo(Koordinata k, Tabla& t)
{
    if (!InRange(k)) return false;
    const Figura* f = t.FiguraNaPolju(k);
    if (f && this->boja == f->vratiBoju())
        return false;

    if (k.x == this->k.x || k.y == this->k.y)
    {
        if (k.x == this->k.x)
        {
            int distanca = abs(k.y - this->k.y);
            int dir = (k.y > this->k.y) ? 1 : -1;
            for (int i = 1; i < distanca; i++)
            {
                Koordinata m = { this->k.x, this->k.y + dir * i };
                if (t.FiguraNaPolju(m))
                    return false;
            }
            return true;
        }
        if (k.y == this->k.y)
        {
            int distanca = abs(k.x - this->k.x);
            int dir = (k.x > this->k.x) ? 1 : -1;
            for (int i = 1; i < distanca; i++)
            {
                Koordinata m = { this->k.x + dir * i, this->k.y };
                if (t.FiguraNaPolju(m))
                    return false;
            }
            return true;
        }
    }

    Koordinata l = this->k;
    int dx = (k.x > l.x) ? 1 : -1;
    int dy = (k.y > l.y) ? 1 : -1;
    int dx0 = k.x - l.x;
    int dy0 = k.y - l.y;
    if (abs(dx0) != abs(dy0)) return false;

    int n = abs(k.x - l.x);
    for (int j = 1; j < n; j++)
    {
        Koordinata m = { l.x + dx * j, l.y + dy * j };
        if (t.FiguraNaPolju(m))
            return false;
    }
    return true;
}


Tabla::Tabla()
{
    Koordinata k;

    k = { 7, 0 }; ubaciFiguru(new Top(k, 'b', "BR1"));
    k = { 7, 7 }; ubaciFiguru(new Top(k, 'b', "BR2"));

    k = { 7, 1 }; ubaciFiguru(new Konj(k, 'b', "BK1"));
    k = { 7, 6 }; ubaciFiguru(new Konj(k, 'b', "BK2"));

    k = { 7, 2 }; ubaciFiguru(new Lovac(k, 'b', "BB1"));
    k = { 7, 5 }; ubaciFiguru(new Lovac(k, 'b', "BB2"));

    k = { 7, 3 }; ubaciFiguru(new Kraljica(k, 'b', "BQ1"));
    k = { 7, 4 }; ubaciFiguru(new Kralj(k, 'b', "BK1"));

    for (int y = 0; y < 8; ++y)
    {
        k = { 6, y };
        ubaciFiguru(new Pijun(k, 'b', "BP" + to_string(y + 1)));
    }

    k = { 0, 0 }; ubaciFiguru(new Top(k, 'w', "WR1"));
    k = { 0, 7 }; ubaciFiguru(new Top(k, 'w', "WR2"));

    k = { 0, 1 }; ubaciFiguru(new Konj(k, 'w', "WK1"));
    k = { 0, 6 }; ubaciFiguru(new Konj(k, 'w', "WK2"));

    k = { 0, 2 }; ubaciFiguru(new Lovac(k, 'w', "WB1"));
    k = { 0, 5 }; ubaciFiguru(new Lovac(k, 'w', "WB2"));

    k = { 0, 3 }; ubaciFiguru(new Kraljica(k, 'w', "WQ1"));
    k = { 0, 4 }; ubaciFiguru(new Kralj(k, 'w', "WK1"));

    for (int y = 0; y < 8; ++y)
    {
        k = { 1, y };
        ubaciFiguru(new Pijun(k, 'w', "WP" + to_string(y + 1)));
    }
}

Tabla::~Tabla()
{
    for (auto it : figure)
        delete it;
}

void Tabla::ubaciFiguru(Figura* f)
{
    figure.push_back(f);
}

Figura* Tabla::vratiFiguruIme(string ime)
{
    for (auto f : figure)
    {
        if (f->vratiSlovo() == ime)
            return f;
    }
    return nullptr;
}

const Figura* Tabla::FiguraNaPolju(Koordinata k) const
{
    if (!InRange(k))
        return nullptr;
    for (Figura* f : figure)
    {
        if (f->vratiKordinatu().x == k.x &&
            f->vratiKordinatu().y == k.y)
        {
            return f;
        }
    }
    return nullptr;
}

void Tabla::UkloniFiguruNa(Koordinata k)
{
    for (auto it = figure.begin(); it != figure.end(); ++it)
    {
        if ((*it)->vratiKordinatu().x == k.x &&
            (*it)->vratiKordinatu().y == k.y)
        {
            delete* it;
            figure.erase(it);
            break;
        }
    }
}

void Tabla::ispis() const
{
    for (int i = 7; i >= 0; i--)
    {
        cout << i + 1 << " ";
        for (int j = 0; j < 8; j++)
        {
            const Figura* f = FiguraNaPolju({ i,j });
            if (f)
                cout << f->vratiSlovo() << " ";
            else
                cout << "... ";
        }
        cout << endl;
    }
    cout << "   a   b   c   d   e   f   g   h\n";
}

bool Tabla::sah(char boja, Tabla& t)
{
    Figura* kralj = nullptr;
    for (auto it : figure)
    {
        if (dynamic_cast<Kralj*>(it) && it->vratiBoju() != boja)
        {
            kralj = it;
            break;
        }
    }
    if (!kralj) return false;

    Koordinata kk = kralj->vratiKordinatu();
    for (auto it : figure)
    {
        if (it->vratiBoju() != kralj->vratiBoju() &&
            it->CanMoveTo(kk, t))
        {
            return true;
        }
    }
    return false;
}

bool Tabla::sahmat(char boja)
{
    if (!sah(boja, *this)) return false;

    Figura* kralj = nullptr;
    for (auto it : figure)
    {
        if (dynamic_cast<Kralj*>(it) && it->vratiBoju() != boja)
        {
            kralj = it;
            break;
        }
    }
    if (!kralj) return false;

    Koordinata pozicije[8] = {
        {1,1},{-1,-1},{-1,1},{1,-1},
        {0,1},{1,0},{-1,0},{0,-1}
    };
    bool moguce[8] = { true,true,true,true,true,true,true,true };

    for (int i = 0; i < 8; i++)
    {
        Koordinata k = {
            kralj->vratiKordinatu().x + pozicije[i].x,
            kralj->vratiKordinatu().y + pozicije[i].y
        };

        if (!InRange(k))
        {
            moguce[i] = false;
            continue;
        }

        if (const Figura* f = this->FiguraNaPolju(k))
        {
            if (f->vratiBoju() == kralj->vratiBoju())
                moguce[i] = false;
        }
    }

    for (int i = 0; i < 8; i++)
    {
        if (!moguce[i])
            continue;

        Koordinata k = {
            kralj->vratiKordinatu().x + pozicije[i].x,
            kralj->vratiKordinatu().y + pozicije[i].y
        };

        for (Figura* f : figure)
        {
            if (f->vratiBoju() == kralj->vratiBoju())
                continue;

            if (f->CanMoveTo(k, *this))
            {
                moguce[i] = false;
                break;
            }
        }
    }

    for (int i = 0; i < 8; i++)
    {
        if (moguce[i])
            return false;
    }
    return true;
}

int main()
{
    char boje[2] = { 'w','b' };
    Tabla* t = new Tabla();
    int i = 0;
    string potez;
    Figura* r;
    Koordinata e;

    t->ispis();

    while (!t->sahmat(boje[i]))
    {
        cout << "Na potezu je ";
        string boja = (boje[i] == 'w') ? "beli" : "crni";
        cout << boja << " igrac:" << endl;

        while (true)
        {
            cout << "Unesi potez (npr. WR1e4): ";
            cin >> potez;

            if (potez.size() < 5) {
                cout << "Los format poteza!" << endl;
                continue;
            }

            string imeFigure = potez.substr(0, 3);
            r = t->vratiFiguruIme(imeFigure);

            if (!r) {
                cout << "Ne postoji figura sa imenom " << imeFigure << endl;
                continue;
            }

            char xs = potez[3];
            char ys = potez[4];

            int y = xs - 'a';
            int x = ys - '1';

            e = { x, y };

            if (!InRange(e)) {
                cout << "Polje van table!" << endl;
                continue;
            }

            if (r->MoveTo(e, *t)) {
                break;
            }
            else {
                cout << "Nevalidan potez, pokusaj ponovo." << endl;
            }
        }
        i = 1 - i;
        t->ispis();
    }

    cout << "Sah-mat! Pobedio je " << ((boje[i] == 'w') ? "crni" : "beli") << " igrac." << endl;

    delete t;
    return 0;
}
