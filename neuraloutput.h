# include "percep.h"
# define NBWEIGHTOUT 4

typedef struct NeuralOutput NeuralOutput;
struct NeuralOutput {
    double weights[NBWEIGHTOUT];
};

void initOutput (NeuralOutput *no) ;
double guessOutput(double input[], NeuralOutput *no);
void guessOutLearn(double input[], double target, NeuralOutput *no, Perceptron perceps[]);
