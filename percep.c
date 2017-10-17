# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>


# include "activation.h"
# include "percep.h"
# include "matrix.h"
# include "algebra.h"
# include "matrix.h"


void guess(double input[],double weights[],size_t n, size_t m, size_t p, double res[])
{
    mul(weights, input, n , m , p, res);
    vector_apply(sigmoid,res,p,res);
}

void training(double answer[], double w1[], double w2[], double guess2[], double guess1[])
{
    double dE[1];
    substract(answer, guess2, 1,1, dE);
    double dS[1];
    vector_apply(tanh_prime, guess2,1,guess2);
    mul(outputSum,errorOutput,1,1,1,dS);
    double res[NBWEIGHTOUT];
    mul(dS,transpose(guess1,NBWEIGHTOUT,1,res),1,1,3,res);
    add(res,w2,1,3);
}

void back()
/*void guessOutLearn(double input[], double target, NeuralOutput *no, Perceptron perceps[]) {
    double output = guessOutput(input, no);
    double dS = (target - output) * tanh_prime(output);
    for (size_t i = 0; i < NBWEIGHTOUT; i++) {
        training(dS, input[i], no->weights[i], &perceps[i]);
        no->weights[i] += (dS/input[i]) * LEARNING_RATE;
    }
}
*/
