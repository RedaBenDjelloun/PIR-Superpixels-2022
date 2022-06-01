#include "partition.h"
using namespace std;

// ******************** Classe ********************
//******************** Constructeurs, Destructeurs ********************
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
    // Energie
    H = 0;
    G= 0;
}
Partition::~Partition(){
    delete[] s;
    delete[] b;
}
//******************** Image I ********************
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
void Partition::display_I(){
    display(I);
}
//******************** Tableau s ********************
int Partition::get_s(int x, int y){
    return  s[x +y*w];
}
void Partition::set_s(int x, int y, int k){
    s[x + y*w] = k;
}
void Partition::print_s(int xk, int yk){
    for (int x = xk*Nw(); x < (xk+1)*Nw(); x++){
        for (int y = yk*Nh(); y < (yk+1)*Nh(); y++){
            cout<<get_s(x,y)<<endl;
        }
    }
}
//******************** Tableau c ********************
int Partition::get_c(int k, int r, int g, int b){
    return c[k][r][g][b];
}
void Partition::set_c(int k, int r, int g, int b, int valeur){
    c[k][r][g][b] = valeur;
}
void Partition::incr_c(int k, int r, int g, int b, int increment){
    c[k][r][g][b] += increment;
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
int Partition::get_Zc(int k){
    calcul_Zc(k);
    return Zc[k];
}
void Partition::remplir_c(){
    for (int k=0; k<K; k++){
            for (int r = 0; r < J; r++){
                for (int g = 0; g < J; g++){
                    for (int b = 0; b < J; b++){
                         set_c(k,r,g,b,0);
                    }
                }
            }
        }
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
//******************** Tableau b ********************
int Partition::get_b(int k, int x, int y){
    return b[x + w*(y + h*k)]; // array 3D aplati
}
void Partition::set_b(int k, int x, int y, int valeur){
    b[x + w*(y + h*k)] = valeur;
}
void Partition::incr_b(int k, int x, int y, int increment){
    b[x + w*(y + h*k)] += increment;
}
void Partition::calcul_Zb(int x, int y){
    int Z = 0;
    if (x == 0 or x == w-1 or y == 0 or y == h-1){
        for(int k=0;k<K;k++)
            Z += get_b(k,x,y);
    }
    else{
        Z = Np*Np;
    }
    Zb[x +y*w] = Z;
}
int Partition::get_Zb(int x, int y){
    calcul_Zb(x,y);
    return Zb[x +y*w];
}
void Partition::remplir_b(){
    int w = getw();
    int h = geth();
    int k = 0;
    for (int x = 0; x < w; x++){
        for (int y = 0; y < h; y++){
            for (int n=0;n<K;n++)// On réinitialise;
                set_b(n,x,y,0);
            for (int i=max(0,x-Np/2);i<=min(w-1,x+Np/2);i++){
                for (int j=max(0,y-Np/2);j<=min(h-1,y+Np/2);j++){
                    k = get_s(i,j);
                    incr_b(k,x,y,1);
                }
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
    int Z = get_Zb(x,y);
    cout<<"Zb = "<<Z<<endl;
    for (int k = 0; k < K; k++){
        barHeight = (get_b(k,x,y)*h)/Z;
        fillRect(barWidth*k,h,barWidth,-barHeight,barColor);
    }

}

//******************** Energie ********************
double Partition::get_H(){
    return H;
}
void Partition::initialize_H(){
    double new_H=0.;
    int Zc = 0;
    int val = 0;
    for(int k=0;k<K;k++){
        double Hk=0.;
        for(int r=0;r<J;r++)
            for(int g=0;g<J;g++)
                for(int b=0;b<J;b++){
                    val=get_c(k,r,g,b);
                    Hk+=val*val;
                }
        Zc = get_Zc(k);
        if(Zc !=0){
            new_H+=Hk/Zc*Zc;
        }
    }
    H = new_H/K;
}
double Partition::get_G(){
    return G;
}
void Partition::initialize_G(){
    double new_G=0;
    int Zb = 0;
    int val = 0;
    for(int x=0;x<getw();x++)
        for(int y=0;y<geth();y++){
            double Gi=0.;
            for(int k=0;k<K;k++){
                val=get_b(k,x,y);
                Gi+=val*val;
            }
            Zb = get_Zb(x,y);
            if(Zb!=0)
                new_G+=Gi/(Zb*Zb);
        }
    G = new_G/(getw()*geth());
}
double Partition::E(){
    return H + gam*G;
}
//******************** Fonctions utiles ********************
int Partition::Nw(){
    return w/Kw;
}
int Partition::Nh(){
    return h/Kh;
}
void Partition::draw(){
    for (int x = 0; x < w-1; x++){
        for (int y = 0; y < h-1; y++){
            if (appartientFrontiere(Point(x,y))){
                drawPoint(x,y,BLACK);
            }
        }
    }
}
bool Partition::testGrille(){
    cout <<"Test de la disposition intiale en grille"<<endl;
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
        cout<<"Réussite"<<endl;
    }
    else{
        cout<<"Attention, le tableau s ne contient pas ce qu'il est censé contenir"<<endl;
    }
    cout<<endl;
    return pass;
}
void Partition::transferBlock(int x1, int y1, int wb, int hb, int k){
    for (int x = x1; x < min(x1+wb,w); x++){
        for (int y = y1; y < min(y1 + hb,h); y++){
            set_s(x,y,k);
        }
    }
}
bool Partition::appartientImage(Point p){
    return (p.x > 0 and p.x < w and p.y > 0  and p.y < h)? true
                                                           : false;
}
bool Partition::appartientFrontiere(Point p){
    bool frontiere = false;
    // Les pixels à la bordure de l'image sont nécéssairement à la bordure de leur superpixel
    // donc on ne les teste pas
    // cette bordure ne nous intéresse pas donc on met false
    if (p.x == 0 or p.x == w or p.y == 0 or p.y == h){ // Frontière
        frontiere = false;
    }
    else {if ((get_s(p.x,p.y) != get_s(p.x+1,p.y))   // fronière à droite
     or (get_s(p.x,p.y) != get_s(p.x-1,p.y))        // frontière à gauche
     or (get_s(p.x,p.y) != get_s(p.x,p.y+1))        // frontière en haut
     or (get_s(p.x,p.y) != get_s(p.x,p.y-1))){      // en bas
        frontiere = true;
    }}
    return frontiere;
}
Point Partition::rechercheFrontiere(Point p0){
    bool t[w][h];
    for(int x=0;x<w;x++)
        for(int y=0;y<h;y++)
            t[x][y]=false;

    list<PointOriente> file;
    PointOriente p0_ = PointOriente(p0,0); // Notation p_ est le point orienté dont le champ point est p
    file.push_back(p0_);
    PointOriente current_p_ = PointOriente(Point(0,0),0);
    PointOriente candidat_ = PointOriente(Point(0,0),0);
    PointOriente pf_ = PointOriente(Point(0,0),0);

    bool trouve = false;
    int compteur = 0;
    while (file.size() > 0 and not trouve){
        current_p_= file.back();
        t[current_p_.p.x][current_p_.p.y] = true;
        file.pop_back();
        if (appartientFrontiere(current_p_.p)){
            trouve = true;
            pf_ = current_p_;
        }
        else{
            for (int i = 0; i < 4; i++){
                candidat_.p = p0 + (1 + compteur/4)*directions[i];
                candidat_.orientation = i;
                compteur+=1;
                if (appartientImage(candidat_.p) and not t[candidat_.p.x][candidat_.p.y]){
                    file.push_front(candidat_);
                    t[candidat_.p.x][candidat_.p.y]=true;
                }
            }
        }
    }
    return pf_.p + directions[pf_.orientation];
}
Point Partition::rechercheFrontiereAffiche(Point p0){
    bool t[w][h];
    for(int x=0;x<w;x++)
        for(int y=0;y<h;y++)
            t[x][y]=false;

    list<PointOriente> file;
    PointOriente p0_ = PointOriente(p0,0); // Notation p_ est le point orienté dont le champ point est p
    file.push_back(p0_);
    PointOriente current_p_ = PointOriente(Point(0,0),0);
    PointOriente candidat_ = PointOriente(Point(0,0),0);
    PointOriente pf_ = PointOriente(Point(0,0),0);

    bool trouve = false;
    int compteur = 0;
    while (file.size() > 0 and not trouve){
        current_p_= file.back();
        t[current_p_.p.x][current_p_.p.y] = true;
        drawPoint(current_p_.p, RED); // seule différence avec la fonction précédente
        // on ne l'intègre pas directement dans la fonction précédente afin de ne pas la ralentir avec des tests de booléen
        file.pop_back();
        if (appartientFrontiere(current_p_.p)){
            trouve = true;
            pf_ = current_p_;
        }
        else{
            for (int i = 0; i < 4; i++){
                candidat_.p = p0 + (1 + compteur/4)*directions[i];
                candidat_.orientation = i;
                compteur+=1;
                if (appartientImage(candidat_.p) and not t[candidat_.p.x][candidat_.p.y]){
                    file.push_front(candidat_);
                    t[candidat_.p.x][candidat_.p.y]=true;
                }
            }
        }
    }
    return pf_.p + directions[pf_.orientation];
}
bool Partition::connexe(int k){
    list<Coords<2>> L;
    bool t[w][h];

    for(int x=0;x<w;x++)
        for(int y=0;y<h;y++)
            t[x][y]=false;

    int x=0, y=0, compteur=0;

    do{
        x=intRandom(0,w);
        y=intRandom(0,h);
    } while(get_s(x,y)!=k);

    Coords<2> p(x,y);
    L.push_back(p);
    while(L.size()!=0){
        p = L.back();
        L.pop_back();

        for (int i=-1;i<2;i+=2){

            if(p[0]+i>=0 and p[0]+i<=w and get_s(p[0]+i,p[1])==k and !t[p[0]+i][p[1]]) {
                t[p[0]+i][p[1]] = true;
                L.push_front(Coords<2> (p[0]+i,p[1]));
                fillRect(p[0]+i,p[1],1,1,RED);
                compteur+=1;
            }

            if(p[1]+i>=0 and p[1]+i<=h and get_s(p[0],p[1]+i)==k and !t[p[0]][p[1]+i]){
                t[p[0]][p[1]+i] = true;
                L.push_front(Coords<2> (p[0],p[1]+i));
                fillRect(p[0],p[1]+i,1,1,RED);
                compteur+=1;
            }

        }
    }

    int nbPixel = 0;
    for(int x=0;x<w;x++)
        for(int y=0;y<h;y++)
            if(get_s(x,y)==k) nbPixel+=1;
    if (nbPixel==compteur) cout<<"OUI"<<endl;
    return (nbPixel==compteur);
}
