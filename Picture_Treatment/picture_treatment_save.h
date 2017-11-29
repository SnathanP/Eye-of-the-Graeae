#ifndef _PICTURE_TREATMENT_H_
#define _PICTURE_TREATMENT_H_

#include <stdlib.h>
#include <SDL.h>
#include <math.h>

SDL_Surface* new_detec(SDL_Surface *img);
SDL_Surface* Sobel_filter(SDL_Surface *img);
SDL_Surface* color_zone(SDL_Surface *img, int *l);
int cut(SDL_Surface *img,int *l_c_w);
void surf_to_array(SDL_Surface *img, int *l);
SDL_Surface* array_to_surf(int *l, SDL_Surface *img);
void print_array(int *h);
SDL_Surface* clean_img(SDL_Surface *img);
SDL_Surface* median(SDL_Surface *img);
SDL_Surface* Gray_scale(SDL_Surface *img);
SDL_Surface* Contrast(SDL_Surface *img);
/*SDL_Surface**/void threshold(SDL_Surface *img);
void array_of_img(SDL_Surface *img, SDL_Surface *imgs[], int *l);

#endif
