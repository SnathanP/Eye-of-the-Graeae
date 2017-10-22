#ifndef LAYER_STRUCT
#define LAYER_STRUCT

typedef struct Layer Layer;
struct Layer {
  int nbinput;
  int nbneurone;
  double *weights;
  double *sum;
  double *result;
  double *sumPrime;
  double *delta;
  double *changes;
  //double *errorLayer;
};

#endif

void applyChanges(Layer *current);
void front2(double input[], Layer *layer);
void outToHidden(double answer[], Layer *last, Layer *previous);
void hiddenToHidden(Layer *next, Layer *current, Layer *previous);
void hiddenToInput(Layer *next, Layer *current, double input[]);
void fillRandom(Layer *layer);
void initLayer(double nbinput, double nbneurone, Layer *layer);
void destroyLayer(Layer *layer);
