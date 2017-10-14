
typedef struct Perceptron Perceptron;

struct Perceptron {
    double weights[2];
    double learningRate;
};


void initPerceptron (Perceptron *perc);
double guess(double input[], Perceptron *perc);
void training(double input[], double target, Perceptron *perc);
