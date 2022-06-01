#include <iostream>
#include <vector>
using namespace std;

#include "partition.h"
#include "energie.h"

//******************** Fonctions d'initialisation ********************
/// Change la graine du module random de la std
void InitRandom(){
    srand((unsigned int) time(0));
}
/// Renvoie l'image "name" du dossier source
Image<Color> loadImage(const char* name, int &w, int &h){
    // Image initiale
    Image<Color> I_;
    if(! load(I_, stringSrcPath(name))) {
        std::cout << "WARNING : Echec de lecture d'image" << std::endl;
    }
    // Dimensions initiales
    int w_ = I_.width();
    int h_ = I_.height();
    assert(w > 0 and h > 0);
    // Dimensions après réduction
    w = w_ - w_%Kw;
    h = h_ - h_%Kh;
    // Image après réduction
    Image<Color> I;
    I = I_.getSubImage(0,0,w,h);
    cout <<"Dimensions de l'image initiale : "<<w_<<" x "<<h_<<endl;
    cout <<"Dimensions de l'image réduite : "<<w<<" x "<<h<<endl;
    cout<<endl;
    return I;
}

//******************** Fonctions de test ********************
/// Test pour le transfert d'un bloc
void testTranferBlocks(Partition &P){
    cout <<"Test du transfert d'un bloc"<<endl;
    P.transferBlock(P.Nw() - R,P.Nh() - R,2*R,2*R,1);
    clearWindow();
    P.display_I();
    P.draw();
    cout<<endl;
}
/// Test pour le calcul du terme de couleurs : affichage de l'histogramme du superpixel 0
void testH(Partition &P, int k){
    cout <<"Test du terme de couleurs de l'énergie"<<endl;
    cout<<"Histogramme du superpixel "<<k<<endl;
    P.draw_c(k);
    cout<<"Terme de couleur = "<<H(P)<<endl;
    cout<<endl;
}
/// Test pour le calcul du terme de frontières : affichage de l'histogramme du pixel (x, y)
void testG(Partition &P, int x, int y){
    cout <<"Test du terme de frontière de l'énergie"<<endl;

    cout<<"Histogramme du pixel ("<<x<<","<<y<<")"<<endl;
    P.draw_b(x,y);
    cout<<"Terme de frontière = "<<G(P)<<endl;
    cout<<endl;
}
/// Test de recherche de frontières
void testFrontiere(Partition &P, int NbIter){
    cout <<"Test de la recherche de frontières"<<endl;
    Point p0 = Point(0,0);
    Point pf = Point(0,0);
    for (int i = 0; i < NbIter; i++){
        p0 = Point(rand()%P.getw(),rand()%P.geth());
        cout<<"Point de départ : pixel ("<<p0.x<<","<<p0.y<<")"<<endl;
        pf = P.rechercheFrontiereAffiche(p0);
        cout<<"Point d'arrivée : pixel ("<<pf.x<<","<<pf.y<<")"<<endl;
        cout<<"Appartenance à la frontière : "<<P.appartientFrontiere(pf)<<endl;
    }
    cout<<endl;
}
void testFrontiereRapide(Partition &P, int NbIter){
    cout <<"Test de la recherche de frontières"<<endl;
    Point p0 = Point(0,0);
    Point pf = Point(0,0);
    for (int i = 0; i < NbIter; i++){
        p0 = Point(rand()%P.getw(),rand()%P.geth());
        cout<<"Point de départ : pixel ("<<p0.x<<","<<p0.y<<")"<<endl;
        pf = P.rechercheFrontiereAffiche(p0);
        cout<<"Point d'arrivée : pixel ("<<pf.x<<","<<pf.y<<")"<<endl;
        cout<<"Appartenance à la frontière : "<<P.appartientFrontiere(pf)<<endl;
    }
    cout<<endl;
}
/// Segmentation aléatoire de l'image pour tester la rapidité des fonctions autres que l'énergie
void testRandomSegmentation(Partition &P, int NbIter){
    cout <<"Test de segmentation aléatoire"<<endl;
    Point p0 = Point(0,0);
    Point pf = Point(0,0);
    int k_pf = 0;
    for (int i = 0; i < NbIter; i++){
        p0 = Point(rand()%P.getw(),rand()%P.geth());
        pf = P.rechercheFrontiere(p0);
        k_pf = P.get_s(pf.x,pf.y);
        P.transferBlock(pf.x - R/2,pf.y-R/2,R,R,k_pf);
    }
    clearWindow();
    P.display_I();
    P.draw();
    cout <<endl;
}
//******************** Fonction Hill-Climbing ********************

void hillClimbing(Partition &P){
    int w = P.getw();
    int h = P.geth();
    Point p0 = Point(0,0);
    Point pf = Point(0,0);
    int k_pf = 0;
    int old_s[R*R];
    int admission = 0;
    for (int i = 0; i < 10000; i++){
        p0 = Point(rand()%P.getw(),rand()%P.geth());
        pf = P.rechercheFrontiere(p0);
        k_pf = P.get_s(pf.x,pf.y);
        if(cTB(P, H(P), G(P), max(0,pf.x - R/2),max(0,pf.y-R/2),min(abs(pf.x-w),R),min(abs(pf.y-h),R),k_pf, old_s)){
            clearWindow();
            P.display_I();
            P.draw();
            admission++;
            cout <<"Itération n° "<<i<<endl;
            cout <<"Taux d'admission : "<<float(admission)/i<<endl;
            cout<<"Terme de couleur = "<<H(P)<<endl;
            cout<<"Terme de frontière = "<<G(P)<<endl;
            cout<<endl;
        }
    }
}

//******************** Fonction Main ********************

int main() {
    InitRandom();

    //******************** Initialisation Image ********************
    int w,h;
    const char* name = "starfish.jpg";
    Image<Color> I = loadImage(name, w, h);

    //******************** Initialisation Partition ********************
    Partition P(I);
    P.remplir_c();
    P.remplir_b();

    //******************** Initialisation Affichage ********************
    Window W1 = openWindow(w, h);
    display(I);
    P.draw();

    //******************** Tests ********************
//    P.testGrille();

//    testTranferBlocks(P);

//    int k = 0;
//    testH(P,k);
//    int x=P.Nw(), y=P.Nh();
//    testG(P,x,y);

    setActiveWindow(W1);
    int NbIter = 1000;
    testFrontiere(P,NbIter);
//    testFrontiereRapide(P,NbIter);
//    testRandomSegmentation(P,NbIter);

    // ******************** Hill-Climbing Optimization ********************

//    hillClimbing(P);
    
    // ******************** Fin ********************
    endGraphics();
    return 0;
}
