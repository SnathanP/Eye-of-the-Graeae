# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>

# include "activation.h"
# include "algebra.h"
# include "matrix.h"
# include "matrix.h"
# include "LayerStruct.h"

void applyChanges(Layer *current)
{
  vector_apply(applyLearningRate, current->changes, current->nbinput*current->nbneurone, current->changes);
  add(current->weights,current->changes,current->nbinput,current->nbneurone,current->weights);
}


void front2(double input[], Layer *layer)
{
  mul(input,layer->weights,1,layer->nbinput,layer->nbneurone,layer->sum);
  vector_apply(sigmoid,layer->sum,layer->nbneurone,layer->result);
}

void outToHidden(double answer[], Layer *last, Layer *previous)
{
  double error[1*last->nbneurone];
  substract(answer,last->result,1,last->nbneurone,error);
  vector_apply(sigmoid_prime,last->sum,last->nbneurone,last->sumPrime);
  dot(last->sumPrime,error,1,last->nbneurone,last->delta);
  //double *resultT = malloc(last->nbneurone*1 * sizeof(double));
  //transpose(previous->result,1,last->nbneurone,resultT);
  mul(previous->result,last->delta,previous->nbneurone,1,last->nbneurone,last->changes);
  //applyChanges(last);
}

void hiddenToHidden(Layer *next, Layer *current, Layer *previous)
{
  double *wT = malloc(next->nbneurone * next->nbinput * sizeof(double));
  transpose(next->weights,next->nbinput,next->nbneurone,wT);
  double *tmp = malloc(1 * next->nbinput * sizeof(double));
  mul(next->delta,wT,1,next->nbneurone,next->nbinput,tmp);
  free(wT);
  vector_apply(sigmoid_prime,current->sum,1*current->nbneurone,current->sumPrime);
  dot(tmp,current->sumPrime,1,current->nbneurone,current->delta);
  free(tmp);
  mul(previous->result,current->delta,current->nbinput,1,current->nbneurone,current->changes);
  //applyChanges(current);
}

void hiddenToInput(Layer *next, Layer *current, double input[])
{
    double *wT = malloc(next->nbneurone * next->nbinput * sizeof(double));
    transpose(next->weights,next->nbinput,next->nbneurone,wT);
    double *tmp = malloc(1 * next->nbinput * sizeof(double));
    mul(next->delta,wT,1,next->nbneurone,next->nbinput,tmp);
    free(wT);
    vector_apply(sigmoid_prime,current->sum,1*current->nbneurone,current->sumPrime);
    dot(tmp,current->sumPrime,1,current->nbneurone,current->delta);
    free(tmp);
    mul(input,current->delta,current->nbinput,1,current->nbneurone,current->changes);
    //applyChanges(current);
}


////////

void fillRandom(Layer *layer)
{
  for (int i = 0; i < layer->nbinput * layer->nbneurone; i++)
  {
    layer->weights[i] = (rand() / (double) RAND_MAX);
    //printf("Weights :%f\n",layer->weights[i]);
  }
}

void initLayer(double nbinput, double nbneurone, Layer *layer)
{
  layer->nbinput = nbinput;
  layer->nbneurone = nbneurone;
  layer->weights = malloc(nbinput*nbneurone * sizeof(double));
  if (layer->weights == NULL) {
    exit(0);
  }
  layer->sum = malloc(1*nbneurone * sizeof(double));
  layer->result = malloc(1*nbneurone * sizeof(double));
  layer->sumPrime = malloc(1*nbneurone * sizeof(double));
  layer->delta = malloc(1*nbneurone * sizeof(double));
  layer->changes = malloc(nbinput*nbneurone * sizeof(double));
  fillRandom(layer);
}

void destroyLayer(Layer *layer)
{
  free(layer->weights);
  free(layer->sum);
  free(layer->result);
  free(layer->sumPrime);
  free(layer->changes);
  free(layer->delta);
}
