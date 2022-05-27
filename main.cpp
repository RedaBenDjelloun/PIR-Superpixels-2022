#include <iostream>
#include <vector>
using namespace std;

#include "partition.h"
#include "energie.h"

/// **** Paramètres ****

void InitRandom(){
    srand((unsigned int) time(0));
}

/// **** Main ****

int main() {
    /// *** Initialisation
    InitRandom();
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

    Window W1 = openWindow(w, h);
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

//    /// **** Terme de couleurs
//    P.remplir_c();
//    int k = 0;
//    P.draw_c(k);
//    cout<<"Histogramme du superpixel "<<k<<endl;

//    cout<<"Terme de couleur = "<<H(P)<<endl;

//    /// **** Terme de frontière
//    P.remplir_b();
//    int x=P.Nw(), y=P.Nh();
//    P.draw_b(x,y);

//    cout<<"Histogramme du pixel ("<<x<<","<<y<<")"<<endl;

//    cout<<"Terme de frontière = "<<G(P)<<endl;

    /// **** Recherche frontiere ****
    setActiveWindow(W1);
    Point p0 = Point(rand()%P.getw(),rand()%P.geth());
    cout<<"Point de départ : pixel ("<<p0.x<<","<<p0.y<<")"<<endl;
//    drawPoint(p0,BLUE);
//    Point pf = P.rechercheFrontiere(p0);
//    drawPoint(pf,RED);
//    cout<<"Point d'arrivée : pixel ("<<pf.x<<","<<pf.y<<")"<<endl;
//    cout<<"Appartenance à la frontière : "<<P.appartientFrontiere(pf)<<endl;
    P.testVoisins(p0,30);

//    int k_pf = P.get_s(pf.x,pf.y);
//    P.transferBlock(pf.x - R/2,pf.y-R/2,R,R,k_pf);
//    clearWindow();
//    display(I);
//    P.draw();
//    drawPoint(pf,RED);
//    click();

    /// **** Fin ****
    endGraphics();
    return 0;
}
