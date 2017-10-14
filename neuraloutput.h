typedef struct NeuralOutput NeuralOutput;
struct NeuralOutput {
    double weights[4];
};

void initOutput (NeuralOutput *no) ;
double guessOutput(double input[], NeuralOutput *no);
