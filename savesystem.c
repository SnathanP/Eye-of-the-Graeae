# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/dir.h>
# include <sys/types.h>
# include <sys/stat.h>

# include "LayerStruct.h"

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
    initLayer(2, nbneurone, l1);
    fread(l1->weights,sizeof(double),2* (nbneurone),fichier1);
    initLayer(nbneurone,1,l2);
    fread(l2->weights,sizeof(double),1* nbneurone,fichier2);
    return learning;
  }
