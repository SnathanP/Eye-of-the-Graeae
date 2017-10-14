# include "property.h"

typedef struct Perceptron Perceptron;

#ifndef PERCEP
#define PERCEP
struct Perceptron {
    double weights[NBWEIGHT];
};
#endif

void initPerceptron (Perceptron *perc);
double guess(double input[], Perceptron *perc);
void training(double dS, double ownResult, double outputW, Perceptron *perc);
