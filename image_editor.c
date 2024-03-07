// Apostol Rares-Andrei 311CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "structs.h"
#include "functions.h"
#include "commands.h"

int main(void)
{
	char command[200];
	int ok_exit = 0;
	FILE *fptr = NULL;
	PGM_pic pgm;
	PGM_pic *pgmp = &pgm;
	PPM_pic ppm;
	PPM_pic *ppmp = &ppm;
	ppm.a = NULL;
	pgm.a = NULL;
	while (1) {
		fgets(command, 200, stdin);
		if (!strncmp(command, "LOAD ", 5))
			load(fptr, pgmp, ppmp, command, &ok_exit);
		else if (check_select_all(command))
			select_all(ppmp, pgmp);
		else if (check_select(command))
			select(ppmp, pgmp, command);
		else if (!strncmp(command, "HISTOGRAM", 9))
			histogram(ppmp, pgmp, command);
		else if (check_equalize(command))
			equalize(pgmp, ppmp);
		else if (check_crop(command))
			crop(pgmp, ppmp);
		else if (!strncmp(command, "APPLY", 5))
			apply(pgmp, ppmp, command);
		else if (!strncmp(command, "SAVE ", 5))
			save(pgmp, ppmp, command);
		else if (check_exit(command))
			exit_program(pgmp, ppmp, ok_exit);
		else if (check_rotate(command))
			rotate(pgmp, ppmp, command);
		else
			printf("Invalid command\n");
	}

	return 0;
}
