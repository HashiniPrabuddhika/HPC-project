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

    // Ensure grayscale
    if (bit_depth == 16)
        png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        png_set_rgb_to_gray_fixed(png, 1, -1, -1);
    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    png_read_update_info(png, info);
    int rowbytes = png_get_rowbytes(png, info);

    png_bytep *rows = malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        rows[y] = malloc(rowbytes);
    }

    png_read_image(png, rows);
    fclose(fp);

    // Save to PGM (ASCII P2)
    FILE *out = fopen(output_file, "w");
    if (!out) {
        perror("PGM write error");
        exit(1);
    }

    fprintf(out, "P2\n%d %d\n255\n", width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fprintf(out, "%d ", rows[y][x]);
        }
        fprintf(out, "\n");
        free(rows[y]);
    }

    free(rows);
    fclose(out);
    png_destroy_read_struct(&png, &info, NULL);
    printf("✅ Converted %s to %s\n", input_file, output_file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s input.png output.pgm\n", argv[0]);
        return 1;
    }

    convert_png_to_pgm(argv[1], argv[2]);
    return 0;
}
