#include <iostream>
#include <vector>
using namespace std;

#include <Imagine/Images.h>
using namespace Imagine;

const int K = 12;

// Partition des 16 millions de couleurs en un nombre restreint de couleurs
void PartitionCouleurs(int &r, int &g, int &b){
}

void drawSuperpixels(){

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
