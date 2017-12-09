# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/dir.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

# include "LayerStruct.h"
# include "Picture_Treatment/picture_treatment.h"

////////////////// COPY PASTE IS BAD


/////////////////////

double *loadMatrix(int path, int alpha) {
  char str[80];
  strcpy(str,"Picture_Treatment/tmp");
  char alphac[2];
  alphac[0] = '1'+alpha;
  alphac[1] = '\0';
  strcat(str, alphac);
  strcat(str, "/");
  char snum[5];
  itoa(path, snum);
  strcat(str, snum);
  strcat(str, ".bmp");
  SDL_Surface *surf = Load_Image(str);
  printf("Location : %s \n", str);
  int *arr = malloc(26*26*sizeof(int));
  surf_to_array(surf, arr);
  double *arr2 = malloc(26*26*sizeof(double));

  for (int i = 0; i < 26*26; i++)
    *(arr2+i) = ((double)*(arr+i))/255;

  free(arr);
  SDL_FreeSurface(surf);
  return arr2;
}

void SaveData(Layer *l1, Layer *l2, double learning){
  mkdir("./DATA",0775);
  FILE *fichier1 = fopen("./DATA/Weights1.bin","wb");
  FILE *fichier2 = fopen("./DATA/Weights2.bin","wb");
  FILE *fichier3 = fopen("./DATA/property.txt","w");
  if (fichier1 != NULL)
    {
      fwrite(l1->weights,sizeof(double),l1->nbinput * l1->nbneurone,fichier1);
      fclose(fichier1);
    }
  if (fichier2 != NULL)
    {
      fwrite(l2->weights,sizeof(double),l2->nbinput *l2->nbneurone,fichier2);
      fclose(fichier2);
    }
  if (fichier3 != NULL)
    {
      fprintf(fichier3, "%d\n",l1->nbneurone);
      fprintf(fichier3, "%lf",learning);
      fclose(fichier3);
    }
  }

  double LoadData (Layer *l1, Layer *l2)
  {
    FILE *fichier1 = fopen("./DATA/Weights1.bin","rb");
    FILE *fichier2 = fopen("./DATA/Weights2.bin","rb");
    FILE *fichier3 = fopen("./DATA/property.txt","r");
    int nbneurone;
    double learning;
    fscanf(fichier3, "%d",&nbneurone);
    fscanf(fichier3,"%lf",&learning);
    initLayer(26*26, nbneurone, l1);
    fread(l1->weights,sizeof(double),26*26 * (nbneurone),fichier1);
    initLayer(nbneurone,94,l2);
    fread(l2->weights,sizeof(double), 94 * nbneurone,fichier2);
    return learning;
  }
