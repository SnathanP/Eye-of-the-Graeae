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

/*
main:
treatment and segmentation of chararcters in an image img.
*/
int main(int argc, char* argv[]){
  SDL_Surface *screen = NULL;
  init_SDL();
  screen = SDL_SetVideoMode(1000, 1200, 32, SDL_HWSURFACE);
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 100, 100, 100));
  if(argc < 2){
    printf("Error: Need a path to the image\n");
    return 1;
  }
  char *path = argv[1];
  SDL_Surface *img = Load_Image(path);

  display(0, 0, img, screen);
  img = Gray_scale(img);
  img = median(img);
  display(310, 0, img, screen);
  img = Contrast(img);
  display(620, 0, img, screen);
  img = threshold(img);
  display(0, 310, img, screen);
  img = clean_img(img);
  display(310, 310, img, screen);
  SDL_Surface *copy = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0, 0);
  int copy_l[img->h * img->w];
  surf_to_array(img,copy_l);
  copy = array_to_surf(copy_l, copy);
  img = Sobel_filter(img);
  display(620, 310, img, screen);
  int l[img->w * img->h];
  surf_to_array(img, l);
  int img_cut[img->w * img->h];
  cut(img, img_cut);
  SDL_Surface *imgs[img_cut[0]];
  array_of_img(copy, imgs, img_cut);
  for(int i = 0; i < img_cut[0]; i++)
    display(i * 110, 620, imgs[i], screen);

  Wait_for_exit();
  SDL_FreeSurface(img);
  for(int i = 0; i < img_cut[0]; i++)
    SDL_FreeSurface(imgs[i]);
  SDL_Quit();
  return EXIT_SUCCESS;
}
