#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "pgm_io.h"

void convert_pgm_to_png(const char *pgm_file, const char *png_file) {
    FILE *pgm = fopen(pgm_file, "rb");
    if (!pgm) {
        perror("Error opening PGM file");
        return;
    }

    char header[3];
    int width, height, maxval;
    fscanf(pgm, "%s", header);
    fscanf(pgm, "%d %d", &width, &height);
    fscanf(pgm, "%d", &maxval);
    fgetc(pgm); 

    if (width != WIDTH || height != HEIGHT) {
        printf("❌ Image size mismatch: expected %dx%d, got %dx%d\n", WIDTH, HEIGHT, width, height);
        fclose(pgm);
        return;
    }

    unsigned char image[HEIGHT][WIDTH];
    fread(image, sizeof(unsigned char), WIDTH * HEIGHT, pgm);
    fclose(pgm);

    FILE *fp = fopen(png_file, "wb");
    if (!fp) {
        perror("Error opening PNG file");
        return;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        printf("❌ png_create_write_struct failed.\n");
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        printf("❌ png_create_info_struct failed.\n");
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        printf("❌ Error during PNG write.\n");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);
    png_set_IHDR(
        png_ptr, info_ptr,
        WIDTH, HEIGHT,
        8,
        PNG_COLOR_TYPE_GRAY,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );

    png_write_info(png_ptr, info_ptr);

    png_bytep row_pointers[HEIGHT];
    for (int y = 0; y < HEIGHT; y++) {
        row_pointers[y] = image[y];
    }

    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, NULL);

    fclose(fp);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    printf("Converted %s ➝ %s\n", pgm_file, png_file);
}
