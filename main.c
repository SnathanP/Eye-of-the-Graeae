# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>

# include "property.h"
# include "LayerStruct.h"
# include "savesystem.h"
# include "matrix.h"

int main(int argc, char const *argv[]) {
    srand ( clock()  );

    // one arg = load savefile (futur)
    // Min arg = in, mid, out, test file (futur) + LEARNING_RATE
    // Max arg = in mid, ..., mid, out, test file (futur) + LEARNING_RATE

    // INIT
    Layer layerHidden;
    //Layer layerHidden2; (futur)
    Layer layerOutput;
    int nbinput = 26*26;

    int nbout = 26;
    double learning = LEARNING_RATE;

    if (argc == 3)
    {
      int nbmid = atoi(argv[1]);
      if (nbmid > 99999)
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





    //double *input = malloc(nbinput * sizeof(double));


    //FILE* fichier = NULL;
    //fichier = fopen(argv[argc-1], "r");
    int ite = atoi(argv[argc-1]);

    //initLayer(HIDDEN,HIDDEN2,&layerHidden2); (futur)

    //////


    int truecount2 = 0;
    int falsecount2 = 0;
    int lastFalse2 = 0;


        for(int i = 0; i < ite; i++) { // On lit le fichier

            //input[0] = data[0] - '0'; // On récupère les tests dans le fichier
            //input[1] = data[1] - '0';
            //answer = data[2] - '0';
            int r = rand() % 26;
            char path = r+'A'; //Faire un random 26 pour trouver une lettre, la mettre dans path, importer en matrice la lettre .bmp
            double *input = loadMatrix(path);
            print_matrix(input,26,26);
            double *answer = malloc(26*sizeof(double));
            for(int i = 0; i < 26; i++)
              answer[i] = 0;
            answer[path-'A'] = 1;

            // FRONT 2
            front2(input,&layerHidden);
            //front2(layerHidden.result,&layerHidden2); (futur)
            front2(layerHidden.result,&layerOutput);
            //ENDFRONT
            double result2 = -1;
            int resultpos;

            for(int it=0; it < 26; it++) {
                //printf("%lf:%c\n", layerOutput.result[it], 'A'+it);
                if(layerOutput.result[it]>result2) {
                  result2 = layerOutput.result[it];
                  resultpos = it;
                }
            }

            // AFFICHAGE
            printf("Ité %d : target = %c\n", i,path);
            printf("%lf (Struct) = %c",result2, 'A'+resultpos);

            if (answer[resultpos] == 1) {
                printf(" : TRUE (Struct)\n");
                truecount2++;
            } else {
                printf(" : FALSE (Struct)\n");
                falsecount2++;
                lastFalse2 = i;
            }
            if (answer[resultpos] == 0 || result2 < 0.95)
            {

                //double tanswer[1] = {answer};
                // back - propagation : Il faut commencer par l'output et
                //                      remonter vers l'input.
                outToHidden(answer, &layerOutput, &layerHidden);
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
            free(input);
            free(answer);
          }


    printf("True2 : %d\nFalse2 : %d\n", truecount2, falsecount2);
    printf("Last False2 : %d\n", lastFalse2);
    //fclose(fichier); // On ferme le fichier qui a été ouvert

    SaveData(&layerHidden,&layerOutput,learning);
    destroyLayer(&layerHidden);
    //destroyLayer(&layerHidden2);
    destroyLayer(&layerOutput);


    return 0;
}
