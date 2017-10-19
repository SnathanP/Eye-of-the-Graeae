#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>

#include "pixel_operations.h"
#include "picture_treatment.h"

void init_SDL(){
  if(SDL_Init(SDL_INIT_VIDEO)==-1)
    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

void Wait_for_exit(){
  int continu = 1;
  SDL_Event event;

  while(continu){
    SDL_WaitEvent(&event);
    switch(event.type){
    case SDL_QUIT:
      continu = 0;
    }
  }
}

SDL_Surface* display(int i, int j, SDL_Surface *img, SDL_Surface *screen){
	SDL_Rect pos;
	pos.x = i;
	pos.y = j;
	SDL_BlitSurface(img, NULL, screen, &pos);
	SDL_UpdateRect(screen, i, j, img->w, img->h);
	return screen;
}

SDL_Surface* Load_Image(char *path){
  SDL_Surface *img;
  img = IMG_Load(path);
  if(!img)
    errx(1, "Can't load %s: %s", path, IMG_GetError());
  return img;
}

int main(){
  SDL_Surface *screen = NULL;
  init_SDL();
  screen = SDL_SetVideoMode(1200, 1200, 32, SDL_HWSURFACE);
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 100, 100, 100));
  char *path = "image.png";
  SDL_Surface *img = Load_Image(path);

  display(0, 0, img, screen);
  img = Gray_scale(img);
  img = median(img);
  display(0, 225, img, screen);
  img = Contrast(img);
  display(0, 450, img, screen);
  img = threshold(img);
  display(225, 0, img, screen);
  img = clean_img(img);
  display(225, 225, img, screen);
  img = Sobel_filter(img);
  display(225, 450, img, screen);
  int l[img->w * img->h];
  surf_to_array(img, l);
  int l_c[img->w * img->h];
  cut(img, l_c);
  img = color_zone(img, l_c);
  display(450, 0, img, screen); 

  Wait_for_exit();
  SDL_FreeSurface(img);
  SDL_Quit();
  return EXIT_SUCCESS;
}
