# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>

# include "property.h"
# include "LayerStruct.h"
# include "savesystem.h"

int main(int argc, char const *argv[]) {
    srand ( clock()  );

    // one arg = load savefile (futur)
    // Min arg = in, mid, out, test file (futur) + LEARNING_RATE
    // Max arg = in mid, ..., mid, out, test file (futur) + LEARNING_RATE

    // INIT
    Layer layerHidden;
    //Layer layerHidden2; (futur)
    Layer layerOutput;
    int nbinput = 2;

    int nbout = 1;
    double learning = LEARNING_RATE;

    if (argc == 3)
    {
      int nbmid = atoi(argv[1]);
      if (nbmid > 40)
        {
          printf("I don't think more than 40 is necessary for a XOR...\n");
          return 1;
        }
      initLayer(nbinput,nbmid,&layerHidden);
      initLayer(nbmid,nbout,&layerOutput);
    }
    else if (argc == 2)
    {
      learning = LoadData(&layerHidden,&layerOutput);
    }
    else{
      printf("To use this network :\n");
      printf("./main NombreDeNeuronnes FichierDeTest\n");
      printf("Recommandé pour XOR : \"./main 4 tests/10\\ 000.txt > out.txt\"\n");
      return 0;
    }





    double *input = malloc(nbinput * sizeof(double));

    double answer;

    FILE* fichier = NULL;
    fichier = fopen(argv[argc-1], "r");


    //initLayer(HIDDEN,HIDDEN2,&layerHidden2); (futur)

    //////


    char data[10] = "";
    int i = 0;
    int truecount2 = 0;
    int falsecount2 = 0;
    int lastFalse2 = 0;
    if (fichier != NULL)
    {

        while (fgets(data, 10, fichier) != NULL) { // On lit le fichier

            input[0] = data[0] - '0'; // On récupère les tests dans le fichier
            input[1] = data[1] - '0';
            answer = data[2] - '0';

            // FRONT 2
            front2(input,&layerHidden);
            //front2(layerHidden.result,&layerHidden2); (futur)
            front2(layerHidden.result,&layerOutput);
            //ENDFRONT

            double result2 = layerOutput.result[0];

            // AFFICHAGE
            printf("Ité %d : %d,%d, target = %d\n", i,(int)input[0], (int)input[1], (int)answer);
            printf("%lf (Struct)",result2);

            if ((answer == 1 && result2 > 0.5) || (answer == 0 && result2 <= 0.5)) {
                printf(" : TRUE (Struct)\n");
                truecount2++;
            } else {
                printf(" : FALSE (Struct)\n");
                falsecount2++;
                lastFalse2 = i;
            }
            if ((answer == 1 && result2 < 0.95) || (answer == 0 && result2 > 0.05))
            {

                double tanswer[1] = {answer};
                // back - propagation : Il faut commencer par l'output et
                //                      remonter vers l'input.
                outToHidden(tanswer, &layerOutput, &layerHidden);
                //hiddenToHidden(&layerOutput, &layerHidden2, &layerHidden); (futur)
                hiddenToInput(&layerOutput, &layerHidden, input);
                applyChanges(&layerHidden, learning);
                //applyChanges(&layerHidden2); (futur)
                applyChanges(&layerOutput, learning);
                printf("Auto-correction\n\n");
            }
            else{
              printf("\n\n");
            }
            i++;
        }

    printf("True2 : %d\nFalse2 : %d\n", truecount2, falsecount2);
    printf("Last False2 : %d\n", lastFalse2);
    fclose(fichier); // On ferme le fichier qui a été ouvert
  } else  {
    printf("File not found !\n");
    printf("Type just ./main for explainations.\n");
  }
    SaveData(&layerHidden,&layerOutput,learning);
    destroyLayer(&layerHidden);
    //destroyLayer(&layerHidden2);
    destroyLayer(&layerOutput);
    free(input);
    return 0;
}
