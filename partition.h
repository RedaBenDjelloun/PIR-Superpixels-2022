#pragma once
#include <cmath>
using namespace std;

#include <Imagine/Images.h>
using namespace Imagine;

/// *** Partition ***

// Classe

class Partition{
    int w;
    int h;
    int Kw;
    int Kh;
    int* s;
public:
    // Constructeur
    Partition();
    Partition(int w_, int h_, int Kw, int Kh); // Parition des pixels en grille de dimensions Kw * Kh

    // Destructeur
    ~Partition();

    // Accesseurs
    int getw();
    int geth();
    int get(int i, int j);
    void set(int i, int j, int k);

    //Fonctions
    int Nw(); // Nombre de pixels horizontaux par case de la grille
    int Nh(); // Nombre de pixels verticaux case de la grille
    void draw();
    void print(int xk, int yk);
    bool testGrille();

};

// Fonctions

/// *** Point ***

// Classe

class Point{
public:
    int i;
    int j;
};

// Fonctions

bool voisin(int i, int j, Point P);
Point Coord(int k, int w);
