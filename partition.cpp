#include "partition.h"

/// **** Partition ****

// Constructeur

Partition::Partition(int w_, int h_, int Kw_, int Kh_){
    w = w_;
    h = h_;
    Kw = Kw_;
    Kh = Kh_;
    s = new int[w*h];
    for (int i = 0; i < w; i++){
        for (int j = 0; j < h; j++){
            set(i, j, 1 + i/Nw() + (j/Nh())*Kw); // s contient des valeurs entre 1 et K
        }
    }
}

// Destructeur
Partition::~Partition(){
    delete[] s;
}

// Accesseur

int Partition::getw(){
    return w;
}
int Partition::geth(){
    return h;
}
int Partition::get(int i, int j){
    return  s[i +j*w];
}
void Partition::set(int i, int j, int k){
    s[i + j*w] = k;
}

// Fonctions

int Partition::Nw(){
    return w/Kw;
}

int Partition::Nh(){
    return h/Kh;
}

void Partition::draw(){
    for (int i = 0; i < w-1; i++){
        for (int j = 0; j < h-1; j ++){
            if (get(i+1, j) != get(i,j)){
                drawPoint(i,j,WHITE);
            }
            if (get(i, j+1) != get(i,j)){
                drawPoint(i,j,WHITE);
            }
        }
    }
}

void Partition::print(int xk, int yk){
    for (int x = xk*Nw(); x < (xk+1)*Nw(); x++){
        for (int y = yk*Nh(); y < (yk+1)*Nh(); y++){
            cout<<get(x,y)<<endl;
        }
    }
}

bool Partition::testGrille(){
    bool pass = true;
    for (int xk = 0; xk < Kw; xk++){
        for (int yk = 0; yk < Kh; yk++){
            for (int x = xk*Nw()+1; x < (xk+1)*Nw(); x++){
                for (int y = yk*Nh()+1; y < (yk+1)*Nh(); y++){
                    if (get(x,y) != get(x-1,y-1)){
                        pass = false;
                        cout <<"Problème en x = "<<x<<" et y = "<<y<<endl;
                    }

                }
            }
        }
    }
    if (pass){
        cout<<"testGrille réussi"<<endl;
    }
    return pass;
}


/// **** Point ****

// Fonctions

Point Coord(int k, int w){
    Point P;
    P.i = k % w;
    P.j = k - P.i*w;
    return P;
}

bool voisin(int i, int j, Point P){
    return (abs(P.i-i)<=1 or abs(P.j-j)<1);
}
