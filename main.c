# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>


# include "percep.h"
# include "neuraloutput.h"

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
    double midout[NBWEIGHTOUT] = {0};
    Perceptron perceps[NBWEIGHTOUT];
    for (size_t i = 0; i < NBWEIGHTOUT; i++) {
        initPerceptron(&perceps[i]);
    }
    NeuralOutput output;
    initOutput(&output);

    FILE* fichier = NULL;
    fichier = fopen(TESTFILE, "r");

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

            for (size_t i = 0; i < NBWEIGHTOUT; i++) {
                midout[i] = guess(input, &perceps[i]);
                //printf("%lf\n", midout[i]);
            }
            double res = guessOutput(midout, &output);

            // AFFICHAGE
            printf("Ité %d : %d,%d, target = %d\n", i,(int)input[0], (int)input[1], (int)answer);
            printf("%lf", res);
            if ((answer == 1 && res > 0.5) || (answer == 0 && res <= 0.5)) {
                printf(" : TRUE\n");
                truecount++;
            } else {
                printf(" : FALSE\n");
                falsecount++;
            }
            if ((answer == 1 && res < 0.95) || (answer == 0 && res > 0.05))
            {
              guessOutLearn(midout, answer, &output, perceps);
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
