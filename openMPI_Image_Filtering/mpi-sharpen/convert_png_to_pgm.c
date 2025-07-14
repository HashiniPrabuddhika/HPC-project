#include <stdio.h>
#include <stdlib.h>
#include <png.h>

void convert_png_to_pgm(const char *input_file, const char *output_file) {
    FILE *fp = fopen(input_file, "rb");
    if (!fp) {
        perror("File open error");
        exit(1);
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    if (!png || !info || setjmp(png_jmpbuf(png))) {
        fprintf(stderr, "PNG init failed\n");
        exit(1);
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16)
        png_set_strip_16(png);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        png_set_rgb_to_gray_fixed(png, 1, -1, -1);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    png_read_update_info(png, info);

    png_bytep *rows = (png_bytep *)malloc(sizeof(png_bytep) * height);
    int rowbytes = png_get_rowbytes(png, info);
    for (int y = 0; y < height; y++) {
        rows[y] = (png_byte *)malloc(rowbytes);
    }

    png_read_image(png, rows);
    fclose(fp);

    FILE *out = fopen(output_file, "w");
    if (!out) {
        perror("PGM write error");
        exit(1);
    }

    fprintf(out, "P2\n%d %d\n255\n", width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            png_byte gray = rows[y][x];
            fprintf(out, "%d ", gray);
        }
        fprintf(out, "\n");
        free(rows[y]);
    }
    free(rows);
    fclose(out);
    png_destroy_read_struct(&png, &info, NULL);
}

int main() {
    convert_png_to_pgm("input.png", "grayscale.pgm");
    printf("Converted input.png to grayscale.pgm\n");
    return 0;
}
