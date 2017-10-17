# include "property.h"

typedef struct Perceptron Perceptron;

#ifndef PERCEP
#define PERCEP
struct Perceptron {
    double weights[NBWEIGHT];
};
#endif

void guess(double input[],double weights[],size_t n, size_t m, size_t p, double res[]);
void training(double answer[], double w1[], double w2[], double guess2[], double guess1[]);
