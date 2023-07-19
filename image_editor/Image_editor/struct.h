struct RGB;
typedef struct RGB RGB;
struct RGB {
	int R, G, B;
};

struct image;
typedef struct image image;
struct image {
	char type[5];
	int max_val;
	int row, col;
	int x1, x2, y1, y2;
	int **mat;
	RGB **rgb;
	int loaded;
};

