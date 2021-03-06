# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
#include "property.h"

double sigmoid(double x) {
    return 1.0/(1.0 + exp(-x));
}

double sigmoid_prime(double x) {
    return sigmoid(x)*(1.0-sigmoid(x));
    //return x;
}

double tanhyp(double x)
{
    return tanh(x);
}

double tanh_prime(double x) {
    return 1.0 - pow(x,2);
}

double applyLearningRate(double x)
{
    return x * LEARNING_RATE;
}
