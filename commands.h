// Apostol Rares-Andrei 311CA
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "structs.h"
#include "functions.h"

void load_P2(PGM_pic * pgmp, FILE * fptr, char name[100]);
void load_P5(PGM_pic *pgmp, FILE *fptr, char name[100]);
void load_P3(PPM_pic *ppmp, FILE *fptr, char name[100]);
void load_P6(PPM_pic *ppmp, FILE *fptr, char name[100]);
void load(FILE *fptr, PGM_pic *pgmp, PPM_pic *ppmp,
		  char command[200], int *ok);
void select_ppm(PPM_pic *ppm, int x1, int y1, int x2, int y2);
void select_pgm(PGM_pic *pgm, int x1, int y1, int x2, int y2);
void select(PPM_pic *ppm, PGM_pic *pgm, char command[200]);
void select_all(PPM_pic *ppm, PGM_pic *pgm);
void histogram(PPM_pic *ppm, PGM_pic *pgm, char command[200]);
void equalize(PGM_pic *pgm, PPM_pic *ppm);
void crop_pgm(PGM_pic *pgm);
void crop_ppm(PPM_pic *ppm);
void crop(PGM_pic *pgm, PPM_pic *ppm);
void apply_effect(PPM_pic *ppm, double kernel[][3], int x1, int x2,
				  int y1, int y2);
void apply(PGM_pic *pgm, PPM_pic *ppm, char command[200]);
void save_pgm_ascii(PGM_pic *pgm, FILE *out);
void save_ppm_ascii(PPM_pic *ppm, FILE *out);
void save_pgm_bin(PGM_pic *pgm, FILE *out);
void save_ppm_bin(PPM_pic *ppm, FILE *out);
void save(PGM_pic *pgm, PPM_pic *ppm, char command[200]);
void exit_program(PGM_pic *pgm, PPM_pic *ppm, int ok_exit);
void rotate_pgm(PGM_pic *pgm, int full);
void rotate_ppm(PPM_pic *ppm, int full);
void rotate(PGM_pic *pgm, PPM_pic *ppm, char command[200]);

#endif
