# include "percep.h"
# include "property.h"

typedef struct NeuralOutput NeuralOutput;
struct NeuralOutput {
    double weights[NBWEIGHTOUT];
};

void initOutput (NeuralOutput *no) ;
double guessOutput(double input[], NeuralOutput *no);
void guessOutLearn(double input[], double target, NeuralOutput *no, Perceptron perceps[]);
