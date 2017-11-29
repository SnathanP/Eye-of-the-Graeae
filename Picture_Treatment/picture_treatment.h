#ifndef _PICTURE_TREATMENT_H_
#define _PICTURE_TREATMENT_H_

#include <stdlib.h>
#include <SDL.h>
#include <math.h>

SDL_Surface *ScaleSurface(SDL_Surface *Surface, Uint16 Width, Uint16 Height);
void new_detec(SDL_Surface *img);
void Sobel_filter(SDL_Surface *img);
void color_zone(SDL_Surface *img, int *l);
int cut(SDL_Surface *img,int *l_c_w);
void surf_to_array(SDL_Surface *img, int *l);
void array_to_surf(int *l, SDL_Surface *img);
void print_array(int *h);
void clean_img(SDL_Surface *img);
void median(SDL_Surface *img);
void Gray_scale(SDL_Surface *img);
void Contrast(SDL_Surface *img);
void threshold(SDL_Surface *img);
void array_of_img(SDL_Surface *img, SDL_Surface *imgs[], int *l);

#endif
