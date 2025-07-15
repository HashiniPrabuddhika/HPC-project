#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WIDTH 512
#define HEIGHT 512

void read_pgm(const char *filename, unsigned char image[HEIGHT][WIDTH]) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, " Failed to open file: %s\n", filename);
        exit(1);
    }

    char magic[3] = {0};
    if (fscanf(fp, "%2s", magic) != 1) {
        fprintf(stderr, " Failed to read magic number from %s\n", filename);
        fclose(fp);
        exit(1);
    }

    if (strcmp(magic, "P2") != 0 && strcmp(magic, "P5") != 0) {
        fprintf(stderr, " Unsupported PGM format in %s: %s\n", filename, magic);
        fclose(fp);
        exit(1);
    }

    int c = fgetc(fp);
    while (c == '#') {
        while (fgetc(fp) != '\n');
        c = fgetc(fp);
    }
    ungetc(c, fp);

    int width = 0, height = 0, maxval = 0;
    if (fscanf(fp, "%d %d", &width, &height) != 2 || fscanf(fp, "%d", &maxval) != 1) {
        fprintf(stderr, " Invalid header in %s\n", filename);
        fclose(fp);
        exit(1);
    }

    if (width != WIDTH || height != HEIGHT || maxval > 255) {
        fprintf(stderr, " Image size or maxval mismatch in %s\n", filename);
        fclose(fp);
        exit(1);
    }

    fgetc(fp); 

    if (strcmp(magic, "P5") == 0) {
        size_t read = fread(image, sizeof(unsigned char), WIDTH * HEIGHT, fp);
        if (read != WIDTH * HEIGHT) {
            fprintf(stderr, " Failed to read binary pixel data from %s\n", filename);
            fclose(fp);
            exit(1);
        }
    } else {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int pixel;
                if (fscanf(fp, "%d", &pixel) != 1) {
                    fprintf(stderr, " Failed to read text pixel data at (%d,%d) in %s\n", i, j, filename);
                    fclose(fp);
                    exit(1);
                }
                image[i][j] = (unsigned char)pixel;
            }
        }
    }

    fclose(fp);
}

double calculate_rmse(unsigned char img1[HEIGHT][WIDTH], unsigned char img2[HEIGHT][WIDTH]) {
    double mse = 0.0;
    int count = 0;
    for (int i = 1; i < HEIGHT - 1; i++) {
        for (int j = 1; j < WIDTH - 1; j++) {
            int diff = img1[i][j] - img2[i][j];
            mse += diff * diff;
            count++;
        }
    }
    mse /= count;
    return sqrt(mse);
}

void compare_filter(const char *filter_name) {
    unsigned char serial[HEIGHT][WIDTH];
    unsigned char openmp[HEIGHT][WIDTH];
    char serial_path[256];
    char openmp_path[256];

    sprintf(serial_path, "../../Serial_Image_Filtering/images/output/%s_output.pgm", filter_name);
    sprintf(openmp_path, "../../OpenMP_Image_Filtering/images/output/%s_output.pgm", filter_name);

    read_pgm(serial_path, serial);
    read_pgm(openmp_path, openmp);

    double rmse = calculate_rmse(serial, openmp);
    printf("RMSE between Serial and OpenMP (%s): %.6f\n", filter_name, rmse);
}

int main() {

    compare_filter("smooth");
    compare_filter("sharpen");
    compare_filter("edge");
    compare_filter("emboss");   

    return 0;
}
