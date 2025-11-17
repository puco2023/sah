#define SAH_H
#include <string>
#include <vector>

struct Koordinata
{
    int x;
    int y;
};

class Tabla;
bool InRange(Koordinata k);

class Figura
{
protected:
    Koordinata k;
    char boja;
    std::string slovo;
public:
    Figura(Koordinata k, char boja, std::string slovo);
    virtual ~Figura() = default;

    virtual bool CanMoveTo(Koordinata k, Tabla& t) = 0;
    bool MoveTo(Koordinata k, Tabla& t);
    void jede(Koordinata k, Tabla& t);
    void namestiKoordinatu(Koordinata k);

    std::string vratiSlovo() const { return slovo; }
    char vratiBoju() const { return boja; }
    Koordinata vratiKordinatu() const { return k; }
};


class Top : public Figura
{
public:
    Top(Koordinata k, char boja, std::string slovo);
    bool CanMoveTo(Koordinata k, Tabla& t) override;
};

class Lovac : public Figura
{
public:
    Lovac(Koordinata k, char boja, std::string slovo);
    bool CanMoveTo(Koordinata k, Tabla& t) override;
};

class Konj : public Figura
{
public:
    Konj(Koordinata k, char boja, std::string slovo);
    bool CanMoveTo(Koordinata k, Tabla& t) override;
};

class Pijun : public Figura
{
public:
    Pijun(Koordinata k, char boja, std::string slovo);
    bool CanMoveTo(Koordinata k, Tabla& t) override;
};

class Kralj : public Figura
{
public:
    Kralj(Koordinata k, char boja, std::string slovo);
    bool CanMoveTo(Koordinata k, Tabla& t) override;
};

class Kraljica : public Figura
{
public:
    Kraljica(Koordinata k, char boja, std::string slovo);
    bool CanMoveTo(Koordinata k, Tabla& t) override;
};


class Tabla
{
protected:
    std::vector<Figura*> figure;
public:
    Tabla();
    ~Tabla();

    void ubaciFiguru(Figura* f);
    Figura* vratiFiguruIme(std::string ime);

    bool sah(char boja, Tabla& t);
    bool sahmat(char boja);

    void ispis() const;

    const std::vector<Figura*>& vratifigure() const { return figure; }

    const Figura* FiguraNaPolju(Koordinata k) const;
    void UkloniFiguruNa(Koordinata k);
};

