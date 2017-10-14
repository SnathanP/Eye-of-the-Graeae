# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>


# include "percep.h"
# include "neuraloutput.h"



int main(int argc, char const *argv[]) {
    srand ( clock()  );
    Perceptron perceps[4];
    for (size_t i = 0; i < 4; i++) {
        initPerceptron(&perceps[i]);
    }
    double input[2] = {1,0};
    double midout[4] = {0};
    for (size_t i = 0; i < 4; i++) {
        midout[i] = guess(input, &perceps[i]);
        printf("%lf\n", midout[i]);
    }
    NeuralOutput output;
    initOutput(&output);
    printf("%lf : final\n\n", guessOutput(midout, &output));

    guessOutLearn(midout, 1, &output, perceps);

    for (size_t i = 0; i < 4; i++) {
        midout[i] = guess(input, &perceps[i]);
        printf("%lf\n", midout[i]);
    }
    printf("%lf : final\n\n", guessOutput(midout, &output));
    return 0;
}
