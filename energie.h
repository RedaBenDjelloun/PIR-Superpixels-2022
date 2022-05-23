#pragma once

#include "partition.h"

// Poids des termes d'énergie
const float gam = 1;
// Nombre maximal d'itérations
const int t_stop=100;

/// **** Terme de couleur ****
double H(Partition &P);

/// **** Terme de frontières ****
double G(Partition &P);

/// **** Hill Climbing ****

/// **** Validation d'un transfert de bloc ****
bool compareTransferBlock(Partition &P, double H_ini, double G_ini,int x1, int y1, int wb, int hb, int k);

