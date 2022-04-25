#pragma once

#include "partition.h"

// Poids des termes d'énergie
const float gam = 1;

/// **** Terme de couleur ****
double H(Partition P);

/// **** Terme de frontières ****
double G(Partition P);

/// **** Hill Climbing ****
const int t_stop=100;

