#include <stdio.h>
#include <stdlib.h>
#include <png.h>

unsigned char rgb_to_gray(unsigned char r, unsigned char g, unsigned char b) {
    return (unsigned char)(0.3 * r + 0.59 * g + 0.11 * b);
}

int main() {
    FILE *fp = fopen("image.png", "rb");
    if (!fp) {
        perror("File open error");
        return 1;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    png_init_io(png, fp);
    png_read_info(png, info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16) png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    png_read_update_info(png, info);

    png_bytep *rows = malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++)
        rows[y] = malloc(png_get_rowbytes(png, info));

    png_read_image(png, rows);
    fclose(fp);

    FILE *out = fopen("grayscale.pgm", "w");
    fprintf(out, "P2\n%d %d\n255\n", width, height);
    for (int y = 0; y < height; y++) {
        png_bytep row = rows[y];
        for (int x = 0; x < width; x++) {
            png_bytep px = &(row[x * 4]);
            unsigned char gray = rgb_to_gray(px[0], px[1], px[2]);
            fprintf(out, "%d ", gray);
        }
        fprintf(out, "\n");
        free(rows[y]);
    }
    free(rows);
    png_destroy_read_struct(&png, &info, NULL);
    fclose(out);

    printf("Grayscale image saved to grayscale.pgm\n");
    return 0;
}
