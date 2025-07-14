#include <stdio.h>
#include <stdlib.h>
#include "pgm_io.h"

void read_pgm(const char *filename, unsigned char image[512][512]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("Error opening file"); exit(1); }

    char ch;
    int width, height, max_val;
    fscanf(fp, "P2\n");
    while ((ch = fgetc(fp)) == '#') while (fgetc(fp) != '\n');
    ungetc(ch, fp);
    fscanf(fp, "%d %d\n%d\n", &width, &height, &max_val);

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            fscanf(fp, "%hhu", &image[i][j]);

    fclose(fp);
}

void write_pgm(const char *filename, unsigned char image[512][512]) {
    FILE *fp = fopen(filename, "w");
    if (!fp) { perror("Error writing file"); exit(1); }

    fprintf(fp, "P2\n512 512\n255\n");
    for (int i = 0; i < 512; i++) {
        for (int j = 0; j < 512; j++)
            fprintf(fp, "%d ", image[i][j]);
        fprintf(fp, "\n");
    }
    fclose(fp);
}
