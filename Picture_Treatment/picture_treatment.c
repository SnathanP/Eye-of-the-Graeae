#include "pixel_operations.h"
#include "picture_treatment.h"

/*
print_array -> int h[]:
print the different value of a array h in which the index 0 contain
the nb of value.
*/
void print_array(int h[]){
  for(int i = 1; i < h[0]; i++)
    printf(" h[%d] = %d /", i, h[i]);
}

/*
sort_array_Uint8_spe -> Uint8 l[],
sort a array l of type Uint8 in which the index 0 contain the nb of value.
*/
void sort_array_Uint8_spe(Uint8 l[]){
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
void neighbors(SDL_Surface *img, int  x, int  y, Uint8 l[], int dneight){
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
void surf_to_array(SDL_Surface *img, int l[]){
  for(int i = 0; i < img->h; i++)
    for(int j = 0; j < img->w; j++)
      l[j + i * img->w] = get_light(img, j, i);
}

/*
array_to_surf -> int l[], SDL_Surface *img:
apply a matrix representing an image to an surface img with the same format. 
*/
SDL_Surface* array_to_surf(int l[], SDL_Surface *img){
  for(int i = 0; i < img->h; i++){
    for(int j = 0; j < img->w; j++){
      Uint8 rgb = l[j + i * img->w];
      putpixel(img, j, i, SDL_MapRGB(img->format, rgb, rgb, rgb));
    }
  }
  return img;
}

SDL_Surface* color_zone(SDL_Surface *img, int l[]){
  int limg[img->w * img->h];
  surf_to_array(img, limg);
  int index = 1;
  for(int k = 0; k < l[0]; k++){
    int x1 = l[index], x2 = l[index+1], y1 = l[index+2], y2 = l[index+3];
    printf("\n(%d,%d) to (%d,%d)\n", x1, y1, x2, y2);
    for(int i = x1; i < x2; i++){
      for(int j = y1; j < y2; j++){
        limg[j + i * img->w] = limg[j + i * img->w] == 255 ? 255 : 127;
      }
    }
    index += 4;
  }
  return array_to_surf(limg, img);
}

void h_cut_array(SDL_Surface *img, int l[], int l_c[]){
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
      l_c[index] = i;
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
  for(int i = 1; i < l_c[0]*2; i++){
    while(l_c[i+1] - l_c[i] < 5){
      l_c[i+1] = l_c[l_c[0]*2];
      l_c[i] = l_c[l_c[0]*2-1];
      l_c[0] -= 1;
    }
    i++;
  }
}

void w_cut_array(int l[], int weight, int height, int l_c[]){
  l_c[0] = 0;
  int index = 1, box_engaged = 0;
  for(int i = 0; i < weight; i++){
    int sum = 0;
    for(int j = 0; j < height && !sum; j++){
      sum = l[i + j * weight] == 255 ? 0 : 1;
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
    l_c[index] = weight - 1;
    l_c[0] += 1;
  }
  l_c[0] = l_c[0]/2;
  for(int i = 1; i < l_c[0]*2; i++){
    while(l_c[i+1] - l_c[i] < 5){
      l_c[i+1] = l_c[l_c[0]*2];
      l_c[i] = l_c[l_c[0]*2-1];
      l_c[0] -= 1;
    }
    i++;
  }
}
  
/*
cut -> SDL_Surface *img, int array_coords[]:
put in array_coords the section of the image where it detect letters.
*/
void cut(SDL_Surface *img,int array_coords[]){
  int copy[img->w * img->h];
  surf_to_array(img, copy);
  int l_c_h[img->h + 1]; 
  array_coords[0] = 0;
  h_cut_array(img, copy, l_c_h);
  int index_lch = 1, index_lcw = 1;
  for(int i = 0; i < l_c_h[0]; i++){
    int x = l_c_h[index_lch], y = l_c_h[index_lch + 1];
    index_lch += 2;
    int l_tmp[(y-x) * img->w];
    for(int j = x * img->w; j <= y * img->w; j++)
      l_tmp[j - x * img->w] = copy[j];
    int l_c_w[img->w + 1];
    w_cut_array(l_tmp, img->w, y-x, l_c_w);
    for(int j = 1; j <= l_c_w[0]; j++){
      array_coords[index_lcw] = x;
      array_coords[index_lcw+1] = y;
      array_coords[index_lcw+2] = l_c_w[j*2-1];
      array_coords[index_lcw+3] = l_c_w[j*2];
      array_coords[0] += 1;
      index_lcw += 4;
    }
  }
}

void Sobel_horizontal(SDL_Surface *img, int l[]){
  int copy[img->w * img->h];
  surf_to_array(img, copy);
  for(int i = 0; i < img->h; i++){
    l[i * img->w] = copy[i * img->w];
    for(int j = 1; j < img->w; j++){
      int index = j + i * img->w;
      l[index] = copy[index] != copy[index -1] ? 1 : 0;
    }
  }
}

void Sobel_vertical(SDL_Surface *img, int l[]){
  int copy[img->w * img->h];
  surf_to_array(img, copy);
  for(int i = 0; i < img->w; i++){
    l[i] = copy[i];
    for(int j = 1; j < img->h; j++){
      int index = i + j * img->w;
      l[index] = copy[index] != copy[index -1] ? 1 : 0;
    }
  }
}

SDL_Surface* Sobel_filter(SDL_Surface* img){
  int l_h[img->h * img->w], l_v[img->h * img->w];
  Sobel_horizontal(img, l_h);
  Sobel_vertical(img, l_v);
  int new_img[img->h * img->w];
  for(int i = 0; i < img->h; i++){
    for(int j = 0; j < img->w; j++){
      int index = j + i * img->w;
      new_img[index] = l_h[index] || l_v[index] ? 0 : 255;
    }
  }
  return array_to_surf(new_img, img);
}

/*
clean_img -> SDL_Surface *img:
reset to 0 all the pixel wich do not have at least 3 black pixels
in his neighborhood
*/
SDL_Surface* clean_img(SDL_Surface *img){
  int l[img->w * img->h];
  surf_to_array(img, l);
  for(int i = 0; i < img->h; i++){
    for(int j = 0; j < img->w; j++){
      Uint8 neight[10];
      neighbors(img, i, j, neight, 1);
      int sum = 0;
      for(Uint8 k = 1; k < neight[0]; k++)
        sum += neight[k];
      if(sum > 1020)
        l[j + i * img->w] = 255;
    }
  }
  return array_to_surf(l, img);
}

int moy_all(int l[], int size){
  long sum = 0;
  for(int i = 0; i < size; i++){
    sum += l[i]; 
  }
  return (int)((double)sum / size);    
}

/*
median -> SDL_Surface *img:
replace all the pixel by the median value among the value of his neightbors and his. 
*/
SDL_Surface* median(SDL_Surface *img){
  int l[img->w * img->h];
  surf_to_array(img, l);
  for(int i = 0; i < img->h; i++){
    for(int j = 0; j < img->w; j++){
      Uint8 neight[81];
      for(int k = 0; k < 81; k++)
        neight[k] = 0;
      neighbors(img, i, j, neight, 1);
      Uint8 rgb = neight[neight[0]/2 + 1];
      l[j + i * img->w] = rgb;
    }
  }
  return array_to_surf(l, img);   
}

/*
Grey_scale -> SDL_Surface *img:
return img after applying a greyscale on each pixel of img.
 */
SDL_Surface* Gray_scale(SDL_Surface *img){
  for(int i = 0;i < img->w;i++){
    for(int j = 0;j < img->h;j++){
      Uint32 p = getpixel(img, i, j);
      Uint8 r, g, b;
      SDL_GetRGB(p, img->format, &r, &g, &b);
      Uint8 moy = (r + g + b) / 3;
      putpixel(img, i, j, SDL_MapRGB(img->format, moy, moy, moy));
    }
  }
  return img;
}

/*
Contrast -> SDL_Surface *img:
upgrade the contrast of img and return img.
 */
SDL_Surface* Contrast(SDL_Surface *img){
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
  return img;
}

/*
threshold -> SDL_Surface *img:
calculate the average of the brightness of each pixel. Every pixel below average
become black, others become white. 
*/
SDL_Surface* threshold(SDL_Surface *img){
  int copy[img->w * img->h];
  surf_to_array(img, copy);
  int n = moy_all(copy, img->h * img->h);
  for(int i = 0; i < img->w * img->h; i++)
    copy[i] = copy[i] < n ? 0 : 255;
  return array_to_surf(copy, img);
}
