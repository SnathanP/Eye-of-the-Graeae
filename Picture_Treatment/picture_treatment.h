#ifndef _PICTURE_TREATMENT_H_
#define _PICTURE_TREATMENT_H_

#include <stdlib.h>
#include <SDL.h>

SDL_Surface* Sobel_filter(SDL_Surface *img);
SDL_Surface* color_zone(SDL_Surface *img, int l[]);
void cut(SDL_Surface *img,int l_c_w[]);
void surf_to_array(SDL_Surface *img, int l[]);
void print_array(int h[]);
SDL_Surface* clean_img(SDL_Surface *img);
SDL_Surface* median(SDL_Surface *img);
SDL_Surface* Gray_scale(SDL_Surface *img);
SDL_Surface* Contrast(SDL_Surface *img);
SDL_Surface* threshold(SDL_Surface *img);

#endif
