#include "point.h"

Point::Point(){
    x = 0;
    y = 0;
}

Point::Point(int x0,int y0){
    x = x0;
    y = y0;
}

Point Point::operator+(Point a){
    return Point(x + a.x, y + a.y);
}

Point Point::operator-(Point a){
    return Point(x - a.x, y - a.y);
}

Point Point::operator* (double lambda){
    return Point(int(lambda*x), int(lambda*y));
}
Point operator*(double lambda, Point p){
    return p*lambda;
}
Point operator*(Point p, int facteur){
    return p*double(facteur);
}
Point operator*(int facteur, Point p){
    return p*facteur;
}

void drawPoint(Point p, Color col){
    drawPoint(p.x,p.y,col);
}

//***** PointOriente ******

PointOriente::PointOriente(Point p0, int orientation0){
    p = p0;
    orientation = orientation0;
}
