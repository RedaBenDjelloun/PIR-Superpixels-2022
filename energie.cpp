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
        P.calcul_Zc(k);
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
            P.calcul_Zb(x,y);
            if(P.get_Zb(x,y)!=0)
                G+=Gi/(P.get_Zb(x,y)*P.get_Zb(x,y));
        }
    return G/(P.getw()*P.geth());
}
/// **** Hill Climbing ****

bool compareTransfertBlock(Partition &P, double H_ini, double G_ini,int x1, int y1, int wb, int hb, int k){
    //patch de sauvegarde
    int s[wb*hb];
    for (int x = x1; x < min(x1+wb,P.getw()); x++){
        for (int y = y1; y < min(y1 + hb,P.geth()); y++){
            s[(x-x1)+(y-y1)*wb]=P.get_s(x,y);
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
    if(H_fin+G_fin<H_ini+G_ini)
        return true;
    //rétablissemeent de P
    for (int x = x1; x < min(x1+wb,P.getw()); x++){
        for (int y = y1; y < min(y1 + hb,P.geth()); y++){
            P.set_s(x,y,s[(x-x1)+(y-y1)*wb]);
        }
    }
    //mise à jour de c et b
    P.remplir_c();
    P.remplir_b();
    //mise à jour de Zc et Zb
    for(int k=0;k<K;k++)
        P.calcul_Zc(k);
    for(int x=0;x<P.getw();x++)
        for(int y=0;y<P.geth();y++)
            P.calcul_Zb(x,y);
    return false;

}
