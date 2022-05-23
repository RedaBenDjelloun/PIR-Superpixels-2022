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

void rechercheFrontiere(int x0, int y0, int& xf, int& yf);


