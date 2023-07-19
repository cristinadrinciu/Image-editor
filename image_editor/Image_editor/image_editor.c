#include "commands.h"

void LOAD_gray(image *img, char type[], FILE *in)
{
	strcpy(img->type, type);
	if (!strcmp(type, "P2")) {  // ASCII
		// read dimensions
		fscanf(in, "%d%d", &img->col, &img->row);
		// get the max val
		fscanf(in, "%d", &img->max_val);
		// alloc memory
		img->mat = malloc(img->row * sizeof(int *));
		for (int i = 0; i < img->row; i++)
			img->mat[i] = malloc(img->col * sizeof(int));
		// read values
		for (int i = 0; i < img->row; i++)
			for (int j = 0; j < img->col; j++)
				fscanf(in, "%d", &img->mat[i][j]);
	}
	if (!strcmp(type, "P5")) {  // binary
		// read dimensions
		fscanf(in, "%d%d", &img->col, &img->row);
		// get the max val
		fscanf(in, "%d", &img->max_val);
		// alloc memory
		img->mat = malloc(img->row * sizeof(int *));
		for (int i = 0; i < img->row; i++)
			img->mat[i] = malloc(img->col * sizeof(int));
		fseek(in, 1, SEEK_CUR);
		// read values
		unsigned char val;
		for (int i = 0; i < img->row; i++)
			for (int j = 0; j < img->col; j++) {
				fread(&val, sizeof(char), 1, in);
				img->mat[i][j] = (int)val;
			}
	}
	img->x1 = 0;
	img->y1 = 0;
	img->x2 = img->col;
	img->y2 = img->row;
	fclose(in);
}

void LOAD_RGB(image *img, char type[], FILE *in)
{
	strcpy(img->type, type);
	if (!strcmp(type, "P3")) {  // ASCII
		// read dimensions
		fscanf(in, "%d%d", &img->col, &img->row);
		// get the max val
		fscanf(in, "%d", &img->max_val);
		// alloc memory
		img->rgb = malloc(img->row * sizeof(RGB *));
		for (int i = 0; i < img->row; i++)
			img->rgb[i] = malloc(img->col * sizeof(RGB));
		// read the values
		for (int i = 0; i < img->row; i++)
			for (int j = 0; j < img->col; j++) {
				fscanf(in, "%d", &img->rgb[i][j].R);
				fscanf(in, "%d", &img->rgb[i][j].G);
				fscanf(in, "%d", &img->rgb[i][j].B);
			}
	}
	if (!strcmp(type, "P6")) {  // binary
		// read dimensions
		fscanf(in, "%d%d", &img->col, &img->row);
		// get the max val
		fscanf(in, "%d", &img->max_val);
		// alloc memory
		img->rgb = malloc(img->row * sizeof(RGB *));
		for (int i = 0; i < img->row; i++)
			img->rgb[i] = malloc(img->col * sizeof(RGB));
		// read the values
		fseek(in, 1, SEEK_CUR);
		for (int i = 0; i < img->row; i++)
			for (int j = 0; j < img->col; j++) {
				unsigned char val;
				fread(&val, sizeof(char), 1, in);
				img->rgb[i][j].R = (int)val;
				fread(&val, sizeof(char), 1, in);
				img->rgb[i][j].G = (int)val;
				fread(&val, sizeof(char), 1, in);
				img->rgb[i][j].B = (int)val;
			}
	}
	img->x1 = 0;
	img->y1 = 0;
	img->x2 = img->col;
	img->y2 = img->row;
	fclose(in);
}

void EXIT(image *img)
{
	if (img->loaded == 0) {
		printf("No image loaded\n");
		free(img);
		return;
	}
	// free the matrix
	if (img->mat) {
		// free the img->mat
		for (int i = 0; i < img->row; i++)
			free(img->mat[i]);
		free(img->mat);
	}
	if (img->rgb) {
		// free img->rgb
		for (int i = 0; i < img->row; i++)
			free(img->rgb[i]);
		free(img->rgb);
	}
	// free the image structure
	free(img);
}

void free_gray(image *img)
{
	for (int i = 0; i < img->row; i++)
		free(img->mat[i]);
	free(img->mat);
	img->mat = NULL;
}

void free_rgb(image *img)
{
	for (int i = 0; i < img->row; i++)
		free(img->rgb[i]);
	free(img->rgb);
	img->rgb = NULL;
}

void LOAD(image *img, char file[])
{
	// free the previous image if it exists
	if (img->mat)
		free_gray(img);
	if (img->rgb)
		free_rgb(img);
	// open the file
	FILE *in = fopen(file, "r");
	if (!in) {
		printf("Failed to load %s\n", file);
		return;
	}
	// read the type
	char type[5];
	img->mat = NULL;
	img->rgb = NULL;
	fgets(type, 5, in);
	type[2] = '\0';
	if (strcmp(type, "P2") == 0 || strcmp(type, "P5") == 0)
		// grayscale image
		LOAD_gray(img, type, in);
	else if (strcmp(type, "P3") == 0 || strcmp(type, "P6") == 0)
		// RGB image
		LOAD_RGB(img, type, in);
	img->loaded = 1;
	printf("Loaded %s\n", file);
}

void SAVE_binary(image *img, char file[])
{
	FILE *out = fopen(file, "wb");
	if (strcmp(img->type, "P5") == 0 || strcmp(img->type, "P2") == 0)
		fprintf(out, "P5\n");
	if (strcmp(img->type, "P6") == 0 || strcmp(img->type, "P3") == 0)
		fprintf(out, "P6\n");
	fprintf(out, "%d %d\n", img->col, img->row);
	fprintf(out, "%d\n", img->max_val);
	if (strcmp(img->type, "P5") == 0 ||
		strcmp(img->type, "P2") == 0) {  // grayscale
		for (int i = 0; i < img->row; i++)
			for (int j = 0; j < img->col; j++)
				fwrite(&img->mat[i][j], sizeof(char), 1, out);
	} else if (strcmp(img->type, "P6") == 0 ||
			strcmp(img->type, "P3") == 0) {  // RGB
		for (int i = 0; i < img->row; i++)
			for (int j = 0; j < img->col; j++) {
				fwrite(&img->rgb[i][j].R, sizeof(char), 1, out);
				fwrite(&img->rgb[i][j].G, sizeof(char), 1, out);
				fwrite(&img->rgb[i][j].B, sizeof(char), 1, out);
			}
	}
	fclose(out);
}

void SAVE_ASCII(image *img, char file[])
{
	FILE *out = fopen(file, "wt");
	if (!out) {
		printf("Error opening file.\n");
		return;
	}

	if (strcmp(img->type, "P5") == 0 || strcmp(img->type, "P2") == 0)
		fprintf(out, "P2\n");
	else if (strcmp(img->type, "P6") == 0 || strcmp(img->type, "P3") == 0)
		fprintf(out, "P3\n");

	fprintf(out, "%d %d\n", img->col, img->row);
	fprintf(out, "%d\n", img->max_val);

	if (strcmp(img->type, "P2") == 0 ||
		strcmp(img->type, "P5") == 0) {  // grayscale
		for (int i = 0; i < img->row; i++) {
			for (int j = 0; j < img->col; j++)
				fprintf(out, "%d ", img->mat[i][j]);
			fprintf(out, "\n");
		}
	} else if (strcmp(img->type, "P3") == 0 ||
			strcmp(img->type, "P6") == 0) {  // RGB
		for (int i = 0; i < img->row; i++) {
			for (int j = 0; j < img->col; j++) {
				fprintf(out, "%d ", img->rgb[i][j].R);
				fprintf(out, "%d ", img->rgb[i][j].G);
				fprintf(out, "%d ", img->rgb[i][j].B);
			}
			fprintf(out, "\n");
		}
	}
	fclose(out);
}

void SAVE(image *img, char file[], int ascii)
{
	if (img->loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	if (!ascii)
		// save binary
		SAVE_binary(img, file);
	else
		// save ASCII
		SAVE_ASCII(img, file);
	printf("Saved %s\n", file);
}

void SELECT(image *img, int x1, int x2, int y1, int y2)
{
	if (img->loaded == 0) {
		printf("No image loaded\n");
		return;
	}

	if (x1 > x2) {
		int aux;
		aux = x1;
		x1 = x2;
		x2 = aux;
	}
	if (y1 > y2) {
		int aux;
		aux = y1;
		y1 = y2;
		y2 = aux;
	}
	// verify if the coordinats are valid
	if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 || x1 == x2 || y1 == y2 ||
		x1 > img->col || x2 > img->col || y1 > img->row || y2 > img->row) {
		printf("Invalid set of coordinates\n");
		return;
	}
	img->x1 = x1;
	img->x2 = x2;
	img->y1 = y1;
	img->y2 = y2;
	printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
}

void SELECT_ALL(image *img)
{
	if (img->loaded == 0) {
		printf("No image loaded\n");
		return;
	}

	img->x1 = 0;
	img->y1 = 0;
	img->x2 = img->col;
	img->y2 = img->row;
	printf("Selected ALL\n");
}

void CROP_gray(image *img)
{
	// create aux matrix to put the selection
	int **aux = malloc((img->y2 - img->y1) * sizeof(int *));
	for (int i = 0; i < (img->y2 - img->y1); i++)
		aux[i] = malloc((img->x2 - img->x1) * sizeof(int));
	// atribute the values in the aux matrix
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			aux[i][j] = img->mat[i + img->y1][j + img->x1];
	// free the original matrix
	for (int i = 0; i < img->row; i++)
		free(img->mat[i]);
	free(img->mat);
	// modify the dimensions
	img->row = img->y2 - img->y1;
	img->col = img->x2 - img->x1;
	// alloc the needed space
	img->mat = malloc(img->row * sizeof(int *));
	for (int i = 0; i < img->row; i++)
		img->mat[i] = malloc(img->col * sizeof(int));
	// atribute the elements from the aux
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++)
			img->mat[i][j] = aux[i][j];
	// free the aux
	for (int i = 0; i < img->row; i++)
		free(aux[i]);
	free(aux);
	// modify the coordinate of the selection
	img->x1 = 0;
	img->y1 = 0;
	img->x2 = img->col;
	img->y2 = img->row;
}

void CROP_RGB(image *img)
{
	// create RGB aux
	RGB **aux = malloc((img->y2 - img->y1) * sizeof(RGB *));
	for (int i = 0; i < (img->y2 - img->y1); i++)
		aux[i] = malloc((img->x2 - img->x1) * sizeof(RGB));
	// atribute the values in the aux matrix
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			aux[i][j] = img->rgb[i + img->y1][j + img->x1];
	// free the original matrix
	for (int i = 0; i < img->row; i++)
		free(img->rgb[i]);
	free(img->rgb);
	// modify the dimensions
	img->row = img->y2 - img->y1;
	img->col = img->x2 - img->x1;
	// alloc the needed space
	img->rgb = malloc(img->row * sizeof(RGB *));
	for (int i = 0; i < img->row; i++)
		img->rgb[i] = malloc(img->col * sizeof(RGB));
	// atribute the values from the aux matrix
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			img->rgb[i][j] = aux[i][j];
	// free the aux
	for (int i = 0; i < img->row; i++)
		free(aux[i]);
	free(aux);
	// modify the coordinate of the selection
	img->x1 = 0;
	img->y1 = 0;
	img->x2 = img->col;
	img->y2 = img->row;
}

void CROP(image *img)
{
	if (img->loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	if (strcmp(img->type, "P2") == 0 || strcmp(img->type, "P5") == 0)
		CROP_gray(img);
	if (strcmp(img->type, "P3") == 0 || strcmp(img->type, "P6") == 0)
		CROP_RGB(img);
	printf("Image cropped\n");
}

void EQUALIZE(image *img)
{
	if (img->loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	if (strcmp(img->type, "P3") == 0 || strcmp(img->type, "P6") == 0) {
		printf("Black and white image needed\n");
		return;
	}
	// create an array of frequences for each pixel
	int *frequence = calloc(256, sizeof(int));
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++)
			frequence[img->mat[i][j]]++;
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++) {
			// calculat ethe sum of appearances of each pizel
			// up to the current pixel
			int S = 0;
			for (int k = 0; k <= img->mat[i][j]; k++)
				S += frequence[k];
			// apply the formula
			img->mat[i][j] =
				(int)round((double)255 * S / (img->row * img->col));
		}
	printf("Equalize done\n");
	free(frequence);
}

int power2(int x)
{
	while (x % 2 == 0)
		x /= 2;
	if (x == 1)
		return 1;
	return 0;
}

void HISTOGRAM(image *img, int x, int y)
{
	if (img->loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	if (strcmp(img->type, "P3") == 0 || strcmp(img->type, "P6") == 0) {
		printf("Black and white image needed\n");
		return;
	}
	if (power2(y) == 0 || y > 255 || y < 0) {
		printf("Invalid set of parameters\n");
		return;
	}
	// histogram array
	int *hist = calloc(y, sizeof(int));
	int group;
	group = 256 / y;  // calculate the number of groups for each bin
	// create an array with the number of appearances of each pixel in the image
	int *frequence = calloc(256, sizeof(int));
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++)
			frequence[img->mat[i][j]]++;
	int k = 0;
	int max_freq = 0;
	for (int i = 0; i < y; i++) {
		for (int j = k; j < (k + group); j++)
			hist[i] += frequence[j];
		if (hist[i] > max_freq)
			max_freq = hist[i];
		k += group;
	}
	// build the histogram by calculating the number of '*' fo each bin
	for (int i = 0; i < y; i++) {
		int number;
		number = (int)(1.0 * hist[i] * x / max_freq);
		printf("%d  |   ", number);
		for (int j = 0; j < number; j++)
			printf("*");
		printf("\n");
	}
	free(hist);
	free(frequence);
}

void EDGE(image *img)
{
	if (img->loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	if (strcmp(img->type, "P2") == 0 || strcmp(img->type, "P5") == 0) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}
	// create an aux matrix for calculus
	RGB **aux = malloc(img->row * sizeof(RGB *));
	for (int i = 0; i < img->row; i++)
		aux[i] = malloc(img->col * sizeof(RGB));
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++) {
			aux[i][j].R = img->rgb[i][j].R;
			aux[i][j].G = img->rgb[i][j].G;
			aux[i][j].B = img->rgb[i][j].B;
		}
	// work on the selection
	for (int i = img->y1; i < img->y2; i++)
		for (int j = img->x1; j < img->x2; j++) {
			if (!(i > 0 && j > 0 && j < img->col - 1 &&
				  i < img->row - 1))  // the edges
				continue;
			aux[i][j].R =
				img->rgb[i - 1][j - 1].R * (-1) + img->rgb[i - 1][j].R * (-1) +
				img->rgb[i - 1][j + 1].R * (-1) + img->rgb[i][j - 1].R * (-1) +
				img->rgb[i][j + 1].R * (-1) + img->rgb[i + 1][j - 1].R * (-1) +
				img->rgb[i + 1][j].R * (-1) + img->rgb[i + 1][j + 1].R * (-1) +
				img->rgb[i][j].R * 8;
			aux[i][j].G =
				img->rgb[i - 1][j - 1].G * (-1) + img->rgb[i - 1][j].G * (-1) +
				img->rgb[i - 1][j + 1].G * (-1) + img->rgb[i][j - 1].G * (-1) +
				img->rgb[i][j + 1].G * (-1) + img->rgb[i + 1][j - 1].G * (-1) +
				img->rgb[i + 1][j].G * (-1) + img->rgb[i + 1][j + 1].G * (-1) +
				img->rgb[i][j].G * 8;
			aux[i][j].B =
				img->rgb[i - 1][j - 1].B * (-1) + img->rgb[i - 1][j].B * (-1) +
				img->rgb[i - 1][j + 1].B * (-1) + img->rgb[i][j - 1].B * (-1) +
				img->rgb[i][j + 1].B * (-1) + img->rgb[i + 1][j - 1].B * (-1) +
				img->rgb[i + 1][j].B * (-1) + img->rgb[i + 1][j + 1].B * (-1) +
				img->rgb[i][j].B * 8;
			if (aux[i][j].R > 255)
				aux[i][j].R = 255;
			if (aux[i][j].R < 0)
				aux[i][j].R = 0;
			if (aux[i][j].G > 255)
				aux[i][j].G = 255;
			if (aux[i][j].G < 0)
				aux[i][j].G = 0;
			if (aux[i][j].B > 255)
				aux[i][j].B = 255;
			if (aux[i][j].B < 0)
				aux[i][j].B = 0;
		}
	// insert the edited selection in the og image
	for (int i = img->y1; i < img->y2; i++)
		for (int j = img->x1; j < img->x2; j++) {
			img->rgb[i][j].R = aux[i][j].R;
			img->rgb[i][j].G = aux[i][j].G;
			img->rgb[i][j].B = aux[i][j].B;
		}
	// free the aux
	for (int i = 0; i < img->row; i++)
		free(aux[i]);
	free(aux);
	printf("APPLY EDGE done\n");
}

void SHARPEN(image *img)
{
	if (img->loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	if (strcmp(img->type, "P2") == 0 || strcmp(img->type, "P5") == 0) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}
	// create an aux matrix for calculus
	RGB **aux = malloc(img->row * sizeof(RGB *));
	for (int i = 0; i < img->row; i++)
		aux[i] = malloc(img->col * sizeof(RGB));
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++) {
			aux[i][j].R = img->rgb[i][j].R;
			aux[i][j].G = img->rgb[i][j].G;
			aux[i][j].B = img->rgb[i][j].B;
		}
	// work on the selection
	for (int i = img->y1; i < img->y2; i++)
		for (int j = img->x1; j < img->x2; j++) {
			if (!(i > 0 && j > 0 && j < img->col - 1 &&
				  i < img->row - 1))  // the edges
				continue;
			aux[i][j].R =
				img->rgb[i - 1][j - 1].R * 0 + img->rgb[i - 1][j].R * (-1) +
				img->rgb[i - 1][j + 1].R * 0 + img->rgb[i][j - 1].R * (-1) +
				img->rgb[i][j + 1].R * (-1) + img->rgb[i + 1][j - 1].R * 0 +
				img->rgb[i + 1][j].R * (-1) + img->rgb[i + 1][j + 1].R * 0 +
				img->rgb[i][j].R * 5;
			aux[i][j].G =
				img->rgb[i - 1][j - 1].G * 0 + img->rgb[i - 1][j].G * (-1) +
				img->rgb[i - 1][j + 1].G * 0 + img->rgb[i][j - 1].G * (-1) +
				img->rgb[i][j + 1].G * (-1) + img->rgb[i + 1][j - 1].G * 0 +
				img->rgb[i + 1][j].G * (-1) + img->rgb[i + 1][j + 1].G * 0 +
				img->rgb[i][j].G * 5;
			aux[i][j].B =
				img->rgb[i - 1][j - 1].B * 0 + img->rgb[i - 1][j].B * (-1) +
				img->rgb[i - 1][j + 1].B * 0 + img->rgb[i][j - 1].B * (-1) +
				img->rgb[i][j + 1].B * (-1) + img->rgb[i + 1][j - 1].B * 0 +
				img->rgb[i + 1][j].B * (-1) + img->rgb[i + 1][j + 1].B * 0 +
				img->rgb[i][j].B * 5;
			if (aux[i][j].R > 255)
				aux[i][j].R = 255;
			if (aux[i][j].R < 0)
				aux[i][j].R = 0;
			if (aux[i][j].G > 255)
				aux[i][j].G = 255;
			if (aux[i][j].G < 0)
				aux[i][j].G = 0;
			if (aux[i][j].B > 255)
				aux[i][j].B = 255;
			if (aux[i][j].B < 0)
				aux[i][j].B = 0;
		}
	// insert the edited selection in the og image
	for (int i = img->y1; i < img->y2; i++)
		for (int j = img->x1; j < img->x2; j++) {
			img->rgb[i][j].R = aux[i][j].R;
			img->rgb[i][j].G = aux[i][j].G;
			img->rgb[i][j].B = aux[i][j].B;
		}
	// free the aux
	for (int i = 0; i < img->row; i++)
		free(aux[i]);
	free(aux);
	printf("APPLY SHARPEN done\n");
}

void BLUR(image *img)
{
	if (img->loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	if (strcmp(img->type, "P2") == 0 || strcmp(img->type, "P5") == 0) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}
	// create an aux matrix for calculus
	RGB **aux = malloc(img->row * sizeof(RGB *));
	for (int i = 0; i < img->row; i++)
		aux[i] = malloc(img->col * sizeof(RGB));
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++) {
			aux[i][j].R = img->rgb[i][j].R;
			aux[i][j].G = img->rgb[i][j].G;
			aux[i][j].B = img->rgb[i][j].B;
		}
	// work on the selection
	for (int i = img->y1; i < img->y2; i++)
		for (int j = img->x1; j < img->x2; j++) {
			if (!(i > 0 && j > 0 && j < img->col - 1 &&
				  i < img->row - 1))  // the edges
				continue;
			aux[i][j].R = (img->rgb[i - 1][j - 1].R + img->rgb[i - 1][j].R +
						img->rgb[i - 1][j + 1].R + img->rgb[i][j - 1].R +
						img->rgb[i][j + 1].R + img->rgb[i + 1][j - 1].R +
						img->rgb[i + 1][j].R + img->rgb[i + 1][j + 1].R +
						img->rgb[i][j].R) /
						9;
			aux[i][j].G = (img->rgb[i - 1][j - 1].G + img->rgb[i - 1][j].G +
						img->rgb[i - 1][j + 1].G + img->rgb[i][j - 1].G +
						img->rgb[i][j + 1].G + img->rgb[i + 1][j - 1].G +
						img->rgb[i + 1][j].G + img->rgb[i + 1][j + 1].G +
						img->rgb[i][j].G) /
						9;
			aux[i][j].B = (img->rgb[i - 1][j - 1].B + img->rgb[i - 1][j].B +
						img->rgb[i - 1][j + 1].B + img->rgb[i][j - 1].B +
						img->rgb[i][j + 1].B + img->rgb[i + 1][j - 1].B +
						img->rgb[i + 1][j].B + img->rgb[i + 1][j + 1].B +
						img->rgb[i][j].B) /
						9;
		}
	// insert the edited selection in the og image
	for (int i = img->y1; i < img->y2; i++)
		for (int j = img->x1; j < img->x2; j++) {
			img->rgb[i][j].R = aux[i][j].R;
			img->rgb[i][j].G = aux[i][j].G;
			img->rgb[i][j].B = aux[i][j].B;
		}
	// free the aux
	for (int i = 0; i < img->row; i++)
		free(aux[i]);
	free(aux);
	printf("APPLY BLUR done\n");
}

void GAUSSIAN_BLUR(image *img)
{
	if (img->loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	if (strcmp(img->type, "P2") == 0 || strcmp(img->type, "P5") == 0) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}
	// create an aux matrix for calculus
	RGB **aux = malloc(img->row * sizeof(RGB *));
	for (int i = 0; i < img->row; i++)
		aux[i] = malloc(img->col * sizeof(RGB));
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++) {
			aux[i][j].R = img->rgb[i][j].R;
			aux[i][j].G = img->rgb[i][j].G;
			aux[i][j].B = img->rgb[i][j].B;
		}
	// work on the selection
	for (int i = img->y1; i < img->y2; i++)
		for (int j = img->x1; j < img->x2; j++) {
			if (!(i > 0 && j > 0 && j < img->col - 1 &&
				  i < img->row - 1))  // the edges
				continue;
			aux[i][j].R = (img->rgb[i - 1][j - 1].R + img->rgb[i - 1][j].R * 2 +
						img->rgb[i - 1][j + 1].R + img->rgb[i][j - 1].R * 2 +
						img->rgb[i][j + 1].R * 2 + img->rgb[i + 1][j - 1].R +
						img->rgb[i + 1][j].R * 2 + img->rgb[i + 1][j + 1].R +
						img->rgb[i][j].R * 4) /
						16;
			aux[i][j].G = (img->rgb[i - 1][j - 1].G + img->rgb[i - 1][j].G * 2 +
						img->rgb[i - 1][j + 1].G + img->rgb[i][j - 1].G * 2 +
						img->rgb[i][j + 1].G * 2 + img->rgb[i + 1][j - 1].G +
						img->rgb[i + 1][j].G * 2 + img->rgb[i + 1][j + 1].G +
						img->rgb[i][j].G * 4) /
						16;
			aux[i][j].B = (img->rgb[i - 1][j - 1].B + img->rgb[i - 1][j].B * 2 +
						img->rgb[i - 1][j + 1].B + img->rgb[i][j - 1].B * 2 +
						img->rgb[i][j + 1].B * 2 + img->rgb[i + 1][j - 1].B +
						img->rgb[i + 1][j].B * 2 + img->rgb[i + 1][j + 1].B +
						img->rgb[i][j].B * 4) /
						16;
		}
	// insert the edited selection in the og image
	for (int i = img->y1; i < img->y2; i++)
		for (int j = img->x1; j < img->x2; j++) {
			img->rgb[i][j].R = aux[i][j].R;
			img->rgb[i][j].G = aux[i][j].G;
			img->rgb[i][j].B = aux[i][j].B;
		}
	// free the aux
	for (int i = 0; i < img->row; i++)
		free(aux[i]);
	free(aux);
	printf("APPLY GAUSSIAN_BLUR done\n");
}

void rotate_90_right_gray(image *img)
{
	// create an auxiliar matrix
	int **aux = malloc(img->col * sizeof(int *));
	for (int i = 0; i < img->col; i++)
		aux[i] = malloc(img->row * sizeof(int));

	// put the values in the aux matrix but roatated
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++)
			aux[j][img->row - i - 1] = img->mat[i][j];

	// free the og matrix
	for (int i = 0; i < img->row; i++)
		free(img->mat[i]);
	free(img->mat);

	// create space for the new one
	img->mat = malloc(img->col * sizeof(int *));
	for (int i = 0; i < img->col; i++)
		img->mat[i] = malloc(img->row * sizeof(int));

	// put the values
	for (int i = 0; i < img->col; i++)
		for (int j = 0; j < img->row; j++)
			img->mat[i][j] = aux[i][j];

	// free the aux
	for (int i = 0; i < img->col; i++)
		free(aux[i]);
	free(aux);

	// change the dimensions
	int tmp;
	tmp = img->row;
	img->row = img->col;
	img->col = tmp;

	// change the selection
	img->x1 = 0;
	img->x2 = img->col;
	img->y1 = 0;
	img->y2 = img->row;
}

void rotate_90_right_select_gray(image *img)
{
	// Create an aux of the selection
	int **aux = malloc((img->y2 - img->y1) * sizeof(int *));
	for (int i = 0; i < (img->y2 - img->y1); i++)
		aux[i] = malloc((img->x2 - img->x1) * sizeof(int));

	// Put the values in the aux but rotated
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			aux[i][j] = img->mat[i + img->y1][j + img->x1];

	// Create an aux to store the rotated selection
	int **rotated_aux = malloc((img->y2 - img->y1) * sizeof(int *));
	for (int i = 0; i < (img->y2 - img->y1); i++)
		rotated_aux[i] = malloc((img->x2 - img->x1) * sizeof(int));

	// Rotate the aux 90 degrees right and store it in the rotated_aux
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			rotated_aux[j][(img->y2 - img->y1) - i - 1] = aux[i][j];

	// Copy the rotated values back to the original image
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			img->mat[i + img->y1][j + img->x1] = rotated_aux[i][j];

	// Free memory for aux and rotated_aux
	for (int i = 0; i < (img->y2 - img->y1); i++)
		free(aux[i]);
	free(aux);

	for (int i = 0; i < (img->y2 - img->y1); i++)
		free(rotated_aux[i]);
	free(rotated_aux);
}

void rotate_90_right_rgb(image *img)
{
	// create an auxiliar matrix
	RGB **aux = malloc(img->col * sizeof(RGB *));
	for (int i = 0; i < img->col; i++)
		aux[i] = malloc(img->row * sizeof(RGB));

	// put the values in the aux matrix but roatated
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++)
			aux[j][img->row - i - 1] = img->rgb[i][j];

	// free the og matrix
	for (int i = 0; i < img->row; i++)
		free(img->rgb[i]);
	free(img->rgb);

	// create space for the new one
	img->rgb = malloc(img->col * sizeof(RGB *));
	for (int i = 0; i < img->col; i++)
		img->rgb[i] = malloc(img->row * sizeof(RGB));

	// put the values
	for (int i = 0; i < img->col; i++)
		for (int j = 0; j < img->row; j++)
			img->rgb[i][j] = aux[i][j];

	// free the aux
	for (int i = 0; i < img->col; i++)
		free(aux[i]);
	free(aux);

	// change the dimensions
	int tmp;
	tmp = img->row;
	img->row = img->col;
	img->col = tmp;

	// change the selection
	img->x1 = 0;
	img->x2 = img->col;
	img->y1 = 0;
	img->y2 = img->row;
}

void rotate_90_right_select_rgb(image *img)
{
	// Create an aux of the selection
	RGB **aux = malloc((img->y2 - img->y1) * sizeof(RGB *));
	for (int i = 0; i < (img->y2 - img->y1); i++)
		aux[i] = malloc((img->x2 - img->x1) * sizeof(RGB));

	// Put the values in the aux but rotated
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			aux[i][j] = img->rgb[i + img->y1][j + img->x1];

	// Create an aux to store the rotated selection
	RGB **rotated_aux = malloc((img->y2 - img->y1) * sizeof(RGB *));
	for (int i = 0; i < (img->y2 - img->y1); i++)
		rotated_aux[i] = malloc((img->x2 - img->x1) * sizeof(RGB));

	// Rotate the aux 90 degrees right and store it in the rotated_aux
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			rotated_aux[j][(img->y2 - img->y1) - i - 1] = aux[i][j];

	// Copy the rotated values back to the original image
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			img->rgb[i + img->y1][j + img->x1] = rotated_aux[i][j];

	// Free memory for aux and rotated_aux
	for (int i = 0; i < (img->y2 - img->y1); i++)
		free(aux[i]);
	free(aux);

	for (int i = 0; i < (img->y2 - img->y1); i++)
		free(rotated_aux[i]);
	free(rotated_aux);
}

void rotate_180_right_gray(image *img)
{
	// create aux
	int **aux = malloc(img->row * sizeof(int *));
	for (int i = 0; i < img->row; i++)
		aux[i] = malloc(img->col * sizeof(int));

	// put the values in the aux matrix but roatated
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++)
			aux[img->row - i - 1][img->col - j - 1] = img->mat[i][j];

	// atribute the values to the og matrix
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++)
			img->mat[i][j] = aux[i][j];

	// free the aux matrix
	for (int i = 0; i < img->row; i++)
		free(aux[i]);
	free(aux);
}

void rotate_180_right_select_gray(image *img)
{
	// Create an aux of the selection
	int **aux = malloc((img->y2 - img->y1) * sizeof(int *));
	for (int i = 0; i < (img->y2 - img->y1); i++)
		aux[i] = malloc((img->x2 - img->x1) * sizeof(int));

	// Put the values in the aux matrix but rotated
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			aux[img->y2 - img->y1 - i - 1][img->x2 - img->x1 - j - 1] =
				img->mat[i + img->y1][j + img->x1];

	// Copy the rotated values back to the original image
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			img->mat[i + img->y1][j + img->x1] = aux[i][j];

	// Free memory for aux
	for (int i = 0; i < (img->y2 - img->y1); i++)
		free(aux[i]);
	free(aux);
}

void rotate_180_right_select_rgb(image *img)
{
	// Create an aux of the selection
	RGB **aux = malloc((img->y2 - img->y1) * sizeof(RGB *));
	for (int i = 0; i < (img->y2 - img->y1); i++)
		aux[i] = malloc((img->x2 - img->x1) * sizeof(RGB));

	// Put the values in the aux matrix but rotated
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			aux[img->y2 - img->y1 - i - 1][img->x2 - img->x1 - j - 1] =
				img->rgb[i + img->y1][j + img->x1];

	// Copy the rotated values back to the original image
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			img->rgb[i + img->y1][j + img->x1] = aux[i][j];

	// Free memory for aux
	for (int i = 0; i < (img->y2 - img->y1); i++)
		free(aux[i]);
	free(aux);
}

void rotate_180_right_rgb(image *img)
{
	// create aux
	RGB **aux = malloc(img->row * sizeof(RGB *));
	for (int i = 0; i < img->row; i++)
		aux[i] = malloc(img->col * sizeof(RGB));

	// put the values in the aux matrix but roatated
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++)
			aux[img->row - i - 1][img->col - j - 1] = img->rgb[i][j];

	// atribute the values to the og matrix
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++)
			img->rgb[i][j] = aux[i][j];

	// free the aux matrix
	for (int i = 0; i < img->row; i++)
		free(aux[i]);
	free(aux);
}

void rotate_90_left_gray(image *img)
{
	// create an auxiliar matrix
	int **aux = malloc(img->col * sizeof(int *));
	for (int i = 0; i < img->col; i++)
		aux[i] = malloc(img->row * sizeof(int));

	// put the values in the aux matrix but roatated
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++)
			aux[img->col - j - 1][i] = img->mat[i][j];

	// free the og matrix
	for (int i = 0; i < img->row; i++)
		free(img->mat[i]);
	free(img->mat);

	// create space for the new one
	img->mat = malloc(img->col * sizeof(int *));
	for (int i = 0; i < img->col; i++)
		img->mat[i] = malloc(img->row * sizeof(int));

	// put the values
	for (int i = 0; i < img->col; i++)
		for (int j = 0; j < img->row; j++)
			img->mat[i][j] = aux[i][j];

	// free the aux
	for (int i = 0; i < img->col; i++)
		free(aux[i]);
	free(aux);

	// change the dimensions
	int tmp;
	tmp = img->row;
	img->row = img->col;
	img->col = tmp;

	// change the selection
	img->x1 = 0;
	img->x2 = img->col;
	img->y1 = 0;
	img->y2 = img->row;
}

void rotate_90_left_select_gray(image *img)
{
	// Create an aux of the selection
	int **aux = malloc((img->y2 - img->y1) * sizeof(int *));
	for (int i = 0; i < (img->y2 - img->y1); i++)
		aux[i] = malloc((img->x2 - img->x1) * sizeof(int));

	// Put the values in the aux but rotated
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			aux[i][j] = img->mat[i + img->y1][j + img->x1];

	// Create an aux to store the rotated selection
	int **rotated_aux = malloc((img->y2 - img->y1) * sizeof(int *));
	for (int i = 0; i < (img->y2 - img->y1); i++)
		rotated_aux[i] = malloc((img->x2 - img->x1) * sizeof(int));

	// Rotate the aux 90 degrees right and store it in the rotated_aux
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			rotated_aux[(img->x2 - img->x1) - j - 1][i] = aux[i][j];

	// Copy the rotated values back to the original image
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			img->mat[i + img->y1][j + img->x1] = rotated_aux[i][j];

	// Free memory for aux and rotated_aux
	for (int i = 0; i < (img->y2 - img->y1); i++)
		free(aux[i]);
	free(aux);

	for (int i = 0; i < (img->y2 - img->y1); i++)
		free(rotated_aux[i]);
	free(rotated_aux);
}

void rotate_90_left_rgb(image *img)
{
	// create an auxiliar matrix
	RGB **aux = malloc(img->col * sizeof(RGB *));
	for (int i = 0; i < img->col; i++)
		aux[i] = malloc(img->row * sizeof(RGB));

	// put the values in the aux matrix but roatated
	for (int i = 0; i < img->row; i++)
		for (int j = 0; j < img->col; j++)
			aux[j][img->row - i - 1] = img->rgb[i][j];

	// free the og matrix
	for (int i = 0; i < img->row; i++)
		free(img->rgb[i]);
	free(img->rgb);

	// create space for the new one
	img->rgb = malloc(img->col * sizeof(RGB *));
	for (int i = 0; i < img->col; i++)
		img->rgb[i] = malloc(img->row * sizeof(RGB));

	// put the values
	for (int i = 0; i < img->col; i++)
		for (int j = 0; j < img->row; j++)
			img->rgb[i][j] = aux[i][j];

	// free the aux
	for (int i = 0; i < img->col; i++)
		free(aux[i]);
	free(aux);

	// change the dimensions
	int tmp;
	tmp = img->row;
	img->row = img->col;
	img->col = tmp;

	// change the selection
	img->x1 = 0;
	img->x2 = img->col;
	img->y1 = 0;
	img->y2 = img->row;
}

void rotate_90_left_select_rgb(image *img)
{
	// Create an aux of the selection
	RGB **aux = malloc((img->y2 - img->y1) * sizeof(RGB *));
	for (int i = 0; i < (img->y2 - img->y1); i++)
		aux[i] = malloc((img->x2 - img->x1) * sizeof(RGB));

	// Put the values in the aux but rotated
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			aux[i][j] = img->rgb[i + img->y1][j + img->x1];

	// Create an aux to store the rotated selection
	RGB **rotated_aux = malloc((img->y2 - img->y1) * sizeof(RGB *));
	for (int i = 0; i < (img->y2 - img->y1); i++)
		rotated_aux[i] = malloc((img->x2 - img->x1) * sizeof(RGB));

	// Rotate the aux 90 degrees right and store it in the rotated_aux
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			rotated_aux[(img->x2 - img->x1) - j - 1][i] = aux[i][j];

	// Copy the rotated values back to the original image
	for (int i = 0; i < (img->y2 - img->y1); i++)
		for (int j = 0; j < (img->x2 - img->x1); j++)
			img->rgb[i + img->y1][j + img->x1] = rotated_aux[i][j];

	// Free memory for aux and rotated_aux
	for (int i = 0; i < (img->y2 - img->y1); i++)
		free(aux[i]);
	free(aux);

	for (int i = 0; i < (img->y2 - img->y1); i++)
		free(rotated_aux[i]);
	free(rotated_aux);
}

void ROTATE(image *img, int angle)
{
	if (img->loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	if (angle != 90 && angle != -90 && angle != 180 && angle != -180 &&
		angle != 270 && angle != -270 && angle != 360 && angle != -360 &&
		angle != 0) {
		printf("Unsupported rotation angle\n");
		return;
	}
	if (((img->x2 - img->x1) != (img->y2 - img->y1)) && img->x1 != 0 &&
		img->x2 != img->col && img->y1 != 0 && img->y2 != img->row) {
		printf("The selection must be square\n");
		return;
	}
	if (angle == 90 || angle == -270) {
		if (img->x1 == 0 && img->x2 == img->col && img->y1 == 0 &&
			img->y2 == img->row) {  // full image
			// grayscale
			if (strcmp(img->type, "P2") == 0 || strcmp(img->type, "P5") == 0)
				rotate_90_right_gray(img);

			// RGB
			if (strcmp(img->type, "P3") == 0 || strcmp(img->type, "P6") == 0)
				rotate_90_right_rgb(img);
		} else {
			// grayscale
			if (strcmp(img->type, "P2") == 0 || strcmp(img->type, "P5") == 0)
				rotate_90_right_select_gray(img);

			// RGB
			if (strcmp(img->type, "P3") == 0 || strcmp(img->type, "P6") == 0)
				rotate_90_right_select_rgb(img);
		}
	}
	if (angle == -90 || angle == 270) {
		if (img->x1 == 0 && img->x2 == img->col && img->y1 == 0 &&
			img->y2 == img->row) {  // full image
			// grayscale
			if (strcmp(img->type, "P2") == 0 || strcmp(img->type, "P5") == 0)
				rotate_90_left_gray(img);

			// RGB
			if (strcmp(img->type, "P3") == 0 || strcmp(img->type, "P6") == 0)
				rotate_90_left_rgb(img);
		} else {
			// grayscale
			if (strcmp(img->type, "P2") == 0 || strcmp(img->type, "P5") == 0)
				rotate_90_left_select_gray(img);

			// RGB
			if (strcmp(img->type, "P3") == 0 || strcmp(img->type, "P6") == 0)
				rotate_90_left_select_rgb(img);
		}
	}
	if (angle == 180 || angle == -180) {
		if (img->x1 == 0 && img->x2 == img->col && img->y1 == 0 &&
			img->y2 == img->row) {  // full image
			// grayscale
			if (strcmp(img->type, "P2") == 0 || strcmp(img->type, "P5") == 0)
				rotate_180_right_gray(img);

			// RGB
			if (strcmp(img->type, "P3") == 0 || strcmp(img->type, "P6") == 0)
				rotate_180_right_rgb(img);
		} else {
			// grayscale
			if (strcmp(img->type, "P2") == 0 || strcmp(img->type, "P5") == 0)
				rotate_180_right_select_gray(img);

			// RGB
			if (strcmp(img->type, "P3") == 0 || strcmp(img->type, "P6") == 0)
				rotate_180_right_select_rgb(img);
		}
	}
	printf("Rotated %d\n", angle);
}

int main(void)
{
	image *img = malloc(sizeof(image));
	img->loaded = 0;
	img->mat = NULL;
	img->rgb = NULL;
	char command[50];
	// scanf("%s", command);
	// printf("%d\n", nr_tokens(command));
	while (1) {
		fgets(command, 50, stdin);
		if (verif_LOAD(command)) {
			call_LOAD(img, command);
		} else if (APPLY_NO_LOADED(command, img)) {
			printf("No image loaded\n");
		} else if (HISTOGRAM_NO_LOADED(command, img)) {
			printf("No image loaded\n");
		} else if (APPLY_LOADED(command, img)) {
			printf("Invalid command\n");
		} else if (verif_SAVE(command)) {
			call_SAVE(img, command);
		} else if (verif_SELECT(command)) {
			call_SELECT(img, command);
		} else if (verif_SELECT_ALL(command)) {
			call_SELECT_ALL(img);
		} else if (verif_CROP(command)) {
			call_CROP(img);
		} else if (verif_EQUALIZE(command)) {
			call_EQUALIZE(img);
		} else if (verif_HISTOGRAM(command)) {
			call_HISTOGRAM(img, command);
		} else if (verif_EDGE(command)) {
			call_APPLY_EDGE(img);
		} else if (verif_SHARPEN(command)) {
			call_APPLY_SHARPEN(img);
		} else if (verif_BLUR(command)) {
			call_APPLY_BLUR(img);
		} else if (verif_GAUSSIAN_BLUR(command)) {
			call_APPLY_GAUSSIAN_BLUR(img);
		} else if (verif_ROTATE(command)) {
			call_ROTATE(img, command);
		} else if (verif_EXIT(command)) {
			call_EXIT(img);
			break;
		} else if (invalid_APPLY(command)) {
			printf("APPLY parameter invalid\n");
		} else {
			printf("Invalid command\n");
		}
	}
	return 0;
}
