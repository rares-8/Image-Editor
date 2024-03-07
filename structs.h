// Apostol Rares-Andrei 311CA
#ifndef STRUCTS_H_
#define STRUCTS_H_

// structura pentru imagini PGM
typedef struct PGM_pic {
	int **a;
	int height;
	int width;
	int max_value;
	int x1;
	int x2;
	int y1;
	int y2;
	char type[3];
} PGM_pic;

typedef struct RGB {
	int R;
	int G;
	int B;
} RGB;

// structura pentru imagini PPM
typedef struct PPM_pic {
	RGB **a;
	int height;
	int width;
	int max_value;
	int x1;
	int x2;
	int y1;
	int y2;
	char type[3];
} PPM_pic;

#endif
