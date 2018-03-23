#include <stdio.h>

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    Pixel pixel[512][512];
    unsigned int w;
    unsigned int h;
} Image;

int max(int a, int b);
int is_equal_pixel(Pixel p1, Pixel p2);
Image gray_scale(Image img);
Image blur(Image img, int T);
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
  int max_color;
  char p3[4];

  // read type of image
  scanf("%s", p3);

  // read width height and color of image
  scanf("%u %u %d", &img.w, &img.h, &max_color);

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
  for (unsigned int x = 0; x < img.h; ++x) {
      for (unsigned int j = 0; j < img.w; ++j) {
          unsigned short int pixel[3];
          pixel[0] = img.pixel[x][j].r;
          pixel[1] = img.pixel[x][j].g;
          pixel[2] = img.pixel[x][j].b;

          int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
          int menor_r = (255 >  p) ? p : 255;
          img.pixel[x][j].r = menor_r;

          p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
          menor_r = (255 >  p) ? p : 255;
          img.pixel[x][j].g = menor_r;

          p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
          menor_r = (255 >  p) ? p : 255;
          img.pixel[x][j].b = menor_r;
      }
  }
  return img;
}

Image mirroring(Image img, int horizontal){
  int w = img.w, h = img.h, x, y;
  Pixel aux1;

  if (horizontal == 1) w /= 2;
  else h /= 2;

  for (int i2 = 0; i2 < h; ++i2) {
      for (int j = 0; j < w; ++j) {
          x = i2, y = j;

          if (horizontal == 1) y = img.w - 1 - j;
          else x = img.h - 1 - i2;

          aux1.r = img.pixel[i2][j].r;
          aux1.g = img.pixel[i2][j].g;
          aux1.b = img.pixel[i2][j].b;

          img.pixel[i2][j].r = img.pixel[x][y].r;
          img.pixel[i2][j].g = img.pixel[x][y].g;
          img.pixel[i2][j].b = img.pixel[x][y].b;

          img.pixel[x][y].r = aux1.r;
          img.pixel[x][y].g = aux1.g;
          img.pixel[x][y].b = aux1.b;
      }
  }
  return img;
}

void record_image(Image img){
  // print type of image
  printf("P3\n");
  // print width height and color of image
  printf("%u %u\n255\n", img.w, img.h);

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

int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

int is_equal_pixel(Pixel p1, Pixel p2) {
    if (p1.r == p2.r &&
        p1.g == p2.g &&
        p1.b == p2.b)
        return 1;
    return 0;
}


Image gray_scale(Image img) {
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            int media = img.pixel[i][j].r +
                        img.pixel[i][j].g +
                        img.pixel[i][j].b;
            media /= 3;
            img.pixel[i][j].r = media;
            img.pixel[i][j].g = media;
            img.pixel[i][j].b = media;
        }
    }

    return img;
}

Image blur(Image img, int T) {
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            Pixel media = {0, 0, 0};

            int menor_h = (img.h - 1 > i + T/2) ? i + T/2 : img.h - 1;
            int min_w = (img.w - 1 > j + T/2) ? j + T/2 : img.w - 1;

            for(int x = (0 > i - T/2 ? 0 : i - T/2); x <= menor_h; ++x) {
                for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= min_w; ++y) {
                    media.r += img.pixel[x][y].r;
                    media.g += img.pixel[x][y].g;
                    media.b += img.pixel[x][y].b;
                }
            }

            // printf("%u", media.r)
            media.r /= T * T;
            media.g /= T * T;
            media.b /= T * T;

            img.pixel[i][j].r = media.r;
            img.pixel[i][j].g = media.g;
            img.pixel[i][j].b = media.b;
        }
    }

    return img;
}

Image rotate_to_right(Image img) {
    Image rotacionada;

    rotacionada.w = img.h;
    rotacionada.h = img.w;

    for (unsigned int i = 0, y = 0; i < rotacionada.h; ++i, ++y) {
        for (int j = rotacionada.w - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel[i][j].r = img.pixel[x][y].r;
            rotacionada.pixel[i][j].g = img.pixel[x][y].g;
            rotacionada.pixel[i][j].b = img.pixel[x][y].b;
        }
    }

    return rotacionada;
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
    Image cortada;

    cortada.w = w;
    cortada.h = h;

    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            cortada.pixel[i][j].r = img.pixel[i + y][j + x].r;
            cortada.pixel[i][j].g = img.pixel[i + y][j + x].g;
            cortada.pixel[i][j].b = img.pixel[i + y][j + x].b;
        }
    }

    return cortada;
}
