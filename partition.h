#pragma once
#include <cmath>
using namespace std;

#include "point.h"

// ******************** Paramètres ********************
/// Taille des blocs de tranfert d'un superpixel à l'autre : R*R
const int R = 24; // Taille recommandée : R = 5 pour 200 superpixels, R*R pixels par bloc
/// Taille des patchs pour le terme de frontière de l'énergie
const int Np = 3; // Taille recommandée : Np = 3
/// Nombre de superpixels sur une ligne
const int Kw = 4;
/// Nombre de superpixels sur une colonne
const int Kh = 4;
/// Nombre total de superpixels de la partition
const int K = Kw*Kh; // Nombre de superpixels recommandé : K = 200 superpixels sur une image 481x321
/// Nombre de paniers de couleurs sur chacun des channels R,G,B
const int J = 5; // Nombre recommandé : 5 par channel soit 125 paniers au total
/// Nombre de couleurs par panier de couleur sur un même channel R,G ou B
const int Nj = 256/J; // Nombre de couleurs par paniers sur un même channel
/// Poids des termes d'énergie
const double gam = 1;

// ******************** Classe ********************
class Partition{
    /// Image associée à la partition
    Image<Color> I;
    /// Largeur de l'image I
    int w;
    /// Hauteur de l'image I
    int h;
    /// Tableau d'appartenance des pixels à chaque superpixel (contient des entiers entre 0 et K-1)
    int* s;
    /// Tableau des K histogrammes des couleurs non normalisés (contient des entiers entre 0 et w*h)
    int c[K][J][J][J];
    /// Tableau des facteurs de normalisation des K histogrammes de couleurs
    int Zc[K];
    /// Tableau des w*h histogrammes de superpixels voisins non normalisés (contient des entiers entre 0 et K-1)
    int* b;
    /// Tableau des facteurs de normalisation des w*h histogrammes de superpixels voisins
    int* Zb;
    /// Terme de couleur de l'énergie
    double H;
    /// Terme de frontière de l'énergie
    double G;
public:
    //******************** Constructeurs, Destructeurs ********************
    /// Constructeur vide
    Partition();
    /// Constructeur par défaut : partition des pixels de l'image en une grille de Kw * Kh cases carrées
    Partition(Image<Color> I_);
    /// Destructeur par défaut
    ~Partition();
    //******************** Image I ********************
    /// Accesseur pour la largeur de I
    int getw();
    /// Accesseur pour la hauteur de I
    int geth();
    /// Accesseur pour la couleur RGB du pixel (x,y) de I
    Color get_I(int x, int y);
    /// Accesseur pour la valeur du channel R du pixel (x,y) de I
    int get_Ir(int x, int y);
    /// Accesseur pour la valeur du channel G du pixel (x,y) de I
    int get_Ig(int x, int y);
    /// Accesseur pour la valeur du channel B du pixel (x,y) de I
    int get_Ib(int x, int y);
    /// Affichage de l'image I
    void display_I();
    //******************** Tableau s ********************
    /// Accesseur pour le superpixel parent du pixel (x,y)
    int get_s(int x, int y);
    /// Mutateur qui définit k comme superpixel parent du pixel (x,y)
    void set_s(int x, int y, int k);
    /// Affiche dans le terminal les superpixels parents des pixels contenus dans le carré (xk, yk, xk+Nw(), yk+Nh())
    void print_s(int xk, int yk);
    //******************** Tableau c ********************
    /// Accesseur pour le panier (r,g,b) de l'histogramme du superpixel k
    int get_c(int k, int r, int g, int b);
    /// Mutateur qui définit 'valeur' comme panier (r,g,b) de l'histogramme du superpixel k
    void set_c(int k, int r, int g, int b, int valeur);
    /// Mutateur qui incrémente de 'increment' le panier (r,g,b) du superpixel k
    void incr_c(int k, int r, int g, int b, int increment);
    /// Accesseur pour la valeur du facteur de normalisation de l'histogramme du superpixel k
    int get_Zc(int k);
    /// Met à jour le facteur de normalisation de l'histogramme du superpixel k dans le tableau Zc
    void calcul_Zc(int k);
    /// Met à jour les valeurs des K histogrammes de couleurs
    void remplir_c();
    /// Affiche dans le terminal les valeurs des K histogrammes de couleurs
    void print_c();
    /// Affiche dans une autre fenêtre l'histogramme de couleurs du superpixel k
    void draw_c(int k);
    //******************** Tableau b ********************
    /// Accesseur pour le panier k de l'histogramme du pixel (x,y)
    int get_b(int k,int x, int y);
    /// Mutateur qui définit 'valeur' comme panier k de l'histogramme du pixel (x,y)
    void set_b(int k, int x, int y, int valeur);
    /// Mutateur qui incrémente de 'increment' le panier k de l'histogramme du pixel (x,y)
    void incr_b(int k, int x, int y, int increment);
    /// Accesseur pour la valeur du facteur de normalisation de l'histogramme du pixel (x,y)
    int get_Zb(int x, int y);
    /// Met à jour le facteur de normalisation de l'histogramme du pixel (x,y) dans le tableau Zb
    void calcul_Zb(int x, int y);
    /// Met à jour les valeurs des w*h histogrammes de frontières
    void remplir_b();
    /// Affiche dans une autre fenêtre l'histogrammee de superpixels voisins du pixel (x,y)
    void draw_b(int x, int y);
    //******************** Energie ********************
    /// Accesseur pour le terme de couleur s
    double get_H();
    /// Calcule H depuis zéro
    void initialize_H();
    /// Accesseur pour le terme de frontières
    double get_G();
    /// Calcule G depuis zéro
    void initialize_G();
    /// Calcul de l'énergie totale
    double E();
    //******************** Fonctions utiles ********************
    /// Renvoie le nombre de pixels horizontaux par case la grille initiale
    int Nw();
    /// Renvoie le nombre de pixels verticaux par case la grille initiale
    int Nh();
    /// Trace les frontières des superpixels
    void draw();
    /// Teste si la parition est dans la configuration initiale (grille de Kw*Kh superpixels carrés)
    bool testGrille();
    /// Transfère tous les pixels du bloc (x1, y1, x1+wb, y1+hb) au superpixel k
    void transferBlock(int x1, int y1, int wb, int hb, int k);
    /// Teste si le pixel p est dans les limites de l'image (0,0,w-1,h-1)
    bool appartientImage(Point p);
    /// Teste si le pixel p est à la frontière de son superpixel
    bool appartientFrontiere(Point p);
    /// Renvoie le pixel pf le plus proche en norme 1 du pixel p0
    Point rechercheFrontiere(Point p0);
    /// Renvoie le pixel pf le plus proche en croix du pixel p0 en affichant les points intermédiaires
    Point rechercheFrontiereAffiche(Point p0); // on sépare de la fonction sans affichage pour ne pas faire le test d'un bool "affiche" à chaque passage
    /// Teste si le superpixel k est connexe
    bool connexe(int k);
};

