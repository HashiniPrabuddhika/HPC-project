#ifndef PGM_IO_H
#define PGM_IO_H

#include <stdio.h>

#define WIDTH 512
#define HEIGHT 512

void read_pgm(const char *filename, unsigned char image[HEIGHT][WIDTH]);
void write_pgm(const char *filename, unsigned char image[HEIGHT][WIDTH]);
void convert_png_to_pgm(const char *png_filename, const char *pgm_filename);
void convert_pgm_to_png(const char *pgm_file, const char *png_file);

#endif
