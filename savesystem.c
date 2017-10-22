# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/dir.h>
# include <sys/types.h>
# include <sys/stat.h>

void SaveData(double weights1[], double nbpoids1, double weights2[], double nbneuronecaches,double nbinput){
  mkdir("./DATA",0775);
  FILE *fichier1 = fopen("./DATA/Weights1.bin","wb");
  FILE *fichier2 = fopen("./DATA/Weights2.bin","wb");
  FILE *fichier3 = fopen("./DATA/property.txt","w");
  if (fichier1 != NULL)
    {
      fwrite(weights1,sizeof(double),nbpoids1,fichier1);
      fclose(fichier1);
    }
  if (fichier2 != NULL)
    {
      fwrite(weights2,sizeof(double),nbneuronecaches,fichier2);
      fclose(fichier2);
    }
  if (fichier3 != NULL)
    {
      fprintf(fichier3, "%d ",nbpoids1);
      fprintf(fichier3, "%d ",nbneuronecaches);
      fprintf(fichier3, "%d",nbinput);
      fclose(fichier3);
    }
}
