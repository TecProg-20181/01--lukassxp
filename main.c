#include <stdio.h>
#include <string.h>

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    Pixel pixel[512][512];
    unsigned int w;
    unsigned int h;
    int max_scale;
    char type[4];
} Image;

int max(int a, int b);
int min(int a, int b);
int is_equal_pixel(Pixel p1, Pixel p2);
Image gray_scale(Image img);
Image blur(Image img, int size);
Image rotate_to_right(Image img);
Image colors_reversed(Image img);
Image image_cut(Image img, int x, int y, int w, int h);
Image read_image(Image img);
Image sepia_filter(Image img);
Image mirroring(Image img, int horizontal);
void record_image(Image img);

int main() {
    int option, n_options, size, rotations, horizontal;

    Image img = read_image(img);

    scanf("%d", &n_options);

    for(int i = 0; i < n_options; ++i) {
        scanf("%d", &option);

        switch(option) {
            case 1: { // Escala de Cinza
                img = gray_scale(img);
                break;
            }
            case 2: { // Filtro Sepia
                img = sepia_filter(img);
                break;
            }
            case 3: { // Blur
                scanf("%d", &size);
                img = blur(img, size);
                break;
            }
            case 4: { // Rotacao
                scanf("%d", &rotations);
                rotations %= 4;
                for (int j = 0; j < rotations; ++j) {
                    img = rotate_to_right(img);
                }
                break;
            }
            case 5: { // Espelhamento
                scanf("%d", &horizontal);
                img = mirroring(img, horizontal);
                break;
            }
            case 6: { // Inversao de Cores
                img = colors_reversed(img);
                break;
            }
            case 7: { // Cortar Imagem
                int x, y;
                scanf("%d %d", &x, &y);
                int w, h;
                scanf("%d %d", &w, &h);

                img = image_cut(img, x, y, w, h);
                break;
            }
        }

    }
    record_image(img);

    return 0;
}

Image read_image(Image img){
  // read type of image
  scanf("%s", img.type);

  // read width height and color of image
  scanf("%u %u %d", &img.w, &img.h, &img.max_scale);

  // read all pixels of image
  for (unsigned int i = 0; i < img.h; ++i) {
      for (unsigned int j = 0; j < img.w; ++j) {
          scanf("%hu %hu %hu", &img.pixel[i][j].r,
                               &img.pixel[i][j].g,
                               &img.pixel[i][j].b);
      }
  }
  return img;
}

Image sepia_filter(Image img){
  Pixel pixel;
  int p, min_r;

  for (unsigned int x = 0; x < img.h; ++x) {
      for (unsigned int j = 0; j < img.w; ++j) {
          pixel.r = img.pixel[x][j].r;
          pixel.g = img.pixel[x][j].g;
          pixel.b = img.pixel[x][j].b;

          p =  pixel.r * .393 + pixel.g * .769 + pixel.b * .189;
          min_r = min(255, p);
          img.pixel[x][j].r = min_r;

          p =  pixel.r * .349 + pixel.g * .686 + pixel.b * .168;
          min_r = min(255, p);
          img.pixel[x][j].g = min_r;

          p =  pixel.r * .272 + pixel.g * .534 + pixel.b * .131;
          min_r = min(255, p);
          img.pixel[x][j].b = min_r;
      }
  }
  return img;
}

Image mirroring(Image img, int horizontal){
  int w = img.w, h = img.h, x, y;
  Pixel aux;

  if (horizontal == 1) w /= 2;
  else h /= 2;

  for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; ++j) {
          x = i, y = j;

          if (horizontal == 1) y = img.w - 1 - j;
          else x = img.h - 1 - i;

          aux.r = img.pixel[i][j].r;
          aux.g = img.pixel[i][j].g;
          aux.b = img.pixel[i][j].b;

          img.pixel[i][j].r = img.pixel[x][y].r;
          img.pixel[i][j].g = img.pixel[x][y].g;
          img.pixel[i][j].b = img.pixel[x][y].b;

          img.pixel[x][y].r = aux.r;
          img.pixel[x][y].g = aux.g;
          img.pixel[x][y].b = aux.b;
      }
  }
  return img;
}

void record_image(Image img){
  // print type of image
  printf("%s\n", img.type);

  // print width height and color of image
  printf("%u %u\n%d\n", img.w, img.h, img.max_scale);

  // print pixels of image
  for (unsigned int i = 0; i < img.h; ++i) {
      for (unsigned int j = 0; j < img.w; ++j) {
          printf("%hu %hu %hu ", img.pixel[i][j].r,
                                 img.pixel[i][j].g,
                                 img.pixel[i][j].b);
      }
      printf("\n");
  }
  return;
}

int is_equal_pixel(Pixel p1, Pixel p2) {
    if (p1.r == p2.r &&
        p1.g == p2.g &&
        p1.b == p2.b)
        return 1;
    return 0;
}

Image gray_scale(Image img) {
    int average;

    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            average = img.pixel[i][j].r +
                      img.pixel[i][j].g +
                      img.pixel[i][j].b;

            average /= 3;

            img.pixel[i][j].r = average;
            img.pixel[i][j].g = average;
            img.pixel[i][j].b = average;
        }
    }
    return img;
}

int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

int min(int a, int b) {
    if (a > b)
        return b;
    return a;
}

Image blur(Image img, int size) {
    int min_h, min_w;
    //Pixel average;

    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            Pixel average = {0, 0, 0};

            min_h = min(img.h - 1, i + size/2);
            min_w = min(img.w - 1, j + size/2);

            for(int x = max(0, i - size/2); x <= min_h; ++x) {
                for(int y = max(0, j - size/2); y <= min_w; ++y) {
                    average.r += img.pixel[x][y].r;
                    average.g += img.pixel[x][y].g;
                    average.b += img.pixel[x][y].b;
                }
            }

            average.r /= size * size;
            average.g /= size * size;
            average.b /= size * size;

            img.pixel[i][j].r = average.r;
            img.pixel[i][j].g = average.g;
            img.pixel[i][j].b = average.b;
        }
    }
    return img;
}

Image rotate_to_right(Image img) {
    Image rotated;

    rotated.w = img.h;
    rotated.h = img.w;
    strcpy(rotated.type, img.type);
    rotated.max_scale = img.max_scale;

    for (unsigned int i = 0, y = 0; i < rotated.h; ++i, ++y) {
        for (int j = rotated.w - 1, x = 0; j >= 0; --j, ++x) {
            rotated.pixel[i][j].r = img.pixel[x][y].r;
            rotated.pixel[i][j].g = img.pixel[x][y].g;
            rotated.pixel[i][j].b = img.pixel[x][y].b;
        }
    }

    return rotated;
}

Image colors_reversed(Image img) {
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            img.pixel[i][j].r = 255 - img.pixel[i][j].r;
            img.pixel[i][j].g = 255 - img.pixel[i][j].g;
            img.pixel[i][j].b = 255 - img.pixel[i][j].b;
        }
    }
    return img;
}

Image image_cut(Image img, int x, int y, int w, int h) {
    Image cropped;

    cropped.w = w;
    cropped.h = h;

    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            cropped.pixel[i][j].r = img.pixel[i + y][j + x].r;
            cropped.pixel[i][j].g = img.pixel[i + y][j + x].g;
            cropped.pixel[i][j].b = img.pixel[i + y][j + x].b;
        }
    }

    return cropped;
}
