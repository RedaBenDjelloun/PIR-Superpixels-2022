#include "energie.h"

/// **** Terme de couleurs ****
double H(Partition P){
    double H=0;
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
    return H;
}
/// **** Terme de frontières ****
double G(Partition P){}
/// **** Hill Climbing ****

