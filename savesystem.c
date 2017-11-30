# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/dir.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <SDL.h>
#include <SDL/SDL_image.h>

# include "LayerStruct.h"
# include "Picture_Treatment/picture_treatment.h"

////////////////// COPY PASTE IS BAD

SDL_Surface* Load_Image(char *letter){
  SDL_Surface *img;
  char path[2] = {*letter,'\0'};
  img = IMG_Load(path);
  if(!img)
    errx(1, "Can't load %s: %s", path, IMG_GetError());
  return img;
}

/////////////////////

double *loadMatrix(char path) {
  SDL_Surface *surf = Load_Image(&path);
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
    initLayer(nbneurone,52,l2);
    fread(l2->weights,sizeof(double),52 * nbneurone,fichier2);
    return learning;
  }
