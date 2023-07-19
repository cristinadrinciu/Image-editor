#include "struct.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void LOAD_gray(image * img, char type[], FILE * in);
void LOAD_RGB(image *img, char type[], FILE *in);
void EXIT(image *img);
void free_gray(image *img);
void free_rgb(image *img);
void LOAD(image *img, char file[]);
void SAVE_binary(image *img, char file[]);
void SAVE_ASCII(image *img, char file[]);
void SAVE(image *img, char file[], int ascii);
void SELECT(image *img, int x1, int x2, int y1, int y2);
void SELECT_ALL(image *img);
void CROP_gray(image *img);
void CROP_RGB(image *img);
void CROP(image *img);
void EQUALIZE(image *img);
int power2(int x);
void HISTOGRAM(image *img, int x, int y);
void EDGE(image *img);
void SHARPEN(image *img);
void BLUR(image *img);
void GAUSSIAN_BLUR(image *img);
void rotate_90_right_gray(image *img);
void rotate_90_right_select_gray(image *img);
void rotate_90_right_rgb(image *img);
void rotate_90_right_select_rgb(image *img);
void rotate_180_right_gray(image *img);
void rotate_180_right_select_gray(image *img);
void rotate_180_right_select_rgb(image *img);
void rotate_180_right_rgb(image *img);
void rotate_90_left_gray(image *img);
void rotate_90_left_select_gray(image *img);
void rotate_90_left_rgb(image *img);
void rotate_90_left_select_rgb(image *img);
void ROTATE(image *img, int angle);
