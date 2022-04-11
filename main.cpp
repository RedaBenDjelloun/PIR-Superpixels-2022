#include <iostream>
#include <vector>
using namespace std;

#include <Imagine/Images.h>
using namespace Imagine;

// Tableau donnant l'appartenance de chaque pixel à un unique superpixel
//int* PartitionPixels  = new int[w*h];

// Partition des 16 millions de couleurs en un nombre restreint de couleurs
void PartitionCouleurs(int &r, int &g, int &b){
}

int main() {
    Image<Color> I;
    if(! load(I, argc>1? argv[1]: srcPath("starfish.jpg"))) {
        std::cout << "Echec de lecture d'image" << std::endl;
        return 1;
    }
    int w = I.width();
    int h = I.height();

    openWindow(w, h);
    display(I);
    endGraphics();
    return 0;
}
