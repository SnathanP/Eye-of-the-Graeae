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

void front(double input[], double weights1[], double weights2[], /* RESULTS */ double hiddenSum[], double hiddenResult[], double outputSum[], double outputResult[]) {
    // input = 1 * nbinput
    //  weights1 = nbinput * HIDDEN
    // hiddenSum = 1 * HIDDEN
    mul(input,weights1,1,INPUT,HIDDEN,hiddenSum);
    //  hiddenResult = 1 * HIDDEN
    vector_apply(sigmoid,hiddenSum,HIDDEN,hiddenResult);
    // weights2 = HIDDEN * 1
    // outputSum =  1 * 1
    mul(hiddenResult,weights2,1,HIDDEN,1, outputSum);
    //outputResult = 1 * 1
    vector_apply(sigmoid,outputSum,1,outputResult);
}

void back(double input[], double answer[], double weights1[], double weights2[], double hiddenSum[], double hiddenResult[], double outputSum[], double outputResult[]) {
    //////////////////////////////var errorOutputLayer = subtract(examples.output, results.outputResult);
    double errorOutputLayer[1*1];
    //outputResult = 1 * 1
    substract(answer,outputResult,1,1,errorOutputLayer);
    //////////////////////////////OK

    ///////////////////////////var deltaOutputLayer = dot(results.outputSum.transform(activatePrime), errorOutputLayer);
    double outputSumPrime[1*1];
    vector_apply(sigmoid_prime,outputSum,1,outputSumPrime);
    // dO
    double deltaOutputLayer[1*1];
    dot(outputSumPrime,errorOutputLayer,1,1,deltaOutputLayer);
    ///////////////////////////OK

    ///////////////////////////var hiddenOutputChanges = scalar(multiply(deltaOutputLayer, results.hiddenResult.transpose()), learningRate);
    double hiddenResultT[HIDDEN*1];
    transpose(hiddenResult, 1, HIDDEN, hiddenResultT);
    //Oc
    double hiddenOutputChanges[HIDDEN * 1]; // APPLY TO weights2
    mul(hiddenResult,deltaOutputLayer,HIDDEN,1,1,hiddenOutputChanges);
    ///////////////////////////OK

    ///////////////////////////var deltaHiddenLayer = dot(multiply(weights.hiddenOutput.transpose(), deltaOutputLayer), results.hiddenSum.transform(activatePrime));
    // dH
    double deltaHiddenLayer[HIDDEN*1];
    double weights2T[1*HIDDEN];
    transpose(weights2,HIDDEN,1,weights2T);
    //  deltaOutputLayer = 1*1
    double tmp[1 * HIDDEN];
    // deltaHiddenLayer = HIDDEN * 1
    mul(deltaOutputLayer,weights2T, 1, 1, HIDDEN, tmp);

    double hiddenSumPrime[1 * HIDDEN];
    // hiddenSumPrime = 1 * HIDDEN
    vector_apply(sigmoid_prime,hiddenSum,HIDDEN,hiddenSumPrime);

    dot(tmp,hiddenSumPrime,1,HIDDEN,deltaHiddenLayer);
    //////////////////////// OK

    /////////////////////////var inputHiddenChanges = scalar(multiply(deltaHiddenLayer, examples.input.transpose()), learningRate);
    double inputT[INPUT*1];
    transpose(input,1,INPUT,inputT);
    double inputHiddenChanges[INPUT*HIDDEN];
    mul(input,deltaHiddenLayer,INPUT,1,HIDDEN, inputHiddenChanges);
    ////////////////////////OK

    // Apply learningRate and add weights

    vector_apply(applyLearningRate, inputHiddenChanges, HIDDEN, inputHiddenChanges); // applyLearningRate
    add(weights1,inputHiddenChanges,INPUT,HIDDEN,weights1);

    vector_apply(applyLearningRate, hiddenOutputChanges, HIDDEN, hiddenOutputChanges); // applyLearningRate
    add(weights2,hiddenOutputChanges,1,HIDDEN,weights2);
    // Finish
}
