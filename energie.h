#pragma once

#include "partition.h"

// Nombre maximal d'itérations
const int t_stop=100;

/// **** Terme de couleur ****
double H(Partition &P);

/// **** Terme de frontières ****
double G(Partition &P);

/// **** Hill Climbing ****

/// **** Validation d'un transfert de bloc ****
bool compareTransfertBlock(Partition &P, double H_ini, double G_ini, int x1, int y1, int wb, int hb, int k, int old_s[]);
//si la nouvelle partition est meilleure: renvoie vrai

bool cTB(Partition &P, double H_ini, double G_ini, int x1, int y1, int wb, int hb, int k, int old_s[]);
//si la nouvelle partition est meilleure: renvoie vrai

//s_i : superpixel donneur, s_f : receveur; return true si meilleure partition pour energie H
bool compare_fast_H(Partition &P, int x1, int y1, int wb, int hb, int s_i, int s_f);

bool compare_fast_G(Partition &P, int x1, int y1,int n);
