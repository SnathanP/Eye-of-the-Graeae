# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>


# include "percep.h"

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
    double input[INPUT] = {1,0};
    double answer = 1;

    int lastFalse;

    FILE* fichier = NULL;
    fichier = fopen(TESTFILE, "r");

    double weights1[HIDDEN * INPUT];
    double weights2[HIDDEN];
    for (size_t i = 0; i < INPUT * HIDDEN; i++)
    {
      weights1[i] = (rand() / (double) RAND_MAX);
      printf("Weights1 :%f\n",weights1[i] );
    }
    for (size_t i = 0; i < HIDDEN; i++)
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
            double hiddenSum[1*HIDDEN];
            double hiddenResult[1*HIDDEN];
            double outputSum[1*1];
            double outputResult[1*1];
            front(input, weights1, weights2, hiddenSum, hiddenResult, outputSum, outputResult);
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
                lastFalse = i;
            }
            if ((answer == 1 && result < 0.95) || (answer == 0 && result > 0.05))
            {

                double tanswer[1] = {answer};
                // TRAINING
                back(input, tanswer, weights1, weights2, hiddenSum, hiddenResult, outputSum, outputResult);

              printf("Auto-correction\n\n");
            }
            else{
              printf("\n\n");
            }
            i++;
        }

    printf("True : %d\nFalse : %d\n", truecount, falsecount);
    printf("Last False : %d\n", lastFalse);
    fclose(fichier); // On ferme le fichier qui a été ouvert
    }

    return 0;
}
