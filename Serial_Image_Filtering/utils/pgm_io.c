#include "pgm_io.h"
#include <string.h>

void read_pgm(const char *filename, unsigned char image[HEIGHT][WIDTH]) {
    FILE *pgm = fopen(filename, "rb");
    if (!pgm) {
        perror("Error opening input image");
        return;
    }

    char format[3];
    fscanf(pgm, "%s", format); 
    if (strcmp(format, "P5") != 0) {
        printf("Unsupported format!\n");
        fclose(pgm);
        return;
    }

    int width, height, maxval;
    fscanf(pgm, "%d %d %d", &width, &height, &maxval);
    fgetc(pgm); 

    fread(image, sizeof(unsigned char), WIDTH * HEIGHT, pgm);
    fclose(pgm);
}

void write_pgm(const char *filename, unsigned char image[HEIGHT][WIDTH]) {
    FILE *pgm = fopen(filename, "wb");
    if (!pgm) {
        perror("Error writing output image");
        return;
    }

    fprintf(pgm, "P5\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(image, sizeof(unsigned char), WIDTH * HEIGHT, pgm);
    fclose(pgm);
}
