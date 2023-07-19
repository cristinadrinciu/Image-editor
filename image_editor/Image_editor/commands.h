#include <string.h>
#include <stdlib.h>
#include "image_editor.h"

int nr_tokens(char command[]);
int is_number(char *p);
void call_LOAD(image *img, char command[]);
void call_SAVE(image *img, char command[]);
void call_SELECT(image *img, char command[]);
void call_SELECT_ALL(image *img);
void call_EXIT(image *img);
void call_EQUALIZE(image *img);
void call_CROP(image *img);
void call_ROTATE(image *img, char command[]);
void call_HISTOGRAM(image *img, char command[]);
void call_APPLY_SHARPEN(image *img);
void call_APPLY_EDGE(image *img);
void call_APPLY_BLUR(image *img);
void call_APPLY_GAUSSIAN_BLUR(image *img);
int verif_LOAD(char command[]);
int verif_SAVE(char command[]);
int verif_EXIT(char command[]);
int verif_CROP(char command[]);
int verif_SELECT_ALL(char command[]);
int verif_SELECT(char command[]);
int verif_EQUALIZE(char command[]);
int verif_SHARPEN(char command[]);
int verif_EDGE(char command[]);
int verif_BLUR(char command[]);
int verif_GAUSSIAN_BLUR(char command[]);
int verif_HISTOGRAM(char command[]);
int APPLY_NO_LOADED(char command[], image *img);
int HISTOGRAM_NO_LOADED(char command[], image *img);
int APPLY_LOADED(char command[], image *img);
int invalid_APPLY(char command[]);
int verif_ROTATE(char command[]);
