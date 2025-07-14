#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "pgm_io.h"

void convert_png_to_pgm(const char *png_filename, const char *pgm_filename) {
    FILE *fp = fopen(png_filename, "rb");
    if (!fp) {
        perror("Error opening PNG file");
        return;
    }

    png_byte header[8];
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)) {
        printf("❌ Not a PNG file.\n");
        fclose(fp);
        return;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        printf("❌ Failed to create PNG read struct.\n");
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        printf("❌ Failed to create PNG info struct.\n");
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        printf("❌ Error during PNG read.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

   
    if (bit_depth == 16)
        png_set_strip_16(png_ptr);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);

    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);

    
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGB_ALPHA ||
        color_type == PNG_COLOR_TYPE_PALETTE || png_get_channels(png_ptr, info_ptr) >= 3) {
        png_set_rgb_to_gray_fixed(png_ptr, 1, -1, -1); 
    }

    if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA || png_get_channels(png_ptr, info_ptr) == 2)
        png_set_strip_alpha(png_ptr); 

    png_read_update_info(png_ptr, info_ptr);

    if (width != WIDTH || height != HEIGHT) {
        printf("❌ Image must be %dx%d. Yours is %dx%d\n", WIDTH, HEIGHT, width, height);
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return;
    }

    png_bytep *row_pointers = malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = malloc(png_get_rowbytes(png_ptr, info_ptr));
    }

    png_read_image(png_ptr, row_pointers);
    fclose(fp);

    unsigned char image[HEIGHT][WIDTH];
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            image[y][x] = row_pointers[y][x]; 
        }
    }

    write_pgm(pgm_filename, image);
    printf("Converted %s ➝ %s\n", png_filename, pgm_filename);

    for (int y = 0; y < height; y++) free(row_pointers[y]);
    free(row_pointers);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}
