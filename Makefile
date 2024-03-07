#Copyright Apostol Rares-Andrei 311CAa 2022-2023
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra
build: image_editor

image_editor: image_editor.c
	$(CC) $(CFLAGS) image_editor.c functions.c commands.c -o image_editor -lm

clean:
	rm -f image_editor
