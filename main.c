# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>


# include "percep.h"

# include "property.h"

///////////////////////
//
// On peut ajuster le réseau dans le fichier property.h !
//
//////////////////////

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
      weights1[i] = (rand() / (double) RAND_MAX) - 0.5f;
      printf("Weights1 :%f\n",weights1[i] );
    }
    for (size_t i = 0; i < NBWEIGHTOUT; i++)
    {
      weights2[i] = (rand() /(double) RAND_MAX) - 0.5f;
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
            double guess1[NBWEIGHTOUT];
            double guess2[1];
            guess(weights1,input, 1, 2, NBWEIGHTOUT, guess1);
            guess(weights2,res, 1, NBWEIGHTOUT, 1, guess2);
            double result = guess2[0];


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
              training(tanswer, guess2,weights1,weights2,guess1);
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
