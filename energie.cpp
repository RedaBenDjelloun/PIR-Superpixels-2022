#include "energie.h"

/// **** Terme de couleurs ****
double H(Partition &P){
    double H=0.;
    for(int k=0;k<K;k++){
        double Hk=0.;
        for(int r=0;r<J;r++)
            for(int g=0;g<J;g++)
                for(int b=0;b<J;b++){
                    int val=P.get_c(k,r,g,b);
                    Hk+=val*val;
                }
        if(P.get_Zc(k)!=0)
            H+=Hk/(P.get_Zc(k)*P.get_Zc(k));
    }
    return H/K;
}

/// **** Terme de frontières ****
double G(Partition &P){
    double G=0;
    for(int x=0;x<P.getw();x++)
        for(int y=0;y<P.geth();y++){
            double Gi=0.;
            for(int k=0;k<K;k++){
                int val=P.get_b(k,x,y);
                Gi+=val*val;
            }
            if(P.get_Zb(x,y)!=0)
                G+=Gi/(P.get_Zb(x,y)*P.get_Zb(x,y));
        }
    return G/(P.getw()*P.geth());
}
/// **** Hill Climbing ****

bool compareTransfertBlock(Partition &P, double H_ini, double G_ini, int x1, int y1, int wb, int hb, int k, int old_s[]){
    //patch de sauvegarde
    for (int x = x1; x < min(x1+wb,P.getw()); x++){
        for (int y = y1; y < min(y1 + hb,P.geth()); y++){
            old_s[(x-x1)+(y-y1)*wb]=P.get_s(x,y);
        }
    }
    //transfert du Bloc
    P.transferBlock(x1,y1,wb,hb,k);//remplace le patch
    //mise à jour de c et b
    P.remplir_c();
    P.remplir_b();
    //calcul de H et G
    double H_fin=H(P);
    double G_fin=G(P);
    if(H_fin+G_fin>H_ini+G_ini)
        return true;
    //rétablissemeent de P
    for (int x = x1; x < min(x1+wb,P.getw()); x++){
        for (int y = y1; y < min(y1 + hb,P.geth()); y++){
            P.set_s(x,y,old_s[(x-x1)+(y-y1)*wb]);
        }
    }
    //mise à jour de c et b
    P.remplir_c();
    P.remplir_b();
    return false;
}

//s_i : superpixel donneur, s_f : receveur; return true si meilleure partition pour energie H
bool compare_fast_H(Partition &P, int x1, int y1, int wb, int hb, int s_i, int s_f){

    //coords des points a deplacer
    list<Point> patch_coords;
    for (int x = x1; x < min(x1+wb,P.getw()); x++){
        for (int y = y1; y < min(y1 + hb,P.geth()); y++){
            if (P.get_s(x,y) == s_i)
                patch_coords.push_back({x,y});
        }
    }
    //nombre de pixels dans le patch mais aussi le facteur de normalisation
    int Z_patch = patch_coords.size();
    Point patch[Z_patch];
    for (int i=0; i<Z_patch; i++){
        patch[i] = patch_coords.front();
        patch_coords.pop_front();
    }

    // histogramme patch
    int c[J][J][J];
    for (int i=0; i<Z_patch; i++){
        int x = patch[i].x;
        int y = patch[i].y;
         // On incrémente de 1 le panier (r,g,b) où tombe le pixel (x,y) du patch
        c[P.get_Ir(x,y)/Nj][P.get_Ig(x,y)/Nj][P.get_Ib(x,y)/Nj] += 1;

    }

    //*****************************
    // Cas du patch de taille 1 : O(1)

    if (Z_patch == 1){
        int x = patch[0].x;
        int y = patch[0].y;
        int r = P.get_Ir(x,y)/Nj;
        int g = P.get_Ig(x,y)/Nj;
        int b = P.get_Ib(x,y)/Nj;

        int int1 = P.get_c(s_f,r,g,b)/P.get_Zc(s_f);
        int int2 = (P.get_c(s_i,r,g,b) - c[r][g][b])/(P.get_Zc(s_i) - Z_patch);

        if(int1 > int2)
            return true;
        else
            return false;

    }
    //****************************

    //patch de taille > 1
    //calcul terme de gauche int()
    double intersection1 = 0;
    for (int r = 0; r < J; r++){
        for (int g = 0; g < J; g++){
            for (int b = 0; b < J; b++){
            intersection1 += min(P.get_c(s_f,r,g,b)/P.get_Zc(s_f), c[r][g][b]/Z_patch);
            }
        }
    }

    //calcul terme de droite int()
    //histogramme superpixel donneur sans le patch
    int c_i[J][J][J];
    for (int r = 0; r < J; r++){
        for (int g = 0; g < J; g++){
            for (int b = 0; b < J; b++){
            c_i[r][g][b] = P.get_c(s_i,r,g,b) - c[r][g][b];
            }
        }
    }
    //nouvelle normalisation associee
    int Z_i = P.get_Zc(s_i) - Z_patch;

    double intersection2 = 0;
    for (int r = 0; r < J; r++){
        for (int g = 0; g < J; g++){
            for (int b = 0; b < J; b++){
            intersection2 += min(c_i[r][g][b]/Z_i, c[r][g][b]/Z_patch);
            }
        }
    }

    //test final
    if (intersection1 > intersection2){
        return true;
    }
    return false;
}
// compare transfert bloc
bool cTB(Partition &P, double H_ini, double G_ini, int x1, int y1, int wb, int hb, int k, int old_s[]){
    int w = P.getw(),h = P.geth() ;
    int r,g,b;
    bool Kconcerned[K];//on optimise connexe en appliquant la fonction qu'au superpixels modifiées
    ///initialisation de tableaux pour taille et connexe
    for(int l=0; l<K; l++){
        Kconcerned[l]=false;
    }
    Kconcerned[k]=true;
    /// On encadre la taille
    for (int x = x1; x < min(x1+wb,w); x++){
        for (int y = y1; y < min(y1 + hb,h); y++){
            int n=P.get_s(x,y);
            if(!(Kconcerned[n]))
                Kconcerned[n]=true;
        }
    }
    /// on modifie le tableau s et c
    for (int x = x1; x < min(x1+wb,w); x++){
        for (int y = y1; y < min(y1 + hb,h); y++){
            old_s[(x-x1)+(y-y1)*wb]=P.get_s(x,y);
            r = P.get_Ir(x,y)/Nj; // entier entre 0 et J-1 correspondant à l'indice r du panier
            g = P.get_Ig(x,y)/Nj;
            b = P.get_Ib(x,y)/Nj;
            P.incr_c(P.get_s(x,y),r,g,b,-1);
            P.incr_c(k,r,g,b,1);
            P.set_s(x,y,k);
        }
    }
    /// on update le tableau b
    for (int x = x1; x < min(x1+wb,P.getw()); x++){
        for (int y = y1; y < min(y1 + hb,P.geth()); y++){
            for (int n=0;n<K;n++)// On réinitialise;
                P.set_b(n,x,y,0);
            for (int i=max(0,x-Np/2);i<=min(w-1,x+Np/2);i++){
                for (int j=max(0,y-Np/2);j<=min(h-1,y+Np/2);j++){
                    P.incr_b(P.get_s(i,j),x,y,1);
                }
            }
        }
    }
    /// on regarde si les superpixels modifiés sont connexes
    bool conn = true;
    for(int l=0; l<K and Kconcerned[l]; l++){
        if(P.connexe(l)) conn = true;
        else{
            conn = false;
            break;
        }
    }
    //calcul de H et G
    double H_fin=H(P);
    double G_fin=G(P);
    if(H_fin+G_fin>H_ini+G_ini and conn)
        return true;
    //rétablissemeent de P et c
    for (int x = x1; x < min(x1+wb,P.getw()); x++){
        for (int y = y1; y < min(y1 + hb,P.geth()); y++){
            r = P.get_Ir(x,y)/Nj;
            g = P.get_Ig(x,y)/Nj;
            b = P.get_Ib(x,y)/Nj;
            P.incr_c(k,r,g,b,-1);
            P.incr_c(old_s[(x-x1)+(y-y1)*wb],r,g,b,1);
            P.set_s(x,y,old_s[(x-x1)+(y-y1)*wb]);
        }
    }
    //rétablissement de b
    for (int x = x1; x < min(x1+wb,P.getw()); x++){
        for (int y = y1; y < min(y1 + hb,P.geth()); y++){
            for (int n=0;n<K;n++)// On réinitialise;
                P.set_b(n,x,y,0);
            for (int i=max(0,x-Np/2);i<=min(w-1,x+Np/2);i++){
                for (int j=max(0,y-Np/2);j<=min(h-1,y+Np/2);j++){
                    P.incr_b(P.get_s(i,j),x,y,1);
                }
            }
        }
    }
    //mise à jour de Zc et Zb
    for(int k=0;k<K;k++)
        P.calcul_Zc(k);
    for(int x=0;x<P.getw();x++)
        for(int y=0;y<P.geth();y++)
            P.calcul_Zb(x,y);
    return false;
}

bool compare_fast_G(Partition &P, int x1, int y1,int n){
    int k=P.get_s(x1,y1);
    double Hk=0;
    double Hn=0;
    for (int i=max(0,x1-Np/2);i<=min(P.getw()-1,x1+Np/2);i++){
        for (int j=max(0,y1-Np/2);j<=min(P.geth()-1,y1+Np/2);j++){
            int Z=P.get_Zb(i,j);
            Hk+=P.get_b(k,i,j)/Z;
            Hn+=P.get_b(n,i,j)/Z+1;
        }
    }
    if(Hn>Hk)// est-ce que le nouveau G est meilleur?
        return true;
    return false;
}
