# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>


# include "activation.h"
# include "neuraloutput.h"

# define NBWEIGHTOUT 4


void initOutput (NeuralOutput *no) {
    for (size_t i = 0; i < NBWEIGHTOUT; i++) {
        no->weights[i] = rand()/(double)RAND_MAX;
    }
}

double guessOutput(double input[], NeuralOutput *no) {
    double sum = 0;
    for (size_t i = 0; i < NBWEIGHTOUT; i++) {
        sum += input[i] * no->weights[i];
    }
    return sigmoid(sum);
}
