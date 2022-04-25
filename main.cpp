#include <iostream>
#include <vector>
using namespace std;

#include "partition.h"
#include "energie.h"

/// **** Paramètres ****

// Taille des boîtes de superpixels
const int R = 24; // Taille recommandée : R = 5 pixels par bloc

// Taille des patchs pour le terme de frontières de l'énergie
const int Np = 3;

/// **** Main ****

int main() {
    /// *** Initialisation
    //Image initiale
    Image<Color> I_;
    if(! load(I_, argc>1? argv[1]: srcPath("starfish.jpg"))) {
        std::cout << "Echec de lecture d'image" << std::endl;
        return 1;
    }
    // Dimensions initiales
    int w_ = I_.width();
    int h_ = I_.height();

    // Dimensions après réduction
    int w = w_ - w_%Kw;
    int h = h_ - h_%Kh;

    // Image après réduction
    Image<Color> I;
    I = I_.getSubImage(0,0,w,h);
    cout <<"Dimensions initiales de l'image : "<<w_<<" x "<<h_<<endl;
    cout <<"Dimensions de l'image réduite : "<<w<<" x "<<h<<endl;

    openWindow(w, h);
    display(I);

    /// *** Partition selon la grille initiale

    Partition P(I);
    P.draw();

    //P.print_s(0,0);
    P.testGrille();
    //click();

    /// **** Transfert de blocs ****

//    P.transferBlock(P.Nw() - R,P.Nh() - R,2*R,2*R,1);
//    clearWindow();
//    display(I);
//    P.draw();
//    click();

    /// **** Terme de couleurs
    P.remplir_c();
    //P.print_c();
    int k = 0;
    P.draw_c(k);
    cout<<"Histogramme du superpixel "<<k<<endl;



    /// **** Fin ****
    endGraphics();
    return 0;
}
