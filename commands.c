// Apostol Rares-Andrei 311CA
#include "commands.h"

// load pentru imagine greyscale tip P2
void load_P2(PGM_pic *pgmp, FILE *fptr, char name[100])
{
	fptr = fopen(name, "r");
	if (!fptr) {
		printf("Failed to load %s\n", name);
		return;
	}

	// se citeste headerul
	fscanf(fptr, "%s", pgmp->type);
	fgetc(fptr);
	ignore_comments_txt(fptr);
	fscanf(fptr, "%d%d", &pgmp->width, &pgmp->height);
	fscanf(fptr, "%d", &pgmp->max_value);

	pgmp->a = alloc_matrix(pgmp->a, pgmp->height, pgmp->width);
	if (!pgmp->a) {
		printf("Failed to load %s\n", name);
		fclose(fptr);
		return;
	}

	for (int i = 0; i < pgmp->height; i++) {
		for (int j = 0; j < pgmp->width; j++)
			fscanf(fptr, "%d", &pgmp->a[i][j]);
	}

	pgmp->x1 = 0;
	pgmp->x2 = pgmp->width;
	pgmp->y1 = 0;
	pgmp->y2 = pgmp->height;
	fclose(fptr);
}

// load pentru imagine greyscale tip P5
void load_P5(PGM_pic *pgmp, FILE *fptr, char name[100])
{
	fptr = fopen(name, "rb");
	if (!fptr) {
		printf("Failed to load %s\n", name);
		return;
	}

	// se citeste headerul
	fscanf(fptr, "%s", pgmp->type);
	fgetc(fptr);
	ignore_comments_bin(fptr);
	fscanf(fptr, "%d%d", &pgmp->width, &pgmp->height);
	fscanf(fptr, "%d", &pgmp->max_value);
	fgetc(fptr);

	pgmp->a = alloc_matrix(pgmp->a, pgmp->height, pgmp->width);
	for (int i = 0; i < pgmp->height; i++) {
		for (int j = 0; j < pgmp->width; j++)
			fread(&pgmp->a[i][j], 1, 1, fptr);
	}

	pgmp->x1 = 0;
	pgmp->x2 = pgmp->width;
	pgmp->y1 = 0;
	pgmp->y2 = pgmp->height;
	fclose(fptr);
}

// load pentru imagine color tip P3
void load_P3(PPM_pic *ppmp, FILE *fptr, char name[100])
{
	fptr = fopen(name, "r");
	if (!fptr) {
		printf("Failed to load %s\n", name);
		return;
	}

	// se citeste headerul
	fscanf(fptr, "%s", ppmp->type);
	fgetc(fptr);
	ignore_comments_txt(fptr);
	fscanf(fptr, "%d%d", &ppmp->width, &ppmp->height);
	fscanf(fptr, "%d", &ppmp->max_value);

	ppmp->a = alloc_matrix_rgb(ppmp->a, ppmp->height, ppmp->width);
	if (!ppmp->a) {
		printf("Failed to load %s\n", name);
		fclose(fptr);
		return;
	}

	for (int i = 0; i < ppmp->height; i++) {
		for (int j = 0; j < ppmp->width; j++) {
			fscanf(fptr, "%d", &ppmp->a[i][j].R);
			fscanf(fptr, "%d", &ppmp->a[i][j].G);
			fscanf(fptr, "%d", &ppmp->a[i][j].B);
		}
	}

	ppmp->x1 = 0;
	ppmp->x2 = ppmp->width;
	ppmp->y1 = 0;
	ppmp->y2 = ppmp->height;
	fclose(fptr);
}

// load pentru imagine color tip P6
void load_P6(PPM_pic *ppmp, FILE *fptr, char name[100])
{
	fptr = fopen(name, "rb");
	if (!fptr) {
		printf("Failed to load %s\n", name);
		return;
	}

	// se citeste headerul
	fscanf(fptr, "%s", ppmp->type);
	fgetc(fptr);
	ignore_comments_bin(fptr);
	fscanf(fptr, "%d%d", &ppmp->width, &ppmp->height);
	fscanf(fptr, "%d", &ppmp->max_value);
	fgetc(fptr);

	ppmp->a = alloc_matrix_rgb(ppmp->a, ppmp->height, ppmp->width);
	if (!ppmp->a) {
		printf("Failed to load %s\n", name);
		fclose(fptr);
		return;
	}

	for (int i = 0; i < ppmp->height; i++) {
		for (int j = 0; j < ppmp->width; j++) {
			fread(&ppmp->a[i][j].R, 1, 1, fptr);
			fread(&ppmp->a[i][j].G, 1, 1, fptr);
			fread(&ppmp->a[i][j].B, 1, 1, fptr);
		}
	}

	ppmp->x1 = 0;
	ppmp->x2 = ppmp->width;
	ppmp->y1 = 0;
	ppmp->y2 = ppmp->height;
	fclose(fptr);
}

// se citeste tipul pozei si se apeleaza alte functii pentru a face loadul
void load(FILE *fptr, PGM_pic *pgmp, PPM_pic *ppmp, char command[200], int *ok)
{
	char *token = strtok(command, "\n ");
	token = strtok(NULL, "\n ");
	char name[100];
	strcpy(name, token);

	if (pgmp->a)
		pgmp->a = free_matrix(pgmp->a, pgmp->height);
	if (ppmp->a)
		ppmp->a = free_matrix_rgb(ppmp->a, ppmp->height);

	fptr = fopen(name, "r");
	if (!fptr) {
		printf("Failed to load %s\n", name);
		return;
	}

	char format[4];
	fscanf(fptr, "%s", format);
	fclose(fptr);

	if (!strcmp(format, "P2"))
		load_P2(pgmp, fptr, name);
	else if (!strcmp(format, "P5"))
		load_P5(pgmp, fptr, name);
	else if (!strcmp(format, "P3"))
		load_P3(ppmp, fptr, name);
	else if (!strcmp(format, "P6"))
		load_P6(ppmp, fptr, name);

	if (pgmp->a || ppmp->a) {
		printf("Loaded %s\n", name);
		*ok = 1;
	}
}

// verifica daca pixelii sunt valizi si ii pune in structura
void select_ppm(PPM_pic *ppm, int x1, int y1, int x2, int y2)
{
	if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0) {
		printf("Invalid set of coordinates\n");
		return;
	}
	if (x1 > ppm->width || x2 > ppm->width) {
		printf("Invalid set of coordinates\n");
		return;
	}
	if (y1 > ppm->height || y2 > ppm->height) {
		printf("Invalid set of coordinates\n");
		return;
	}
	printf("Selected %d %d %d %d\n", x1, y1, x2, y2);

	ppm->x1 = x1;
	ppm->x2 = x2;
	ppm->y1 = y1;
	ppm->y2 = y2;
}

// verifica daca pixelii sunt valizi si ii pune in structura
void select_pgm(PGM_pic *pgm, int x1, int y1, int x2, int y2)
{
	if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0) {
		printf("Invalid set of coordinates\n");
		return;
	}
	if (x1 > pgm->width || x2 > pgm->width) {
		printf("Invalid set of coordinates\n");
		return;
	}
	if (y1 > pgm->height || y2 > pgm->height) {
		printf("Invalid set of coordinates\n");
		return;
	}
	printf("Selected %d %d %d %d\n", x1, y1, x2, y2);

	pgm->x1 = x1;
	pgm->x2 = x2;
	pgm->y1 = y1;
	pgm->y2 = y2;
}

// se verificia daca este alocata o imagine si citeste pixelii
void select(PPM_pic *ppm, PGM_pic *pgm, char command[200])
{
	int x1, x2, y1, y2;
	if (!(ppm->a) && !(pgm->a)) {
		printf("No image loaded\n");
		return;
	}
	read_pixels(&x1, &x2, &y1, &y2, command);

	if (y1 == y2 || x1 == x2) {
		printf("Invalid set of coordinates\n");
		return;
	}

	if (x1 > x2)
		swap_int(&x1, &x2);
	if (y1 > y2)
		swap_int(&y1, &y2);

	if (ppm->a)
		select_ppm(ppm, x1, y1, x2, y2);
	else
		select_pgm(pgm, x1, y1, x2, y2);
}

void select_all(PPM_pic *ppm, PGM_pic *pgm)
{
	if (!(ppm->a) && !(pgm->a)) {
		printf("No image loaded\n");
		return;
	} else if (ppm->a) {
		ppm->x1 = 0;
		ppm->x2 = ppm->width;
		ppm->y1 = 0;
		ppm->y2 = ppm->height;
	} else {
		pgm->x1 = 0;
		pgm->x2 = pgm->width;
		pgm->y1 = 0;
		pgm->y2 = pgm->height;
	}
	printf("Selected ALL\n");
}

void histogram(PPM_pic *ppm, PGM_pic *pgm, char command[200])
{
	if (!(pgm->a) && !(ppm->a)) {
		printf("No image loaded\n");
		return;
	}

	if (command[9] != ' ') {
		printf("Invalid command\n");
		return;
	}

	if (ppm->a) {
		printf("Black and white image needed\n");
		return;
	}

	int stars, bins, max = 0;
	if (!check_histogram(command)) {
		printf("Invalid command\n");
		return;
	}
	read_hist(&stars, &bins, command);

	if (!check_bins(bins)) {
		printf("Invalid command\n");
		return;
	}

	int fr[256] = {0};
	freq(fr, pgm);

	int nsum = 256 / bins, sum, aux = 0, n = 0;
	int *v = calloc(bins, sizeof(int));

	// se calculeaza numarul de aparitii al pixelilor dintr-un anumit interval
	for (int i = 0; i < bins; i++) {
		sum = 0;
		for (int j = aux; j < aux + nsum; j++)
			sum += fr[j];
		aux += nsum;
		v[n++] = sum;
	}
	// maximul aparitiilor
	for (int i = 0; i < bins; i++)
		if (v[i] > max)
			max = v[i];
	int n_stars;
	// se calculeaza numarul de stelute
	for (int i = 0; i < bins; i++) {
		n_stars = floor(1.0 * stars * v[i] / (1.0 * max));
		printf("%d\t|\t", n_stars);
		for (int j = 0; j < n_stars; j++)
			printf("*");
		printf("\n");
	}

	free(v);
}

void equalize(PGM_pic *pgm, PPM_pic *ppm)
{
	if (!(pgm->a) && !(ppm->a)) {
		printf("No image loaded\n");
		return;
	} else if (ppm->a) {
		printf("Black and white image needed\n");
		return;
	}

	int v[256] = {0};
	freq(v, pgm);
	double area = pgm->height * pgm->width;
	for (int i = 0; i < pgm->height; i++) {
		for (int j = 0; j < pgm->width; j++) {
			double sum = 0;
			// se calculeaza suma din formula
			for (int k = 0; k <= pgm->a[i][j]; k++)
				sum += v[k];
			double aux1 = 255.0 * (1.0 / area) * sum;
			int aux2 = round(aux1);
			aux2 = clamp(aux2, 0, 255);
			pgm->a[i][j] = aux2;
		}
	}
	printf("Equalize done\n");
}

void crop_pgm(PGM_pic *pgm)
{
	int **aux = NULL;
	aux = alloc_matrix(aux, pgm->y2 - pgm->y1, pgm->x2 - pgm->x1);
	if (!aux) {
		printf("Could not allocate memory\n");
		exit(1);
	}

	// se copiaza selectia intr-o matrice
	int x_aux = 0, y_aux = 0;
	for (int i = pgm->y1; i < pgm->y2; i++) {
		y_aux = 0;
		for (int j = pgm->x1; j < pgm->x2; j++) {
			aux[x_aux][y_aux] = pgm->a[i][j];
			y_aux++;
		}
		x_aux++;
	}
	// se elibereaza matricea veche
	pgm->a = free_matrix(pgm->a, pgm->height);

	// se actualizeaza indicii si selectia
	pgm->height = pgm->y2 - pgm->y1;
	pgm->width = pgm->x2 - pgm->x1;

	pgm->x1 = 0;
	pgm->x2 = pgm->width;
	pgm->y1 = 0;
	pgm->y2 = pgm->height;

	pgm->a = aux;
}

void crop_ppm(PPM_pic *ppm)
{
	RGB **aux = NULL;
	aux = alloc_matrix_rgb(aux, ppm->y2 - ppm->y1, ppm->x2 - ppm->x1);
	if (!aux) {
		printf("Could not allocate memory\n");
		exit(1);
	}
	// se copiaza selectia intr-o matrice noua
	int x_aux = 0, y_aux = 0;
	for (int i = ppm->y1; i < ppm->y2; i++) {
		y_aux = 0;
		for (int j = ppm->x1; j < ppm->x2; j++) {
			aux[x_aux][y_aux].R = ppm->a[i][j].R;
			aux[x_aux][y_aux].G = ppm->a[i][j].G;
			aux[x_aux][y_aux].B = ppm->a[i][j].B;
			y_aux++;
		}
		x_aux++;
	}

	ppm->a = free_matrix_rgb(ppm->a, ppm->height);

	ppm->height = ppm->y2 - ppm->y1;
	ppm->width = ppm->x2 - ppm->x1;

	ppm->x1 = 0;
	ppm->x2 = ppm->width;
	ppm->y1 = 0;
	ppm->y2 = ppm->height;

	ppm->a = aux;
}

void crop(PGM_pic *pgm, PPM_pic *ppm)
{
	if (!(pgm->a) && !(ppm->a)) {
		printf("No image loaded\n");
		return;
	} else if (pgm->a) {
		crop_pgm(pgm);
	} else if (ppm->a) {
		crop_ppm(ppm);
	}

	printf("Image cropped\n");
}

void apply_effect(PPM_pic *ppm, double kernel[][3], int x1, int x2,
				  int y1, int y2)
{
	RGB **new_pic = NULL;
	new_pic = alloc_matrix_rgb(new_pic, ppm->height, ppm->width);
	if (!new_pic)
		exit(1);
	double sum;

	// copiez matricea veche in cea noua
	for (int i = 0; i < ppm->height; i++) {
		for (int j = 0; j < ppm->width; j++)
			new_pic[i][j] = ppm->a[i][j];
	}

	// se aplica efectul pe selectie
	for (int i = y1; i < y2; i++) {
		for (int j = x1; j < x2; j++) {
			sum = 0.0;
			sum += kernel[0][0] * (double)ppm->a[i - 1][j - 1].R;
			sum += kernel[0][1] * (double)ppm->a[i - 1][j].R;
			sum += kernel[0][2] * (double)ppm->a[i - 1][j + 1].R;
			sum += kernel[1][0] * (double)ppm->a[i][j - 1].R;
			sum += kernel[1][1] * (double)ppm->a[i][j].R;
			sum += kernel[1][2] * (double)ppm->a[i][j + 1].R;
			sum += kernel[2][0] * (double)ppm->a[i + 1][j - 1].R;
			sum += kernel[2][1] * (double)ppm->a[i + 1][j].R;
			sum += kernel[2][2] * (double)ppm->a[i + 1][j + 1].R;
			new_pic[i][j].R = round(sum);

			sum = 0.0;
			sum += kernel[0][0] * (double)ppm->a[i - 1][j - 1].G;
			sum += kernel[0][1] * (double)ppm->a[i - 1][j].G;
			sum += kernel[0][2] * (double)ppm->a[i - 1][j + 1].G;
			sum += kernel[1][0] * (double)ppm->a[i][j - 1].G;
			sum += kernel[1][1] * (double)ppm->a[i][j].G;
			sum += kernel[1][2] * (double)ppm->a[i][j + 1].G;
			sum += kernel[2][0] * (double)ppm->a[i + 1][j - 1].G;
			sum += kernel[2][1] * (double)ppm->a[i + 1][j].G;
			sum += kernel[2][2] * (double)ppm->a[i + 1][j + 1].G;
			new_pic[i][j].G = round(sum);

			sum = 0.0;
			sum += kernel[0][0] * (double)ppm->a[i - 1][j - 1].B;
			sum += kernel[0][1] * (double)ppm->a[i - 1][j].B;
			sum += kernel[0][2] * (double)ppm->a[i - 1][j + 1].B;
			sum += kernel[1][0] * (double)ppm->a[i][j - 1].B;
			sum += kernel[1][1] * (double)ppm->a[i][j].B;
			sum += kernel[1][2] * (double)ppm->a[i][j + 1].B;
			sum += kernel[2][0] * (double)ppm->a[i + 1][j - 1].B;
			sum += kernel[2][1] * (double)ppm->a[i + 1][j].B;
			sum += kernel[2][2] * (double)ppm->a[i + 1][j + 1].B;
			new_pic[i][j].B = round(sum);
		}
	}
	/* se copiaza in matrice, pe selectia, matricea
	 noua obtinuta in urma aplicarii efectului */
	for (int i = y1; i < y2; i++) {
		for (int j = x1; j < x2; j++) {
			ppm->a[i][j].R = clamp(new_pic[i][j].R, 0, 255);
			ppm->a[i][j].G = clamp(new_pic[i][j].G, 0, 255);
			ppm->a[i][j].B = clamp(new_pic[i][j].B, 0, 255);
		}
	}
	new_pic = free_matrix_rgb(new_pic, ppm->height);
}

void apply(PGM_pic *pgm, PPM_pic *ppm, char command[200])
{
	if (!check_apply_command(pgm, ppm, command))
		return;

	char *token = strtok(command, " ");
	token = strtok(NULL, " ");
	if (token[strlen(token) - 1] == '\n')
		token[strlen(token) - 1] = '\0';

	int type = check_effect(token);

	if (type == -1) {
		printf("APPLY parameter invalid\n");
		return;
	}

	double kernel[3][3];
	initialize_kernel(kernel, type);
	int x1, x2, y1, y2;
	initialize_selection(&x1, &x2, &y1, &y2, ppm);
	apply_effect(ppm, kernel, x1, x2, y1, y2);
	printf("APPLY %s done\n", token);
}

// se salveaza o poza in format PGM ASCII
void save_pgm_ascii(PGM_pic *pgm, FILE *out)
{
	fprintf(out, "P2\n%d %d\n%d\n", pgm->width, pgm->height, pgm->max_value);

	for (int i = 0; i < pgm->height; i++) {
		for (int j = 0; j < pgm->width; j++)
			fprintf(out, "%d ", pgm->a[i][j]);
		fprintf(out, "\n");
	}
}

// se salveaza o poza in format PPM ASCII
void save_ppm_ascii(PPM_pic *ppm, FILE *out)
{
	fprintf(out, "P3\n%d %d\n%d\n", ppm->width, ppm->height, ppm->max_value);

	for (int i = 0; i < ppm->height; i++) {
		for (int j = 0; j < ppm->width; j++) {
			fprintf(out, "%d ", ppm->a[i][j].R);
			fprintf(out, "%d ", ppm->a[i][j].G);
			fprintf(out, "%d ", ppm->a[i][j].B);
		}
		fprintf(out, "\n");
	}
}

// se salveaza o poza in format PGM binar
void save_pgm_bin(PGM_pic *pgm, FILE *out)
{
	fprintf(out, "P5\n%d %d\n%d\n", pgm->width, pgm->height, pgm->max_value);
	for (int i = 0; i < pgm->height; i++) {
		for (int j = 0; j < pgm->width; j++)
			fwrite(&pgm->a[i][j], 1, 1, out);
	}
}

// se salveaza o poza in format PPM binar
void save_ppm_bin(PPM_pic *ppm, FILE *out)
{
	fprintf(out, "P6\n%d %d\n%d\n", ppm->width, ppm->height, ppm->max_value);

	for (int i = 0; i < ppm->height; i++) {
		for (int j = 0; j < ppm->width; j++) {
			fwrite(&ppm->a[i][j].R, 1, 1, out);
			fwrite(&ppm->a[i][j].G, 1, 1, out);
			fwrite(&ppm->a[i][j].B, 1, 1, out);
		}
	}
}

void save(PGM_pic *pgm, PPM_pic *ppm, char command[200])
{
	if (!(pgm->a) && !(ppm->a)) {
		printf("No image loaded\n");
		return;
	}
	int ascii = 0;
	char *token = strtok(command, "\n ");
	token = strtok(NULL, "\n ");
	char file_name[50];
	strcpy(file_name, token);
	token = strtok(NULL, "\n ");

	FILE *out;

	if (token && !strcmp(token, "ascii"))
		ascii = 1;

	if (ascii == 1) {
		out = fopen(file_name, "w");
		if (!out)
			exit(1);
		if (pgm->a)
			save_pgm_ascii(pgm, out);
		else
			save_ppm_ascii(ppm, out);
	} else {
		out = fopen(file_name, "wb");
		if (!out)
			exit(1);
		if (pgm->a)
			save_pgm_bin(pgm, out);
		else
			save_ppm_bin(ppm, out);
	}
	fclose(out);
	printf("Saved %s\n", file_name);
}

/* se verifica ce poza este incarcata in memorie si se iese din program
   daca a fost incarcata cel putin o poza inainte */
void exit_program(PGM_pic *pgm, PPM_pic *ppm, int ok_exit)
{
	if (!pgm->a && !ppm->a && ok_exit) {
		printf("No image loaded\n");
		exit(0);
	}
	if (pgm->a) {
		pgm->a = free_matrix(pgm->a, pgm->height);
		exit(0);
	} else if (ppm->a) {
		ppm->a = free_matrix_rgb(ppm->a, ppm->height);
		exit(0);
	}
	printf("No image loaded\n");
}

// se roteste o matrice
void rotate_pgm(PGM_pic *pgm, int full)
{
	int **new_pic = NULL;
	new_pic = alloc_matrix(new_pic, pgm->y2 - pgm->y1, pgm->x2 - pgm->x1);
	int m = 0, n = 0;

	for (int i = pgm->y1; i < pgm->y2; i++) {
		n = 0;
		for (int j = pgm->x1; j < pgm->x2; j++)
			new_pic[m][n++] = pgm->a[i][j];
		m++;
	}

	new_pic = transpose_pgm(new_pic, m, n);
	/* daca este selectata intreaga poza, se face swap intre
	   dimensiuni si se realoca o matrice cu noile dimensiuni */
	if (full == 1) {
		pgm->a = free_matrix(pgm->a, pgm->height);
		pgm->x1 = 0;
		pgm->x2 = pgm->height;
		pgm->y1 = 0;
		pgm->y2 = pgm->width;
		swap_int(&pgm->height, &pgm->width);
		swap_int(&m, &n);
		pgm->a = alloc_matrix(pgm->a, pgm->height, pgm->width);
	}
	/* se pastreaza pe rand elementele unei coloane intr-un vector si se
	   pun inapoi in matrice in ordine inversa */
	for (int i = 0; i < n; i++) {
		int *v = malloc(m * sizeof(int));
		if (!v) {
			printf("Failed to allocate memory\n");
			exit(1);
		}
		for (int j = 0; j < m; j++)
			v[j] = new_pic[j][i];
		for (int j = 0; j < m; j++)
			new_pic[j][i] = v[m - j - 1];
		free(v);
	}

	int x = pgm->y1;
	int y = pgm->x1;

	// se copiaza imaginea rotita peste imaginea originala
	for (int i = 0; i < m; i++) {
		y = pgm->x1;
		for (int j = 0; j < n; j++)
			pgm->a[x][y++] = new_pic[i][j];
		x++;
	}
	new_pic = free_matrix(new_pic, m);
}

void rotate_ppm(PPM_pic *ppm, int full)
{
	RGB **new_pic = NULL;
	new_pic = alloc_matrix_rgb(new_pic, ppm->y2 - ppm->y1, ppm->x2 - ppm->x1);
	int m = 0, n = 0;

	for (int i = ppm->y1; i < ppm->y2; i++) {
		n = 0;
		for (int j = ppm->x1; j < ppm->x2; j++) {
			new_pic[m][n].R = ppm->a[i][j].R;
			new_pic[m][n].G = ppm->a[i][j].G;
			new_pic[m][n].B = ppm->a[i][j].B;
			n++;
		}
		m++;
	}

	new_pic = transpose_ppm(new_pic, m, n);
	/* daca este selectata intreaga poza, se face swap intre
	   dimensiuni si se realoca o matrice cu noile dimensiuni */
	if (full == 1) {
		ppm->a = free_matrix_rgb(ppm->a, ppm->height);
		ppm->x1 = 0;
		ppm->x2 = ppm->height;
		ppm->y1 = 0;
		ppm->y2 = ppm->width;
		swap_int(&ppm->height, &ppm->width);
		swap_int(&m, &n);
		ppm->a = alloc_matrix_rgb(ppm->a, ppm->height, ppm->width);
	}

	/* se pastreaza pe rand elementele unei coloane intr-un vector si se
	   pun inapoi in matrice in ordine inversa */
	for (int i = 0; i < n; i++) {
		RGB *v = malloc(m * sizeof(RGB));
		if (!v) {
			printf("Failed to allocate memory\n");
			exit(1);
		}
		for (int j = 0; j < m; j++) {
			v[j].R = new_pic[j][i].R;
			v[j].G = new_pic[j][i].G;
			v[j].B = new_pic[j][i].B;
		}
		for (int j = 0; j < m; j++) {
			new_pic[j][i].R = v[m - j - 1].R;
			new_pic[j][i].G = v[m - j - 1].G;
			new_pic[j][i].B = v[m - j - 1].B;
		}
		free(v);
	}

	int x = ppm->y1;
	int y = ppm->x1;

	for (int i = 0; i < m; i++) {
		y = ppm->x1;
		for (int j = 0; j < n; j++) {
			ppm->a[x][y].R = new_pic[i][j].R;
			ppm->a[x][y].G = new_pic[i][j].G;
			ppm->a[x][y].B = new_pic[i][j].B;
			y++;
		}
		x++;
	}
	new_pic = free_matrix_rgb(new_pic, m);
}

void rotate(PGM_pic *pgm, PPM_pic *ppm, char command[200])
{
	if (!pgm->a && !ppm->a) {
		printf("No image loaded\n");
		return;
	}
	char *token = strtok(command, " ");
	token = strtok(NULL, " ");
	int angle = atoi(token), full = 0;

	if (!square_matrix(pgm, ppm, &full)) {
		printf("The selection must be square\n");
		return;
	}

	if (angle == 0 || abs(angle) == 360) {
		printf("Rotated %d\n", angle);
		return;
	}

	if (angle % 90 != 0) {
		printf("Unsupported rotation angle\n");
		return;
	}

	if (pgm->a) {
		if (angle < 0) {
			for (int i = 0; i > angle; i -= 90)
				rotate_pgm(pgm, full);
		} else {
			/* +90 = de 3 ori -90
			   +180 = de 6 ori -90
			   +270 = de 9 ori -90 */
			int n = angle / 30;
			for (int i = 0; i < n; i++)
				rotate_pgm(pgm, full);
		}
	} else {
		if (angle < 0) {
			for (int i = 0; i > angle; i -= 90)
				rotate_ppm(ppm, full);
		} else {
			int n = angle / 30;
			for (int i = 0; i < n; i++)
				rotate_ppm(ppm, full);
		}
	}
	printf("Rotated %d\n", angle);
}
