#include "commands.h"

int nr_tokens(char command[])
{
	char aux[50];
	strcpy(aux, command);
	char *p;
	int nr = 0;
	p = strtok(aux, "\n ");
	while (p) {
		nr++;
		p = strtok(NULL, "\n ");
	}
	return nr;
}

int is_number(char *p)
{
	int nr = atoi(p);
	if (strcmp(p, "0") != 0 && nr == 0)
		return 0;
	return 1;
}

int verif_LOAD(char command[])
{
	if (nr_tokens(command) != 2)
		return 0;
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	if (strcmp(p, "LOAD") == 0)
		return 1;
	return 0;
}

int verif_SAVE(char command[])
{
	if (nr_tokens(command) != 2 && nr_tokens(command) != 3)
		return 0;
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	if (strcmp(p, "SAVE") == 0)
		return 1;
	return 0;
}

int verif_EXIT(char command[])
{
	if (nr_tokens(command) != 1)
		return 0;
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	if (strcmp(p, "EXIT") == 0)
		return 1;
	return 0;
}

int verif_CROP(char command[])
{
	if (nr_tokens(command) != 1)
		return 0;
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	if (strcmp(p, "CROP") == 0)
		return 1;
	return 0;
}

int verif_SELECT_ALL(char command[])
{
	if (nr_tokens(command) != 2)
		return 0;
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	if (strcmp(p, "SELECT"))
		return 0;
	p = strtok(NULL, "\n ");
	if (strcmp(p, "ALL") == 0)
		return 1;
	return 0;
}

int verif_SELECT(char command[])
{
	if (nr_tokens(command) != 5)
		return 0;
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	if (strcmp(p, "SELECT") == 0)
		return 1;
	return 0;
}

int verif_EQUALIZE(char command[])
{
	if (nr_tokens(command) != 1)
		return 0;
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	if (strcmp(p, "EQUALIZE") == 0)
		return 1;
	return 0;
}

int verif_SHARPEN(char command[])
{
	if (nr_tokens(command) != 2)
		return 0;
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	if (strcmp(p, "APPLY"))
		return 0;
	p = strtok(NULL, "\n ");
	if (strcmp(p, "SHARPEN") == 0)
		return 1;
	return 0;
}

int verif_EDGE(char command[])
{
	if (nr_tokens(command) != 2)
		return 0;
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	if (strcmp(p, "APPLY"))
		return 0;
	p = strtok(NULL, "\n ");
	if (strcmp(p, "EDGE") == 0)
		return 1;
	return 0;
}

int verif_BLUR(char command[])
{
	if (nr_tokens(command) != 2)
		return 0;
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	if (strcmp(p, "APPLY"))
		return 0;
	p = strtok(NULL, "\n ");
	if (strcmp(p, "BLUR") == 0)
		return 1;
	return 0;
}

int verif_GAUSSIAN_BLUR(char command[])
{
	if (nr_tokens(command) != 2)
		return 0;
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	if (strcmp(p, "APPLY"))
		return 0;
	p = strtok(NULL, "\n ");
	if (strcmp(p, "GAUSSIAN_BLUR") == 0)
		return 1;
	return 0;
}

int verif_HISTOGRAM(char command[])
{
	if (nr_tokens(command) != 3)
		return 0;
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	if (strcmp(p, "HISTOGRAM") == 0)
		return 1;
	return 0;
}

int APPLY_NO_LOADED(char command[], image *img)
{
	if (nr_tokens(command) != 1)
		return 0;
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	if (strcmp(p, "APPLY") == 0 && img->loaded == 0)
		return 1;
	return 0;
}

int HISTOGRAM_NO_LOADED(char command[], image *img)
{
	if (nr_tokens(command) != 1)
		return 0;
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	if (strcmp(p, "HISTOGRAM") == 0 && img->loaded == 0)
		return 1;
	return 0;
}

int APPLY_LOADED(char command[], image *img)
{
	if (nr_tokens(command) != 1)
		return 0;
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	if (strcmp(p, "APPLY") == 0 && img->loaded)
		return 1;
	return 0;
}

int invalid_APPLY(char command[])
{
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	if (strcmp(p, "APPLY"))
		return 0;
	p = strtok(NULL, "\n ");
	if (strcmp(p, "GAUSSIAN_BLUR") && strcmp(p, "BLUR") &&
		strcmp(p, "SHARPEN") && strcmp(p, "EDGE"))
		return 1;
	return 0;
}

int verif_ROTATE(char command[])
{
	if (nr_tokens(command) != 2)
		return 0;
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	if (strcmp(p, "ROTATE") == 0)
		return 1;
	return 0;
}

void call_LOAD(image *img, char command[])
{
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	p = strtok(NULL, "\n ");  // the file
	char file[50];
	strcpy(file, p);
	img->loaded = 0;
	LOAD(img, file);
}

void call_SAVE(image *img, char command[])
{
	char aux[50];
	int ascii = 0;
	if (nr_tokens(command) == 3)
		ascii = 1;  // save ascii
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	p = strtok(NULL, "\n ");  // the file
	char file[50];
	strcpy(file, p);
	SAVE(img, file, ascii);
}

void call_SELECT(image *img, char command[])
{
	char aux[50];
	strcpy(aux, command);
	char *p;
	// get the coordinates
	int x1, x2, y1, y2;
	p = strtok(aux, "\n ");
	p = strtok(NULL, "\n ");
	if (is_number(p) == 0) {
		printf("Invalid command\n");
		return;
	}
	x1 = atoi(p);
	p = strtok(NULL, "\n ");
	if (is_number(p) == 0) {
		printf("Invalid command\n");
		return;
	}
	y1 = atoi(p);
	p = strtok(NULL, "\n ");
	if (is_number(p) == 0) {
		printf("Invalid command\n");
		return;
	}
	x2 = atoi(p);
	p = strtok(NULL, "\n ");
	if (is_number(p) == 0) {
		printf("Invalid command\n");
		return;
	}
	y2 = atoi(p);
	SELECT(img, x1, x2, y1, y2);
}

void call_SELECT_ALL(image *img)
{
	SELECT_ALL(img);
}

void call_EXIT(image *img)
{
	EXIT(img);
}

void call_EQUALIZE(image *img)
{
	EQUALIZE(img);
}

void call_CROP(image *img)
{
	CROP(img);
}

void call_ROTATE(image *img, char command[])
{
	char aux[50];
	strcpy(aux, command);
	char *p;
	p = strtok(aux, "\n ");
	p = strtok(NULL, "\n ");  // the file
	int angle = atoi(p);
	ROTATE(img, angle);
}

void call_HISTOGRAM(image *img, char command[])
{
	char aux[50];
	strcpy(aux, command);
	char *p;
	// get the coordinates
	int x, y;
	p = strtok(aux, "\n ");
	p = strtok(NULL, "\n ");
	if (is_number(p) == 0) {
		printf("Invalid command\n");
		return;
	}
	x = atoi(p);
	p = strtok(NULL, "\n ");
	if (is_number(p) == 0) {
		printf("Invalid command\n");
		return;
	}
	y = atoi(p);
	HISTOGRAM(img, x, y);
}

void call_APPLY_SHARPEN(image *img)
{
	SHARPEN(img);
}

void call_APPLY_EDGE(image *img)
{
	EDGE(img);
}

void call_APPLY_BLUR(image *img)
{
	BLUR(img);
}

void call_APPLY_GAUSSIAN_BLUR(image *img)
{
	GAUSSIAN_BLUR(img);
}
