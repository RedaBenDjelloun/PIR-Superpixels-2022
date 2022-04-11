#include <iostream>
#include <vector>
#include "partition.h"
using namespace std;

#include <Imagine/Images.h>
using namespace Imagine;


const int K = 12;
const int Ncol = 12;
// Partition des 16 millions de couleurs en un nombre restreint de couleurs
void PartitionCouleurs(int &r, int &g, int &b){
    int pas=256/Ncol;
    r = r%Ncol, g = g%Ncol, b = b%Ncol;
    r = (2*r+1)*pas/2;
    g = (2*g+1)*pas/2;
    b = (2*b+1)*pas/2;
}

void drawSuperpixels(){

}
Point Coord(int k, int w){
    Point P;
    P.i = k % w;
    P.j = k - P.i*w;
    return P;
}
bool voisin(int i, int j, Point P){
    return (abs(P.i-i)<=1 or abs(P.j-j)<1);
}
int main() {
    Image<Color> I;
    if(! load(I, argc>1? argv[1]: srcPath("starfish.jpg"))) {
        std::cout << "Echec de lecture d'image" << std::endl;
        return 1;
    }
    int w = I.width();
    int h = I.height();
    cout <<w<<" x "<<h<<endl;

    openWindow(w, h);
    display(I);
    endGraphics();
    return 0;
}
