#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "pixel_operations.h"
#include "picture_treatment.h"

#define WIDTH 1200
#define HEIGHT 900

void init_SDL(){
  if(SDL_Init(SDL_INIT_VIDEO)==-1)
    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

char *itoa(int num, char *str)
{
        if(str == NULL)
        {
                return NULL;
        }
        sprintf(str, "%d", num);
        return str;
}

void Wait_for_exit(){
  int continu = 1;
  SDL_Event event;

  while(continu){
    SDL_WaitEvent(&event);
    switch(event.type){
      case SDL_MOUSEBUTTONDOWN:
      continu = 0;
    }
  }
}

void display(int i, int j, SDL_Surface *img, SDL_Surface *screen){
	SDL_Rect pos;
	pos.x = i;
	pos.y = j;
	SDL_BlitSurface(img, NULL, screen, &pos);


	//return screen;
}



/*
main:
treatment and segmentation of chararcters in an image img.
*/



int main(int argc, char* argv[]){

	// variable declarations
	SDL_Window *win = NULL;
	//SDL_Renderer *renderer = NULL;
	//int w, h; // texture width & height

	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
			return 1;

	// create the window and renderer
	// note that the renderer is accelerated
	win = SDL_CreateWindow("Eye of the Graeae", 100, 100, WIDTH, HEIGHT, 0);
  //renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  SDL_Surface* screen = SDL_GetWindowSurface(win);
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 100, 100, 100));

  if(argc < 2){
    printf("Error: Need a path to the image\n");
    return 1;
  }
  char *path = argv[1];
  SDL_Surface *img = Load_Image(path);

  display(0, 0, img, screen);
  SDL_UpdateWindowSurface(win); // POUR ACTUALISER LA FENETRE
  Gray_scale(img);
  SDL_Surface *copy = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0, 0);
  int *copy_l = malloc(sizeof(int) * img->h * img->w);
  surf_to_array(img,copy_l);
  array_to_surf(copy_l, copy);
  free(copy_l);
  //img = new_detec(img);
  /*img = median(img);
  display(310, 0, img, screen);
  img = Contrast(img);
  display(620, 0, img, screen);
  img = */threshold(img);
  //display(0, 310, img, screen);
  /*img = clean_img(img);
  display(310, 310, img, screen);
  img = Sobel_filter(img);
  display(620, 310, img, screen);
  */int *img_cut = malloc(sizeof(int) * img->w * img->h);
  cut(img, img_cut);
  SDL_Surface *imgs[img_cut[0]];
  array_of_img(img, imgs, img_cut);
  for(int i = 0; i < img_cut[0]; i++)
    display(i * 52, 620, imgs[i], screen);


  SDL_UpdateWindowSurface(win); // POUR ACTUALISER LA FENETRE
  Wait_for_exit();
  SDL_FreeSurface(img);
  SDL_FreeSurface(copy);

  for(int i = 0; i < img_cut[0]; i++) {
    char str[80];
    strcpy(str, "tmp/");
    char snum[5];
    itoa(i, snum);
    strcat(str, snum);
    strcat(str, ".bmp");
    SDL_SaveBMP(imgs[i], str);
  }
  for(int i = 0; i < img_cut[0]; i++)
    SDL_FreeSurface(imgs[i]);
  free(img_cut);
  SDL_FreeSurface(screen);
  SDL_Quit();
  return EXIT_SUCCESS;
}
