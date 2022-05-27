#pragma once
#include <cmath>
using namespace std;

#include <Imagine/Images.h>
using namespace Imagine;

/// **** Paramètres
// Taille des boîtes de superpixels
const int R = 24; // Taille recommandée : R = 5 pour 200 superpixels, R*R pixels par bloc

// Taille des patchs pour le terme de frontières de l'énergie
const int Np = 3;

// Nombre de superpixels
const int Kw = 4;
const int Kh = 3;
const int K = Kw*Kh; // Nombre de superpixels recommandé : K = 200 superpixels sur une image 481x321

// Nombre de paniers de couleurs par channel
const int J = 5; // Nombre recommandé : 5 par channel soit 125 paniers au total
const int Nj = 256/J; // Nombre de couleurs par paniers sur un même channel

/// **** Point ****

struct Point{
    int x;
    int y;

    Point();
    Point(int x0,int y0);
    Point operator+ (Point a);
    Point operator- (Point a);
};

void drawPoint(Point p0, Color col);

const Point haut = Point(0,1);
const Point bas = Point(0,-1);
const Point gauche = Point(-1,0);
const Point droite = Point(1,0);

const Point directions[4] = {gauche, bas, droite, haut};

/// **** Partition ****

// Classe

class Partition{
    // Image associée à la partition
    Image<Color> I;
    int w;
    int h;
    // Tableau d'appartenance des pixels à chaque superpixel
    int* s;
    // Histogramme de couleurs (non normalisé) et liste des facteurs de normalisation
    int c[K][J][J][J];
    int Zc[K];
    // Histogramme de superpixels voisins (non normalisé) et liste des facteurs de normalisation
    int* b;
    int* Zb;

public:
    // Constructeur
    Partition();
    Partition(Image<Color> I_); // Parition des pixels en grille de dimensions Kw * Kh

    // Destructeur
    ~Partition();

    // Accesseurs
    int getw();
    int geth();

    Color get_I(int x, int y);
    int get_Ir(int x, int y);
    int get_Ig(int x, int y);
    int get_Ib(int x, int y);

    int get_s(int x, int y);
    void set_s(int x, int y, int k);

    int get_c(int k, int r, int g, int b);
    void set_c(int k, int r, int g, int b, int valeur);
    void incr_c(int k, int r, int g, int b, int increment);
    int get_Zc(int k);
    void calcul_Zc(int k);

    int get_b(int k,int x, int y);
    void set_b(int k, int x, int y, int valeur);
    void incr_b(int k, int x, int y, int increment);
    int get_Zb(int x, int y);
    void calcul_Zb(int x, int y);

    //Fonctions
    int Nw(); // Nombre de pixels horizontaux par case de la grille
    int Nh(); // Nombre de pixels verticaux case de la grille
    void draw();
    void print_s(int xk, int yk);
    bool testGrille();
    void transferBlock(int x1, int y1, int wb, int hb, int k); // Transférer les pixels dans le bloc sélectionné au superpixel k

    void remplir_c();
    void remplir_b();
    void print_c();
    void draw_c(int k);
    void draw_b(int x, int y);

    bool appartientImage(Point p);
    bool appartientFrontiere(Point p);
    Point rechercheFrontiere(Point p0);

    bool connexe(int k);
};

