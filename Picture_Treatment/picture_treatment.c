#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "pixel_operations.h"
#include "picture_treatment.h"

#define WIDTH 1200
#define HEIGHT 900

int is_malloc_error(int *l, int *p[], size_t size){
  if(l == NULL){
    for(size_t i = 0; i < size; i++)
      free(p[i]);
    warn("Error: fails memory allocation");
    return 1;
  }
  return 0;
}

SDL_Surface* Load_Image(char *path){
  SDL_Surface *img;
  IMG_Init(IMG_INIT_PNG);
  img = IMG_Load(path);
  if(!img)
    errx(1, "Can't load %s: %s", path, IMG_GetError());
  return img;
}

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

double **getFinal(char* path, int* taille) {

	// variable declarations
	//SDL_Window *win = NULL;
	//SDL_Renderer *renderer = NULL;
	//int w, h; // texture width & height

	// Initialize SDL.
	//SDL_Init(SDL_INIT_VIDEO);
	
	// create the window and renderer
	// note that the renderer is accelerated
	//win = SDL_CreateWindow("Eye of the Graeae", 100, 100, WIDTH, HEIGHT, 0);
  //renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  //SDL_Surface* screen = SDL_GetWindowSurface(win);
  //SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 100, 100, 100));

  SDL_Surface *img = Load_Image(path);

  //display(0, 0, img, screen);
  //SDL_UpdateWindowSurface(win); // POUR ACTUALISER LA FENETRE
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
  */
  int *img_cut = malloc(sizeof(int) * img->w * img->h);
  //int *l_space = malloc(sizeof(int) * img->w * img->h);
  int *l_back = malloc(sizeof(int) * img->h * img->w);
  printf("%p / %p\n\n", img_cut, l_back);
  //cut(img, img_cut, l_back/*, l_space*/);
  SDL_Surface *imgs[img_cut[0]];
  array_of_img(img, imgs, img_cut);
  //for(int i = 0; i < img_cut[0]; i++)
  //  display(i * 52, 620, imgs[i], screen);


  //SDL_UpdateWindowSurface(win); // POUR ACTUALISER LA FENETRE
  //Wait_for_exit();

  //SDL_Quit();

  double **res = malloc(sizeof(double*)*img_cut[0]);
  for (Uint16 i = 0; i < img_cut[0]; i++) {
    *(res+i) = malloc(26*26*sizeof(double));
    surf_to_double(imgs[i], *(res+i));
  }
  SDL_FreeSurface(img);
  SDL_FreeSurface(copy);
  for(int i = 0; i < img_cut[0]; i++)
    SDL_FreeSurface(imgs[i]);
  *taille = img_cut[0];
  free(img_cut);
  return res;
}


/*
sort_array_Uint8_spe -> Uint8 l[],
sort a array l of type Uint8 in which the index 0 contain the nb of value.
*/
void sort_array_Uint8_spe(Uint8 *l){
  for(int i = 1; i < l[0]; i++){
    int min_index = i;
    for(int j = i+1; j < l[0]; j++)
      min_index = l[j] <= l[min_index] ? j : min_index;
    Uint8 memory = l[i];
    l[i] = l[min_index];
    l[min_index] = memory;
  }
}

/*
neighbors -> SDL_Surface *img, int x, int y, Uint8 l[], int dneight:
stock in l[0] the nb of neighbors and next the value of brightness of each
neighbors and his.
*/
void neighbors(SDL_Surface *img, int  x, int  y, Uint8 *l, int dneight){
  l[0] = 0;
  int index = 1;
  for(int i = x - dneight; i <= x + dneight; i++){
    for(int j = y - dneight; j <= y + dneight; j++){
      if(i >= 0 && j >= 0 && i < img->h && j < img->w){
        l[index] = get_light(img, j, i);
        l[0] += 1;
        index += 1;
      }
    }
  }
  sort_array_Uint8_spe(l);
}

/*
surf_to_array -> SDL_Surface *img, int l[]:
transform an array l whith the same size than img on a matrix representing img.
*/
void surf_to_array(SDL_Surface *img, int *l){
  for(int i = 0; i < img->h; i++)
    for(int j = 0; j < img->w; j++)
      l[j + i * img->w] = get_light(img, j, i);
}

void surf_to_double(SDL_Surface *img, double *l) {
  for(int i = 0; i < img->h; i++)
    for(int j = 0; j < img->w; j++)
      l[j + i * img->w] = ((double)get_light(img, j, i)) / 255;
}

/*
array_to_surf -> int l[], SDL_Surface *img:
apply a matrix representing an image to an surface img with the same format.
*/
void array_to_surf(int *l, SDL_Surface *img){
  for(int i = 0; i < img->h; i++){
    for(int j = 0; j < img->w; j++){
      Uint8 rgb = l[j + i * img->w];
      putpixel(img, j, i, SDL_MapRGB(img->format, rgb, rgb, rgb));
    }
  }
}

/*
h_cut_array -> SDL_Surface *img, int l_c[]:
*parcour* line by line the image img and stock in l_c first the line
where it detects a black pixel and next the lines where
there is no more black pixel.
At the end remove all group of coordinates whith a difference below 5.
*/
int h_cut_array(SDL_Surface *img, int *l_c){
  int *l = malloc(sizeof(int) * img->w * img->h);
  if(is_malloc_error(l, NULL, 0))
    return 0;
  surf_to_array(img, l);
  l_c[0] = 0;
  int index = 1, box_engaged = 0;
  for(int i = 0; i < img->h; i++){
    int sum = 0;
    for(int j = 0; j < img->w && !sum; j++){
      sum = l[j + i * img->w] == 255 ? 0 : 1;
    }
    if(sum && !box_engaged){
      l_c[index] = i;
      index += 1;
      box_engaged = 1;
      l_c[0] += 1;
    }
    else if(!sum && box_engaged){
      l_c[index] = i - 1;
      index += 1;
      box_engaged = 0;
      l_c[0] += 1;
    }
  }
  if(l_c[0]%2 == 1){
    l_c[index] = img->w - 1;
    l_c[0] += 1;
  }
  l_c[0] = l_c[0]/2;
  /*for(int i = 1; i < l_c[0]*2; i++){
    while(l_c[i+1] - l_c[i] < 5){
      l_c[i+1] = l_c[l_c[0]*2];
      l_c[i] = l_c[l_c[0]*2-1];
      l_c[0] -= 1;
    }
    i++;
  }*/
  free(l);
  return 1;
}

/*
w_cut_array -> int l[], int w, int h, int l_c[]:
*parcour* columns by columns the partitions of an image, of size wxh,
stock in l and put in l_c first the columns where it detects a black
pixel and next the columns where there is no more black pixel.
At the end remove all group of coordinates whith a difference below 5x5.
*/
void w_cut_array(int *l, int w, int h, int *l_c){
  l_c[0] = 0;
  int index = 1, box_engaged = 0;
  for(int i = 0; i < w; i++){
    int sum = 0;
    for(int j = 0; j < h && !sum; j++){
      sum = l[i + j * w] == 255 ? 0 : 1;
    }
    if(sum && !box_engaged){
      l_c[index] = i;
      index += 1;
      box_engaged = 1;
      l_c[0] += 1;
    }
    else if(!sum && box_engaged){
      l_c[index] = i;
      index += 1;
      box_engaged = 0;
      l_c[0] += 1;
    }
  }
  if(l_c[0]%2 == 1){
    l_c[index] = w - 1;
    l_c[0] += 1;
  }
  l_c[0] = l_c[0]/2;
  /*for(int i = 1; i < l_c[0]*2; i++){
    while(l_c[i+1] - l_c[i] < 5){
      l_c[i+1] = l_c[l_c[0]*2];
      l_c[i] = l_c[l_c[0]*2-1];
      l_c[0] -= 1;
    }
    i++;
  }*/
}

void h_cut_2(SDL_Surface *img, int *l_c, int i){
  int *l = malloc(sizeof(int) * img->w * img->h);
  if(is_malloc_error(l, NULL, 0))
    return;
  surf_to_array(img, l);
  int y1 = l_c[i], y2 = l_c[i+1], x1 = l_c[i+2], x2 = l_c[i+3];
  int begin = y1, end = y2;
  for(int i = y1; i < y2; i++){
    int sum = 0;
    for(int j = x1; j < x2 && !sum; j++){
      sum = l[j + i * img->w] == 255 ? 0 : 1;
    }
    if(sum) {
      begin = i;
      break;
    }
  }
  for(int i = y2; i > y1; i--) {
    int sum = 0;
    for(int j = x1; j < x2 && !sum; j++){
      sum = l[j + i * img->w] == 255 ? 0 : 1;
    }
    if(sum) {
      end = i;
      break;
    }
  }
  l_c[i + 1] = end;
  l_c[i + 0] = begin;
  free(l);
}

void get_spaces(int *l_c, int *l_s, int begin_line){
  int sum = -0, nb = 0;
  for(int i = begin_line; i < l_c[0] - 1; i++){
    int delta = l_c[i*4 + 3] - l_c[(i-1)*4 + 4];
    sum += delta;
    nb += 1;
    if(delta > sum/nb){
      sum += 2*delta;
      nb += 2;
    }
  }
  int moy = sum/nb;
  int next_space = 1;
  for(int i = begin_line; i < l_c[0] - 1; i++){
    int space_bitw = l_c[i*4 + 3] - l_c[(i-1)*4 + 4];
    if(space_bitw > moy){
      l_s[next_space] += i;
      l_s[0] += 1;
      next_space += 1;
    }
  }
}

/*
cut -> SDL_Surface *img, int array_coords[]:
put in array_coords the section of the image where it detect letters.
*/
int cut(SDL_Surface *img,int *array_coords, int *l_back, int *l_space){
  int *copy = malloc(sizeof(int) * img->w * img->h);
  if(is_malloc_error(copy, NULL, 0))
    return 0;
  
  int *pointers[] = {copy, NULL, NULL};
  surf_to_array(img, copy);
  int *l_c_h = malloc(sizeof(int) * (img->h + 1));
  if(is_malloc_error(l_c_h, pointers, 1))
    return 0;
  
  pointers[1] = l_c_h;
  array_coords[0] = 0;
  if(!h_cut_array(img, l_c_h))
    return 0;
  
  int index_lch = 1, index_lcw = 1, next_back = 1;
  l_back[0] = -1;
  l_space[0] = 0;
  for(int i = 0; i < l_c_h[0]; i++){
    int x = l_c_h[index_lch], y = l_c_h[index_lch + 1];
    index_lch += 2;
    if(y - x > 3){
      int *l_tmp = malloc(sizeof(int) * (y-x) * img->w);
    
      if(is_malloc_error(l_tmp, pointers, 2))
        return 0;
    
      pointers[2] = l_tmp;
    
      for(int j = x * img->w; j <= y * img->w; j++)
        l_tmp[j - x * img->w] = copy[j];
    
      int *l_c_w = malloc(sizeof(int) * (img->w + 1));
    
      if(is_malloc_error(l_c_w, pointers, 3))
        return 0;
    
      w_cut_array(l_tmp, img->w, y-x, l_c_w);
      int beg_space_det = array_coords[0] + 1;
    
      for(int j = 1; j <= l_c_w[0]; j++){
        array_coords[index_lcw] = x;
        array_coords[index_lcw+1] = y;
        array_coords[index_lcw+2] = l_c_w[j*2-1];
        array_coords[index_lcw+3] = l_c_w[j*2];
        array_coords[0] += 1;
        index_lcw += 4;
      }
      get_spaces(array_coords, l_space, beg_space_det);
            l_back[next_back] = array_coords[0];
      l_back[0] += 1;
      next_back += 1;
      
      free(l_tmp);
      free(l_c_w);
    }
  }
  printf("nb_char = %d\n", *array_coords);
  printf("nb_back = %d\n", *l_back);
  printf("nb_space = %d\n", *l_space);

  for(int i = 1; i < 1+array_coords[0]*4; i += 4)
    h_cut_2(img,array_coords,i);

  free(copy);
  free(l_c_h);
  return 1;
}

/*
Sobel_horizontal -> SDL_Surface *img, int l[]:
Transform l into a map where changement of contrast are save as 1.
Changement of contrast are detected by scaning the img from left to right.
*/
int Sobel_horizontal(SDL_Surface *img, int *l){
  int *copy = malloc(sizeof(int) * img->w * img->h);
  if(is_malloc_error(copy, NULL, 0))
    return 0;
  surf_to_array(img, copy);
  for(int i = 0; i < img->h; i++){
    l[i * img->w] = 0;
    for(int j = 1; j < img->w; j++){
      int index = j + i * img->w;
      l[index] = copy[index] != copy[index -1] ? 1 : 0;
    }
  }
  free(copy);
  return 1;
}

/*
Sobel_horizontal -> SDL_Surface *img, int l[]:
Transform l into a map where changement of contrast are save as 1.
Changement of contrast are detected by scaning the img from top to bottom.
*/
int Sobel_vertical(SDL_Surface *img, int *l){
  int *copy = malloc(sizeof(int) * img->w * img->h);
  if(is_malloc_error(copy, NULL, 0))
    return 0;
  surf_to_array(img, copy);
  for(int i = 0; i < img->w; i++){
    l[i] = 0;
    for(int j = 1; j < img->h; j++){
      int index = i + j * img->w;
      l[index] = copy[index] != copy[i + (j-1) * img->w] ? 1 : 0;
    }
  }
  free(copy);
  return 1;
}

/*
Sobel_filter -> SDL_Surface *img:
detect and mark in black changement of contrast the image img.
*/
void Sobel_filter(SDL_Surface* img){
  int *l_h = malloc(sizeof(int) * img->h * img->w);
  if(is_malloc_error(l_h, NULL, 0))
    return;
  int *pointers[] = {l_h, NULL};
  int *l_v = malloc(sizeof(int) * img->h * img->w);
  if(is_malloc_error(l_v, pointers, 1))
    return;
  pointers[1] = l_v;
  if(!Sobel_horizontal(img, l_h) || !Sobel_vertical(img, l_v))
    return;
  int *new_img = malloc(sizeof(int) * img->h * img->w);
  if(is_malloc_error(new_img, pointers, 2))
    return;
  for(int i = 0; i < img->h; i++){
    for(int j = 0; j < img->w; j++){
      int index = j + i * img->w;
      new_img[index] = l_h[index] || l_v[index] ? 0 : 255;
    }
  }
  free(l_h);
  free(l_v);
  array_to_surf(new_img, img);
  free(new_img);
}

void new_detec(SDL_Surface *img){
  int *l = malloc(sizeof(int) * img->w * img->h);
  if(is_malloc_error(l, NULL, 0))
    return;
  int *save = malloc(sizeof(int) * img->w * img->h);
  if(is_malloc_error(save, NULL, 0)){
    free(l);
    return;
  }
  surf_to_array(img, save);
  for(int i = 0; i < img->h; i++){
    for(int j = 0; j < img->w; j++){
      if(i == 0 || j == 0)
        l[j + i * img->w] = 0;
      else{
        int b = save[j - 1 + i * img->w];
        int c = save[j + (i-1) * img->w];
        int d = save[j + 1 + i * img->w];
        int e = save[j + (i+1) * img->w];
        int pow_b_d = (b-d) * (b-d);
        int pow_c_e = (c-e) * (c-e);
        l[j + i * img->w] = sqrt(pow_b_d + pow_c_e);
      }
    }
  }
  free(save);
  array_to_surf(l, img);
  free(l);
}

/*
clean_img -> SDL_Surface *img:
reset to 0 all the pixel wich do not have at least 3 black pixels
in his neighborhood
*/
void clean_img(SDL_Surface *img){
  int *l = malloc(sizeof(int) * img->w * img->h);
  if(is_malloc_error(l, NULL, 0))
    return;
  surf_to_array(img, l);
  for(int i = 0; i < img->h; i++){
    for(int j = 0; j < img->w; j++){
      Uint8 *neight = malloc(sizeof(Uint8) * 10);
      if(neight == NULL){
        free(l);
        warn("ERROR: fails memory allocation");
        return;
      }
      neighbors(img, i, j, neight, 1);
      int sum = 0;
      for(Uint8 k = 1; k < neight[0]; k++)
        sum += neight[k];
      if(sum > 1020)
        l[j + i * img->w] = 255;
      free(neight);
    }
  }
  array_to_surf(l, img);
  free(l);
}

int moy_all(int *l, int size){
  long sum = 0;
  for(int i = 0; i < size; i++)
    sum += l[i];
  return (int)((double)sum / size);
}

/*
median -> SDL_Surface *img:
replace all the pixel by the median value among
the value of his neightbors and his.
*/
void median(SDL_Surface *img){
  int *l = malloc(sizeof(int) * img->w * img->h);
  if(is_malloc_error(l, NULL, 0))
    return;
  surf_to_array(img, l);
  for(int i = 0; i < img->h; i++){
    for(int j = 0; j < img->w; j++){
      Uint8 *neight = malloc(sizeof(int) * 10);
      if(neight == NULL){
        free(l);
        warn("ERROR: fails memory allocation");
        return;
      }
      for(int k = 0; k < 10; k++)
        neight[k] = 0;
      neighbors(img, i, j, neight, 1);
      Uint8 rgb = neight[neight[0]/2 + !(neight[0]%2 == 1)];
      l[j + i * img->w] = rgb;
      free(neight);
    }
  }
  array_to_surf(l,img);
  free(l);
}

/*
Grey_scale -> SDL_Surface *img:
return img after applying a greyscale on each pixel of img.
 */
void Gray_scale(SDL_Surface *img){
  for(int i = 0;i < img->w;i++){
    for(int j = 0;j < img->h;j++){
      Uint32 p = getpixel(img, i, j);
      Uint8 r, g, b;
      SDL_GetRGB(p, img->format, &r, &g, &b);
      Uint8 moy = (r + g + b) / 3;
      putpixel(img, i, j, SDL_MapRGB(img->format, moy, moy, moy));
    }
  }
}

/*
Contrast -> SDL_Surface *img:
upgrade the contrast of img and return img.
 */
void Contrast(SDL_Surface *img){
  int min = 255, max = 0;
  for(int i = 0;i < img->w;i++){
    for(int j = 0;j < img->h;j++){
      Uint8 r = get_light(img, i, j);
      min = r < min ? r : min;
      max = r > max ? r : max;
      }
  }
  double fac = 255/(double)(max - min);
  for(int i = 0;i < img->w;i++){
    for(int j = 0;j < img->h;j++){
      Uint8 r = get_light(img, i, j);
      int moy = (int)(fac*(r - min));
      moy = moy < 0 ? 0 : moy > 255 ? 255 : moy;
      Uint8 rgb = (Uint8)moy;
      putpixel(img, i, j, SDL_MapRGB(img->format, rgb, rgb, rgb));
    }
  }
}

/*
threshold -> SDL_Surface *img:
calculate the average of the brightness of each pixel. Every pixel below average
become black, others become white.
*/
void threshold(SDL_Surface *img){
  int *copy = malloc(sizeof(int) * img->w * img->h);
  if(is_malloc_error(copy, NULL, 0))
    return;
  surf_to_array(img, copy);
  int n = 110;
  for(int i = 0; i < img->w * img->h; i++)
    copy[i] = copy[i] < n ? 0 : 255;
  array_to_surf(copy, img);
  free(copy);
}

/*
img_resizing -> SDL_Surface *img, int w, int h:
compress an image of size wxh to a size < 26x26.
*/
void img_resizing(SDL_Surface *img, int w, int h){
  int *copy = malloc(sizeof(int) * img->h * img->w);
  if(is_malloc_error(copy, NULL, 0))
    return;
  int *pointers[] = {copy};
  surf_to_array(img, copy);
  int w_copy = img->w;
  int *tmp = malloc(sizeof(int) * h * w);
  if(is_malloc_error(tmp, pointers, 1))
    return;
  pointers[0] = tmp;
  while(w > 26 || h > 26){
    w = w/2;
    h = h/2;
    for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
        int sum = copy[2*i*w_copy+2*j] + copy[(2*i+1)*w_copy+2*j];
        sum += copy[2*i*w_copy+(2*j+1)] + copy[(2*i+1)*w_copy+(2*j+1)];
        tmp[j + i * w] = (sum < 510) ? 0 :255;
      }
    }
    for(int i = 0; i < h*w; i++)
      copy[i] = tmp[i];
    w_copy = w;
  }
  free(copy);
  int *n_img = malloc(sizeof(int) * h * w);
  if(is_malloc_error(n_img, pointers, 1))
    return;
  for(int i = 0; i < h*w; i++)
    n_img[i] = tmp[i];
  free(tmp);
  SDL_Surface *resized_img;
  resized_img = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
  array_to_surf(n_img, resized_img);
  free(n_img);
  free(img);
  img = resized_img;
}

/*
array_of_img -> SDL_Surface *img, SDL_Surface*imgs[], int l[]:
Stock in imgs all the parts cut in img which have their coordonates stock in l.
*/
void array_of_img(SDL_Surface *img, SDL_Surface *imgs[], int *l){
  int index = 1, index_imgs = 0, cmpt = 0;
  for(int k = 0; k < l[0]; k++){
    int y1 = l[index], y2 = l[index+1], x1 = l[index+2], x2 = l[index+3];
    SDL_Surface *new_img,*resized_img, *center_img;
    new_img = SDL_CreateRGBSurface(0, x2-x1, y2-y1, 32, 0, 0, 0, 0);
    SDL_FillRect(new_img, NULL, SDL_MapRGB(new_img->format, 255, 255, 255));
    //if(x2-x1 <= 26 && y2-y1 <= 26){
      SDL_Rect src;//, center;
      src.x = x1;
      src.y = y1;
      src.w = x2 - x1;
      src.h = y2 - y1;

      //center.x = (26 -(y2-y1))/2;
      //center.y = (26 -(x2-x1))/2;

      SDL_BlitSurface(img, &src, new_img,NULL);// &center);

      resized_img = SDL_CreateRGBSurface(0, 26, 26, 32, 0, 0, 0, 0);


      //Decoupe horizontale => Faire la découpe pour garder juste le caractère
      // + Plein de trucs a free

      SDL_Rect square;

      int max = (((src.w)>(src.h))?(src.w):(src.h));
      square.w = max;
      square.h = max;
      square.x = max/2-src.w/2; // On centre bien l'image.
      square.y = max/2-src.h/2;

      center_img = SDL_CreateRGBSurface(0, max, max, 32, 0, 0, 0, 0);
      // On remplit les contours en blanc
      SDL_FillRect(center_img, NULL, SDL_MapRGB(center_img->format, 255, 255, 255));

      SDL_BlitSurface(new_img,NULL,center_img,&square);

      resize(center_img, resized_img); // fonction pour scale en 26 26

      SDL_FreeSurface(new_img);
      SDL_FreeSurface(center_img);
    /*}
    else{
      SDL_Rect src, center;
      SDL_Surface *unresized_img;
      unresized_img = SDL_CreateRGBSurface(0, x2-x1, y2-y1, 32, 0, 0, 0, 0);

      src.x = x1;
      src.y = y1;
      src.w = x2 - x1;
      src.h = y2 - y1;

      SDL_BlitSurface(img, &src, unresized_img, NULL);
      SDL_Surface *resized_img = img_resizing(unresized_img, x2 - x1, y2 - y1);

      center.x = (26 -(resized_img->h))/2;
      center.y = (26 -(resized_img->w))/2;

      SDL_BlitSurface(resized_img, NULL, new_img, &center);
    }*/
    imgs[index_imgs] = resized_img;
    index_imgs += 1;
    cmpt += 1;
    index += 4;
  }
}

void resize(SDL_Surface *src, SDL_Surface *dest) {
  SDL_Rect final_size;
  final_size.w = 26;
  final_size.h = 26;
  final_size.x = 0;
  final_size.y = 0;
  SDL_BlitScaled(src, NULL, dest, &final_size);
}
