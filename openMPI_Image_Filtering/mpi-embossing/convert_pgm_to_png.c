#include <stdio.h>
#include <stdlib.h>
#include <png.h>

void convert_pgm_to_png(const char *pgm_file, const char *png_file) {
    FILE *fp = fopen(pgm_file, "r");
    if (!fp) {
        perror("PGM open error");
        exit(1);
    }

    char format[3];
    int width, height, max_val;
    fscanf(fp, "%2s\n%d %d\n%d\n", format, &width, &height, &max_val);

    unsigned char *pixels = malloc(width * height);
    for (int i = 0; i < width * height; i++) {
        int val;
        fscanf(fp, "%d", &val);
        pixels[i] = (unsigned char)val;
    }
    fclose(fp);

    FILE *out_fp = fopen(png_file, "wb");
    if (!out_fp) {
        perror("PNG write error");
        exit(1);
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    if (setjmp(png_jmpbuf(png))) exit(1);

    png_init_io(png, out_fp);
    png_set_IHDR(png, info, width, height, 8,
                 PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    png_bytep *rows = malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        rows[y] = pixels + y * width;
    }

    png_write_image(png, rows);
    png_write_end(png, NULL);

    free(rows);
    free(pixels);
    fclose(out_fp);
    png_destroy_write_struct(&png, &info);
    printf("Converted %s → %s\n", pgm_file, png_file);
}

int main() {
    convert_pgm_to_png("emboss_output.pgm", "emboss_output.png");
    return 0;
}
