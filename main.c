# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>


# include "percep.h"
# include "neuraloutput.h"



int main(int argc, char const *argv[]) {
    printf("%f\n", rand()/(double)RAND_MAX);
    Perceptron perceps[4];
    for (size_t i = 0; i < 4; i++) {
        initPerceptron(&perceps[i]);
        printf("%f : weights\n",perceps->weights[i]);
    }
    double input[2] = {1,1};
    double midout[4] = {0};
    for (size_t i = 0; i < 4; i++) {
        midout[i] = guess(input, &perceps[i]);
        printf("%f\n", midout[i]);
    }
    NeuralOutput output;
    initOutput(&output);
    printf("%f\n", guessOutput(midout, &output));

    return 0;
}
