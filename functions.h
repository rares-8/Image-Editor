// Apostol Rares-Andrei 311CA
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "structs.h"

int **free_matrix(int **a, int m);
RGB **free_matrix_rgb(RGB **a, int m);
RGB **alloc_matrix_rgb(RGB **a, int m, int n);
int **alloc_matrix(int **a, int m, int n);
void swap_int(int *a, int *b);
void ignore_comments_txt(FILE *fptr);
void ignore_comments_bin(FILE *fptr);
int check_if_number(char command[200], int index);
int clamp(int x, int min, int max);
void freq(int v[256], PGM_pic *pgm);
int check_select(char command[200]);
void read_pixels(int *x1, int *x2, int *y1, int *y2, char command[200]);
int check_select_all(char command[200]);
int check_histogram(char command[200]);
int check_bins(int x);
void read_hist(int *stars, int *bins, char command[200]);
int check_equalize(char command[200]);
int check_crop(char command[200]);
int check_effect(char *token);
void initialize_kernel(double kernel[][3], int type);
void initialize_selection(int *x1, int *x2, int *y1, int *y2, PPM_pic *ppm);
int check_apply_command(PGM_pic *pgm, PPM_pic *ppm, char command[200]);
int check_exit(char command[200]);
int check_rotate(char command[200]);
int square_matrix(PGM_pic *pgm, PPM_pic *ppm, int *full);
int **transpose_pgm(int **a, int m, int n);
RGB **transpose_ppm(RGB **a, int m, int n);

#endif
