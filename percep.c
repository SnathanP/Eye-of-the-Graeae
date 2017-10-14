# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>


# include "activation.h"
# include "percep.h"

# define NBWEIGHT 2




void initPerceptron (Perceptron *perc) {
    for (size_t i = 0; i < NBWEIGHT; i++) {
        perc->weights[i] = rand()/(double)RAND_MAX;
    }

}

double guess(double input[], Perceptron *perc) {
    double sum = 0;
    for (size_t i = 0; i < NBWEIGHT; i++) {
        sum += input[i] * perc->weights[i];
    }
    /*
    double output;
    if (sum > 0.5) {
        output = 1;
    } else  {
        output = 0;
    }
    */

    return sigmoid(sum);
}

void training(double dS, double ownResult, double outputW, Perceptron *perc) {
    for (size_t i = 0; i < NBWEIGHT; i++) {
        perc->weights[i] += (dS / outputW) * sigmoid_prime(ownResult);
    }
}
