#include "partition.h"

/// **** Partition ****

// Constructeur

Partition::Partition(Image<Color> I_){
    I = I_.clone();
    w = I_.width();
    h = I_.height();
    //Initialisation de s
    s = new int[w*h];
    for (int x = 0; x < w; x++){
        for (int y = 0; y < h; y++){
            set_s(x, y, x/Nw() + (y/Nh())*Kw); // s contient des valeurs entre 0 et K-1, qui numérotent les superpixels
        }
    }
    // Initialisation de c
    for (int k = 0; k < K; k++){
        for (int r = 0; r < J; r++){
            for (int g = 0; g < J; g++){
                for (int b = 0; b < J; b++){
                    set_c(k,r,g,b,0);
                }
            }
        }
    }
    // Initialisation de Zc
    for (int k = 0; k < K; k++){
        Zc[k] = 0;
    }
    //Initialisation de b
    b = new int[K*w*h];
    for (int k = 0; k < K; k++){
        for (int x = 0; x < w; x++){
            for (int y = 0; y < h; y++){
                set_b(k,x,y,0);
            }
        }
    }
    // Initialisation de Zb
    Zb = new int[w*h];
    for (int x = 0; x < w; x++){
        for(int y = 0; y < h; y ++){
            Zb[x+y*w] = 0;
        }
    }
}

// Destructeur
Partition::~Partition(){
    delete[] s;
    delete[] b;
}

// Accesseurs

int Partition::getw(){
    return w;
}
int Partition::geth(){
    return h;
}


Color Partition::get_I(int x, int y){
    return I(x,y);
}
int Partition::get_Ir(int x, int y){
    return int(I(x,y).r());
}
int Partition::get_Ig(int x, int y){
    return int(I(x,y).g());
}
int Partition::get_Ib(int x, int y){
    return int(I(x,y).b());
}


int Partition::get_s(int x, int y){
    return  s[x +y*w];
}
void Partition::set_s(int x, int y, int k){
    s[x + y*w] = k;
}


int Partition::get_c(int k, int r, int g, int b){
    return c[k][r][g][b];
}
void Partition::set_c(int k, int r, int g, int b, int valeur){
    c[k][r][g][b] = valeur;
}
void Partition::incr_c(int k, int r, int g, int b, int increment){
    c[k][r][g][b] += increment;
}
int Partition::get_Zc(int k){
    return Zc[k];
}
void Partition::calcul_Zc(int k){
    int Z = 0;
    for (int r = 0; r < J; r++){
        for (int g = 0; g < J; g++){
            for (int b = 0; b < J; b++){
                 Z += get_c(k,r,g,b);
            }
        }
    }
    Zc[k] = Z;
}


int Partition::get_b(int k, int x, int y){
    return b[x + w*(y + h*k)]; // array 3D aplati
}
void Partition::set_b(int k, int x, int y, int valeur){
    b[x + w*(y + h*k)] = valeur;
}
void Partition::incr_b(int k, int x, int y, int increment){
    b[x + w*(y + h*k)] += increment;
}
int Partition::get_Zb(int x, int y){
    return Zb[x +y*w];
}
void Partition::calcul_Zb(int x, int y){
    int Z = 0;
    for(int k=0;k<K;k++)
        Z += get_b(k,x,y);
    Zb[x +y*w] = Z;
}


// Fonctions

int Partition::Nw(){
    return w/Kw;
}

int Partition::Nh(){
    return h/Kh;
}

void Partition::draw(){
    for (int x = 0; x < w-1; x++){
        for (int y = 0; y < h-1; y++){
            if (appartientFrontiere(x,y)){
                drawPoint(x,y,BLACK);
            }
        }
    }
}

void Partition::print_s(int xk, int yk){
    for (int x = xk*Nw(); x < (xk+1)*Nw(); x++){
        for (int y = yk*Nh(); y < (yk+1)*Nh(); y++){
            cout<<get_s(x,y)<<endl;
        }
    }
}

bool Partition::testGrille(){
    bool pass = true;
    for (int xk = 0; xk < Kw; xk++){
        for (int yk = 0; yk < Kh; yk++){
            for (int x = xk*Nw()+1; x < (xk+1)*Nw(); x++){
                for (int y = yk*Nh()+1; y < (yk+1)*Nh(); y++){
                    if (get_s(x,y) != get_s(x-1,y-1)){
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
    else{
        cout<<"Attention, le tableau s ne contient pas ce qu'il est censé contenir"<<endl;
    }
    return pass;
}

void Partition::transferBlock(int x1, int y1, int wb, int hb, int k){
    for (int x = x1; x < min(x1+wb,w); x++){
        for (int y = y1; y < min(y1 + hb,h); y++){
            set_s(x,y,k);
        }
    }
}

void Partition::remplir_c(){
    int w = getw();
    int h = geth();
    int k, r, g, b;
    for (int x = 0; x < w; x++){
        for (int y = 0; y < h; y++){
            k = get_s(x,y);
            r = get_Ir(x,y)/Nj; // entier entre 0 et J-1 correspondant à l'indice r du panier
            g = get_Ig(x,y)/Nj;
            b = get_Ib(x,y)/Nj;
            incr_c(k,r,g,b,1); // On incrémente de 1 le panier (r,g,b) du superpixel k où tombe le pixel (x,y)
        }
    }
}

void Partition::remplir_b(){
    int w = getw();
    int h = geth();
    int k = 0;
    for (int x = 0; x < w; x++){
        for (int y = 0; y < h; y++){
            for (int i=max(0,x-Np/2);i<=min(w-1,x+Np/2);i++){
                for (int j=max(0,y-Np/2);j<=min(h-1,y+Np/2);j++){
                    k = get_s(i,j);
                    incr_b(k,x,y,1);
                 }
            }
        }
    }
}

void Partition::print_c(){
    for (int k = 0; k < K; k++){
        for (int r = 0; r < J; r++){
            for (int g = 0; g < J; g++){
                for (int b = 0; b < J; b++){
                    cout <<"k = "<<k<<", r = "<<r<<", g = "<<g<<", b = "<<b<<", valeur de c : "<<get_c(k,r,g,b)<<endl;
                }
            }
        }
    }
}

void Partition::draw_c(int k){
    int barWidth = 8;
    int barHeight = h;
    Color barColor = RED;
    Window Histo = openWindow(J*J*J*barWidth,h);
    setActiveWindow(Histo);
    calcul_Zc(k);
    int Z = get_Zc(k);
    cout<<"Zc = "<<Z<<endl;
    for (int r = 0; r < J; r++){
        for (int g = 0; g < J; g++){
            for (int b = 0; b < J; b++){
                barHeight = (get_c(k,r,g,b)*h)/Z;
                barColor = Color((r+1)*Nj/2, (g+1)*Nj/2, (b+1)*Nj/2);
                fillRect(barWidth*(r + J*(g + J*b)),h,barWidth,-barHeight,barColor);
                //fillRect(0,0,100,100,RED);
            }
        }
    }
}
void Partition::draw_b(int x, int y){
    int barWidth = 30;
    int barHeight = h;
    Color barColor = RED;
    Window Histo = openWindow(K*barWidth,h);
    setActiveWindow(Histo);
    calcul_Zb(x,y);
    int Z = get_Zb(x,y);
    cout<<"Zb = "<<Z<<endl;
    for (int k = 0; k < K; k++){
        barHeight = (get_b(k,x,y)*h)/Z;
        fillRect(barWidth*k,h,barWidth,-barHeight,barColor);
    }

}

/// *** Frontières ****

// Indique si le point (x,y) appartient à la frontière de son Superpixel
bool Partition::appartientFrontiere(int x, int y){
    bool frontiere = false;
    // Les pixels à la bordure de l'image sont nécéssairement à la bordure de leur superpixel
    // donc on ne les teste pas
    if (x == 0 or x == w or y == 0 or y == h){ // Frontière
        frontiere = true;
    }
    else if ((get_s(x,y) != get_s(x+1,y))   // fronière à droite
     or (get_s(x,y) != get_s(x-1,y))        // frontière à gauche
     or (get_s(x,y) != get_s(x,y+1))        // frontière en haut
     or (get_s(x,y) != get_s(x,y-1))){      // en bas
        frontiere = true;
    }
    return frontiere;
}

// Trouve la frontière la plus proche du point (x0, y0) et met ses coordonnées dans (xf,yf)
void Partition::rechercheFrontiere(int x0, int y0, int& xf, int& yf){
    int x, y;
}
