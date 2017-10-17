# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>


//# include "percep.h"

# include "property.h"
# include "activation.h"
# include "algebra.h"
# include "matrix.h"
///////////////////////
//
// On peut ajuster le réseau dans le fichier property.h !
//
//////////////////////

double dotProduct(double *a, double *b, int length) {
   double runningSum = 0;
   for (int index = 0; index < length; index++)
       runningSum += a[index] * b[index];
   return runningSum;
}

int main(int argc, char const *argv[]) {
    srand ( clock()  );
// INIT
    double input[2] = {1,0};
    double answer = 1;



    FILE* fichier = NULL;
    fichier = fopen(TESTFILE, "r");

    double weights1[NBWEIGHTOUT * 2];
    double weights2[NBWEIGHTOUT];
    for (size_t i = 0; i < 2 * NBWEIGHTOUT; i++)
    {
      weights1[i] = (rand() / (double) RAND_MAX);
      printf("Weights1 :%f\n",weights1[i] );
    }
    for (size_t i = 0; i < NBWEIGHTOUT; i++)
    {
      weights2[i] = (rand() /(double) RAND_MAX);
      printf("Weights2 :%f\n",weights2[i] );
    }

    char data[10] = "";
    int i = 0;
    int truecount = 0;
    int falsecount = 0;
    if (fichier != NULL)
    {

        while (fgets(data, 10, fichier) != NULL) { // On lit le fichier

            input[0] = data[0] - '0'; // On récupère les tests dans le fichier
            input[1] = data[1] - '0';
            answer = data[2] - '0';

            // FRONT
            // ret.hiddenSum = multiply(weights.inputHidden, examples.input);
            double hiddenSum[1*NBWEIGHTOUT];
            // input = 1 * nbinput
            //  weights1 = nbinput * NBWEIGHTOUT
            // hiddenSum = 1 * NBWEIGHTOUT
            mul(input,weights1,1,2,NBWEIGHTOUT,hiddenSum);
            //ret.hiddenResult = ret.hiddenSum.transform(activate);
            double hiddenResult[1*NBWEIGHTOUT];
            //  hiddenResult = 1 * NBWEIGHTOUT
            vector_apply(tanh,hiddenSum,NBWEIGHTOUT,hiddenResult);
            //ret.outputSum = multiply(weights.hiddenOutput, ret.hiddenResult);
            double outputSum[1*1];
            // weights2 = NBWEIGHTOUT * 1
            // outputSum =  1 * 1
            mul(hiddenResult,weights2,1,NBWEIGHTOUT,1, outputSum);
            //ret.outputResult = ret.outputSum.transform(activate);
            double outputResult[1*1];
            //outputResult = 1 * 1
            vector_apply(tanh,outputSum,1,outputResult);
            //ENDFRONT

            double result = outputResult[0];


            // AFFICHAGE
            printf("Ité %d : %d,%d, target = %d\n", i,(int)input[0], (int)input[1], (int)answer);
            printf("%lf", result);
            if ((answer == 1 && result > 0.5) || (answer == 0 && result <= 0.5)) {
                printf(" : TRUE\n");
                truecount++;
            } else {
                printf(" : FALSE\n");
                falsecount++;
            }
            if ((answer == 1 && result < 0.95) || (answer == 0 && result > 0.05))
            {
              double tanswer[1] = {answer};
                // TRAINING
                //var errorOutputLayer = subtract(examples.output, results.outputResult);
                // ERROR
                double errorOutputLayer[1*1];
                //outputResult = 1 * 1
                substract(tanswer,outputResult,1,1,errorOutputLayer);
                //var deltaOutputLayer = dot(results.outputSum.transform(activatePrime), errorOutputLayer);
                double outputSumPrime[1*1];
                vector_apply(tanh_prime,outputSum,1,outputSumPrime);
                // dO
                double deltaOutputLayer[1*1];
                dot(outputSumPrime,errorOutputLayer,1,1,deltaOutputLayer);
                //var hiddenOutputChanges = scalar(multiply(deltaOutputLayer, results.hiddenResult.transpose()), learningRate);
                double hiddenResultT[NBWEIGHTOUT*1];
                transpose(hiddenResult, 1, NBWEIGHTOUT, hiddenResultT);
                //Oc
                double hiddenOutputChanges[NBWEIGHTOUT * 1]; // APPLY TO weights2
                mul(hiddenResult,deltaOutputLayer,NBWEIGHTOUT,1,1,hiddenOutputChanges);
                //var deltaHiddenLayer = dot(multiply(weights.hiddenOutput.transpose(), deltaOutputLayer), results.hiddenSum.transform(activatePrime));
                // dH
                double deltaHiddenLayer[NBWEIGHTOUT*1];
                double weights2T[1*NBWEIGHTOUT];
                transpose(weights2,NBWEIGHTOUT,1,weights2T);
                //  deltaOutputLayer = 1*1
                double tmp[1 * NBWEIGHTOUT];
                // deltaHiddenLayer = NBWEIGHTOUT * 1
                mul(deltaOutputLayer,weights2T, 1, 1, NBWEIGHTOUT, tmp);

                double hiddenSumPrime[1 * NBWEIGHTOUT];
                // hiddenSumPrime = 1 * NBWEIGHTOUT
                vector_apply(tanh_prime,hiddenSum,NBWEIGHTOUT,hiddenSumPrime);

                dot(tmp,hiddenSumPrime,1,NBWEIGHTOUT,deltaHiddenLayer);

                double inputT[2*1];
                transpose(input,1,2,inputT);
                //var inputHiddenChanges = scalar(multiply(deltaHiddenLayer, examples.input.transpose()), learningRate);
                double inputHiddenChanges[2*NBWEIGHTOUT];
                mul(input,deltaHiddenLayer,2,1,NBWEIGHTOUT, inputHiddenChanges);

                add(weights1,inputHiddenChanges,2,NBWEIGHTOUT,weights1);
                add(weights2,hiddenOutputChanges,1,NBWEIGHTOUT,weights2);
                // Hc
              printf("Auto-correction\n\n");
            }
            else{
              printf("\n\n");
            }
            i++;
        }

    printf("True : %d\nFalse : %d\n", truecount, falsecount);
    fclose(fichier); // On ferme le fichier qui a été ouvert
    }

    return 0;
}
