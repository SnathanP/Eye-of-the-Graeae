# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>


# include "activation.h"
# include "neuraloutput.h"
# include "percep.h"


void initOutput (NeuralOutput *no) {
    for (size_t i = 0; i < NBWEIGHTOUT; i++) {
        no->weights[i] = rand()/(double)RAND_MAX;
        printf("%lf : weights\n", no->weights[i]);
    }
}

double guessOutput(double input[], NeuralOutput *no) {
    double sum = 0;
    for (size_t i = 0; i < NBWEIGHTOUT; i++) {
        sum += input[i] * no->weights[i];
    }
    //printf("%lf : sum\n", sum);
    return sigmoid(sum);
}

void guessOutLearn(double input[], double target, NeuralOutput *no, Perceptron perceps[]) {
    double output = guessOutput(input, no);
    double dS = (target - output) * sigmoid_prime(output);
    for (size_t i = 0; i < NBWEIGHTOUT; i++) {
        training(dS, input[i], no->weights[i], &perceps[i]);
        no->weights[i] += (dS/input[i]) * 0.05; // LearningRate
    }
}
