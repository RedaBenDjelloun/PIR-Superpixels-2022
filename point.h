#pragma once

#include <Imagine/Images.h>
using namespace Imagine;


struct Point{
    int x;
    int y;

    Point();
    Point(int x0,int y0);
    Point operator+ (Point a);
    Point operator- (Point a);
    Point operator* (double lambda);
};
Point operator*(double lambda, Point p);
Point operator*(Point p, int facteur);
Point operator*(int facteur, Point p);

const Point haut = Point(0,1);
const Point bas = Point(0,-1);
const Point gauche = Point(-1,0);
const Point droite = Point(1,0);

const Point haut_gauche = Point(-1,1);
const Point haut_droite = Point(1,1);
const Point bas_gauche = Point(-1,-1);
const Point bas_droite = Point(1,-1);

const Point directions[4] = {gauche, bas, droite, haut};
const Point voisins[8] = {gauche, bas_gauche, bas, bas_droite, droite, haut_droite, haut, haut_gauche};

void drawPoint(Point p0, Color col);
