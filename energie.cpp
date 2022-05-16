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
    return H;
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
    return G;
}
/// **** Hill Climbing ****

