// Apostol Rares-Andrei 311CA
#include "functions.h"

// free la o matrice int
int **free_matrix(int **a, int m)
{
	for (int i = 0; i < m; i++)
		free(a[i]);
	free(a);
	a = NULL;
	return a;
}

// free la o matrice RGB
RGB **free_matrix_rgb(RGB **a, int m)
{
	for (int i = 0; i < m; i++)
		free(a[i]);
	free(a);
	a = NULL;
	return a;
}

// aloc o matrice RGB
RGB **alloc_matrix_rgb(RGB **a, int m, int n)
{
	a = (RGB **)calloc(m, sizeof(RGB *));

	if (!a)
		return NULL;

	for (int i = 0; i < m; i++) {
		a[i] = (RGB *)calloc(n, sizeof(RGB));
		if (!a[i]) {
			for (int j = i; j >= 0; j--)
				free(a[j]);
			return NULL;
		}
	}
	return a;
}

// aloc o matrice int
int **alloc_matrix(int **a, int m, int n)
{
	a = (int **)calloc(m, sizeof(int *));

	if (!a)
		return NULL;

	for (int i = 0; i < m; i++) {
		a[i] = (int *)calloc(n, sizeof(int));
		if (!a[i]) {
			for (int j = i; j >= 0; j--)
				free(a[j]);
			return NULL;
		}
	}
	return a;
}

void swap_int(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

// se ignora comentariile din fisier
void ignore_comments_txt(FILE *fptr)
{
	char first, ch;
	fscanf(fptr, "%c", &first);
	fscanf(fptr, "%c", &ch);

	/* se citesc cate doua caractere, si daca primul
	   este != # atunci se iese din while */

	while (first == '#') {
		while (ch != '\n')
			fscanf(fptr, "%c", &ch);
		fscanf(fptr, "%c", &first);
		fscanf(fptr, "%c", &ch);
	}
	// se revine la inceputul liniei
	fseek(fptr, -2, SEEK_CUR);
}

// se ignora comentariile din fisierele binare
void ignore_comments_bin(FILE *fptr)
{
	char first, ch;
	fread(&first, sizeof(char), 1, fptr);
	fread(&ch, sizeof(char), 1, fptr);

	while (first == '#') {
		while (ch != '\n')
			fread(&ch, sizeof(char), 1, fptr);
		fread(&first, sizeof(char), 1, fptr);
		fread(&ch, sizeof(char), 1, fptr);
	}
	// se revine la inceputul liniei
	fseek(fptr, -2, SEEK_CUR);
}

// se verifica daca un string este numar
int check_if_number(char command[200], int index)
{
	if (command[index] >= '0' && command[index] <= '9')
		return 1;
	if (command[index] == '-') {
		if (!(command[index + 1] >= '0' && command[index + 1] <= '9'))
			return 0;
		if (!(command[index - 1] == ' '))
			return 0;
		return 1;
	}
	return 0;
}

int clamp(int x, int min, int max)
{
	if (x < 0)
		return min;
	if (x > 255)
		return max;
	return x;
}

// se face un vector de frecventa pentru o poza
void freq(int v[256], PGM_pic *pgm)
{
	for (int i = 0; i < pgm->height; i++)
		for (int j = 0; j < pgm->width; j++)
			v[pgm->a[i][j]]++;
}

// verifica daca a fost introdusa comanda SELECT si daca  valida
int check_select(char command[200])
{
	if (strncmp(command, "SELECT", 6))
		return 0;

	char copy[200];
	strcpy(copy, command);
	copy[strlen(copy) - 1] = '\0';
	for (unsigned long i = 7; i < strlen(copy); i++) {
		if (command[i] != ' ' && command[i] != '\n') {
			if (!check_if_number(command, i))
				return 0;
		}
	}

	int cnt_num = 0;
	char *token = strtok(copy, " ");
	token = strtok(NULL, " ");
	while (token) {
		cnt_num++;
		token = strtok(NULL, " ");
	}
	if (cnt_num != 4)
		return 0;

	return 1;
}

// se citesc pixelii pentru selectie
void read_pixels(int *x1, int *x2, int *y1, int *y2, char command[200])
{
	char *token = strtok(command, " ");
	token = strtok(NULL, " ");
	*x1 = atoi(token);
	token = strtok(NULL, " ");
	*y1 = atoi(token);
	token = strtok(NULL, " ");
	*x2 = atoi(token);
	token = strtok(NULL, " ");
	*y2 = atoi(token);
}

// verifica daca a fost introdusa comanda SELECT ALL si daca este valida
int check_select_all(char command[200])
{
	if (strncmp(command, "SELECT ALL", 9))
		return 0;

	for (unsigned long i = 10; i < strlen(command); i++) {
		if (command[i] != ' ' && command[i] != '\n')
			return 0;
	}
	return 1;
}

// verifica daca a fost introdusa comanda HISTOGRAM si daca este valida
int check_histogram(char command[200])
{
	char copy[200];
	strcpy(copy, command);
	copy[strlen(copy) - 1] = '\0';
	for (unsigned long i = 9; i < strlen(copy); i++) {
		if (command[i] != ' ' && command[i] != '\n') {
			if (!check_if_number(command, i))
				return 0;
		}
	}

	int cnt_num = 0;
	char *token = strtok(copy, " ");
	token = strtok(NULL, " ");
	// se numara parametrii
	while (token) {
		cnt_num++;
		token = strtok(NULL, " ");
	}
	if (cnt_num != 2)
		return 0;

	return 1;
}

// verifica daca bins este o putere a lui 2 in intervalul [2,256]
int check_bins(int x)
{
	int ok = 0, nr = 2;
	while (nr != 256) {
		if (x % nr == 0) {
			ok = 1;
			break;
		}
		nr *= 2;
	}
	return ok;
}

void read_hist(int *stars, int *bins, char command[200])
{
	char *token = strtok(command, " ");
	token = strtok(NULL, " ");
	*stars = atoi(token);
	token = strtok(NULL, " ");
	*bins = atoi(token);
}

// se verifica daca a fost introdusa comanda EQUALIZE si daca e valida
int check_equalize(char command[200])
{
	if (strncmp(command, "EQUALIZE", 8))
		return 0;

	for (unsigned long i = 9; i < strlen(command); i++) {
		if (command[i] != ' ' && command[i] != '\n')
			return 0;
	}
	return 1;
}

// verifica daca a fost introdusa corect comanda for
int check_crop(char command[200])
{
	if (strncmp(command, "CROP", 4))
		return 0;

	for (unsigned long i = 5; i < strlen(command); i++) {
		if (command[i] != ' ' && command[i] != '\n')
			return 0;
	}
	return 1;
}

// verifica daca efectul este valid
int check_effect(char *token)
{
	if (!strcmp(token, "EDGE"))
		return 1;
	if (!strcmp(token, "SHARPEN"))
		return 2;
	if (!strcmp(token, "BLUR"))
		return 3;
	if (!strcmp(token, "GAUSSIAN_BLUR"))
		return 4;
	return -1;
}

// in functie de efect, se initializeaza kernelul
void initialize_kernel(double kernel[][3], int type)
{
	switch (type) {
	case 1:
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (i == 1 && j == 1)
					kernel[i][j] = 8.0;
				else
					kernel[i][j] = -1.0;
			}
		}
		break;
	case 2:
		kernel[0][0] = 0.0;
		kernel[0][1] = -1.0;
		kernel[0][2] = 0.0;
		kernel[1][0] = -1.0;
		kernel[1][1] = 5.0;
		kernel[1][2] = -1.0;
		kernel[2][0] = 0.0;
		kernel[2][1] = -1.0;
		kernel[2][2] = 0.0;
		break;
	case 3:
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
				kernel[i][j] = 1.0 / 9.0;
		}
		break;
	case 4:
		kernel[0][0] = 1.0 / 16.0;
		kernel[0][1] = 2.0 / 16.0;
		kernel[0][2] = 1.0 / 16.0;
		kernel[1][0] = 2.0 / 16.0;
		kernel[1][1] = 4.0 / 16.0;
		kernel[1][2] = 2.0 / 16.0;
		kernel[2][0] = 1.0 / 16.0;
		kernel[2][1] = 2.0 / 16.0;
		kernel[2][2] = 1.0 / 16.0;
		break;
	}
}

/* se initializeaza zona de selectie astfel incat pixelii din margine
 sa ramana nemodificati */
void initialize_selection(int *x1, int *x2, int *y1, int *y2, PPM_pic *ppm)
{
	if (ppm->x1 == 0)
		*x1 = 1;
	else
		*x1 = ppm->x1;

	if (ppm->x2 == ppm->width)
		*x2 = ppm->width - 1;
	else
		*x2 = ppm->x2;

	if (ppm->y1 == 0)
		*y1 = 1;
	else
		*y1 = ppm->y1;

	if (ppm->y2 == ppm->height)
		*y2 = ppm->height - 1;
	else
		*y2 = ppm->y2;
}

// verific daca a fost introdusa corect comanda APPLY (in afara de efect)
int check_apply_command(PGM_pic *pgm, PPM_pic *ppm, char command[200])
{
	if (command[5] != '\n' && command[5] != ' ') {
		printf("Invalid command\n");
		return 0;
	}
	if (!pgm->a && !ppm->a && command[5] == '\n') {
		printf("No image loaded\n");
		return 0;
	} else if ((pgm->a || ppm->a) && command[5] == '\n') {
		printf("Invalid command\n");
		return 0;
	}

	if (!pgm->a && !ppm->a) {
		printf("No image loaded\n");
		return 0;
	}

	if (pgm->a) {
		printf("Easy, Charlie Chaplin\n");
		return 0;
	}

	return 1;
}

int check_exit(char command[200])
{
	if (strncmp(command, "EXIT", 4))
		return 0;

	for (unsigned long i = 5; i < strlen(command); i++) {
		if (command[i] != ' ' && command[i] != '\n')
			return 0;
	}
	return 1;
}

// verifica daca comanda ROTATE a fost introdusa corect
int check_rotate(char command[200])
{
	if (strncmp(command, "ROTATE", 6))
		return 0;

	char copy[200];
	strcpy(copy, command);
	copy[strlen(copy) - 1] = '\0';
	for (unsigned long i = 7; i < strlen(copy); i++) {
		if (command[i] != ' ' && command[i] != '\n') {
			if (!check_if_number(command, i))
				return 0;
		}
	}

	int cnt_num = 0;
	char *token = strtok(copy, " ");
	token = strtok(NULL, " ");
	while (token) {
		cnt_num++;
		token = strtok(NULL, " ");
	}
	if (cnt_num != 1)
		return 0;
	return 1;
}

/* verifica daca matricea selectata este patratica
   sau daca este selectata intreaga poza */
int square_matrix(PGM_pic *pgm, PPM_pic *ppm, int *full)
{
	int l1, l2;
	// calculez laturile submatricei selectate
	if (pgm->a) {
		l1 = pgm->x2 - pgm->x1;
		l2 = pgm->y2 - pgm->y1;
	} else {
		l1 = ppm->x2 - ppm->x1;
		l2 = ppm->y2 - ppm->y1;
	}

	if (l1 == l2) {
		*full = 0;
		return 1;
	}

	if (pgm->a) {
		if (pgm->x1 == 0 && pgm->x2 == pgm->width) {
			if (pgm->y1 == 0 && pgm->y2 == pgm->height) {
				*full = 1;
				return 1;
			}
		}
	} else {
		if (ppm->x1 == 0 && ppm->x2 == ppm->width) {
			if (ppm->y1 == 0 && ppm->y2 == ppm->height) {
				*full = 1;
				return 1;
			}
		}
	}
	*full = -1;
	return 0;
}

// transpusa pentru o poza PGM
int **transpose_pgm(int **a, int m, int n)
{
	int **transp = NULL;
	int new_m = n;
	int new_n = m;

	// aloc noua matrice
	transp = alloc_matrix(transp, new_m, new_n);

	if (!transp) {
		printf("Failed to allocate memory\n");
		exit(1);
	}

	for (int i = 0; i < new_n; i++) {
		for (int j = 0; j < new_m; j++)
			transp[j][i] = a[i][j];
	}

	// realoc matricea pe noile dimensiuni
	a = free_matrix(a, m);
	a = NULL;
	a = alloc_matrix(a, new_m, new_n);

	if (!a) {
		printf("Failed to allocate memory\n");
		exit(1);
	}

	// copiez matricea noua in a
	for (int i = 0; i < new_m; i++) {
		for (int j = 0; j < new_n; j++)
			a[i][j] = transp[i][j];
	}

	// eliberez memoria folosita
	transp = free_matrix(transp, new_m);
	return a;
}

RGB **transpose_ppm(RGB **a, int m, int n)
{
	RGB **transp = NULL;
	int new_m = n;
	int new_n = m;

	// aloc noua matrice
	transp = alloc_matrix_rgb(transp, new_m, new_n);

	if (!transp) {
		printf("Failed to allocate memory\n");
		exit(1);
	}

	for (int i = 0; i < new_n; i++) {
		for (int j = 0; j < new_m; j++) {
			transp[j][i].R = a[i][j].R;
			transp[j][i].G = a[i][j].G;
			transp[j][i].B = a[i][j].B;
		}
	}

	// realoc matricea pe noile dimensiuni
	a = free_matrix_rgb(a, m);
	a = NULL;
	a = alloc_matrix_rgb(a, new_m, new_n);

	if (!a) {
		printf("Failed to allocate memory\n");
		exit(1);
	}

	// copiez matricea noua in a
	for (int i = 0; i < new_m; i++) {
		for (int j = 0; j < new_n; j++) {
			a[i][j].R = transp[i][j].R;
			a[i][j].G = transp[i][j].G;
			a[i][j].B = transp[i][j].B;
		}
	}

	// eliberez memoria folosita
	transp = free_matrix_rgb(transp, new_m);
	return a;
}
