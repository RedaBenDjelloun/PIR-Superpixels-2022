#pragma once

class Partition{
    int w;
    int h;
    int* s = new int[w*h];
public:
    Partition();
    Partition(int K); //
    ~Partition();
};
class Point{
public:
    int i;
    int j;
};
bool voisin(int i, int j, Point P);
Point Coord(int k, int w);
