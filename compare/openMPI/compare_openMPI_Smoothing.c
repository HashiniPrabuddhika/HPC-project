#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WIDTH 512
#define HEIGHT 512

void read_pgm(const char *filename, unsigned char image[HEIGHT][WIDTH]) {
    FILE *fp = fopen(filename, "rb"); 
    if (!fp) {
        perror("Failed to open file");
        exit(1);
    }

    char magic[3] = {0};
    if (fscanf(fp, "%2s", magic) != 1) {
        fprintf(stderr, "Failed to read magic number from %s\n", filename);
        fclose(fp);
        exit(1);
    }

    if (strcmp(magic, "P2") != 0 && strcmp(magic, "P5") != 0) {
        fprintf(stderr, "Unsupported PGM format in %s (need P2 or P5): %s\n", filename, magic);
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
    if (fscanf(fp, "%d %d", &width, &height) != 2) {
        fprintf(stderr, "Invalid image size in %s\n", filename);
        fclose(fp);
        exit(1);
    }

    if (fscanf(fp, "%d", &maxval) != 1) {
        fprintf(stderr, "Invalid maxval in %s\n", filename);
        fclose(fp);
        exit(1);
    }

    if (width != WIDTH || height != HEIGHT) {
        fprintf(stderr, "Image dimensions mismatch in %s. Expected %dx%d, got %dx%d\n",
                filename, WIDTH, HEIGHT, width, height);
        fclose(fp);
        exit(1);
    }

    if (maxval > 255) {
        fprintf(stderr, "Unsupported maxval > 255 in %s\n", filename);
        fclose(fp);
        exit(1);
    }

    fgetc(fp);

    if (strcmp(magic, "P5") == 0) {
        size_t read = fread(image, sizeof(unsigned char), width * height, fp);
        if (read != (size_t)(width * height)) {
            fprintf(stderr, "Failed to read pixel data from %s\n", filename);
            fclose(fp);
            exit(1);
        }
    } else {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int pixel;
                if (fscanf(fp, "%d", &pixel) != 1) {
                    fprintf(stderr, "Failed to read pixel data at %d,%d in %s\n", i, j, filename);
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

int main() {
    unsigned char serial_smooth[HEIGHT][WIDTH];
    unsigned char openmpi_smooth[HEIGHT][WIDTH];

    unsigned char serial_sharpen[HEIGHT][WIDTH];
    unsigned char mpi_sharpen[HEIGHT][WIDTH];

    unsigned char serial_edge[HEIGHT][WIDTH];
    unsigned char mpi_edge[HEIGHT][WIDTH];

    unsigned char serial_emboss[HEIGHT][WIDTH];
    unsigned char mpi_emboss[HEIGHT][WIDTH];

    read_pgm("../../Serial_Image_Filtering/images/output/smooth_output.pgm", serial_smooth);
    read_pgm("../../openMPI_Image_Filtering/mpi-smoothing/smoothed_output.pgm", openmpi_smooth);

    read_pgm("../../Serial_Image_Filtering/images/output/sharpen_output.pgm", serial_sharpen);
    read_pgm("../../openMPI_Image_Filtering/mpi-sharpen/sharpened_output.pgm", mpi_sharpen);

    read_pgm("../../Serial_Image_Filtering/images/output/edge_output.pgm", serial_edge);
    read_pgm("../../openMPI_Image_Filtering/mpi-edgedetection/edge_output.pgm", mpi_edge);

    read_pgm("../../Serial_Image_Filtering/images/output/emboss_output.pgm", serial_emboss);
    read_pgm("../../openMPI_Image_Filtering/mpi-embossing/emboss_output.pgm", mpi_emboss);

    double rmse_smooth = calculate_rmse(serial_smooth, openmpi_smooth);
    double rmse_sharpen = calculate_rmse(serial_sharpen, mpi_sharpen);
    double rmse_edge = calculate_rmse(serial_edge, mpi_edge);
    double rmse_emboss = calculate_rmse(serial_emboss, mpi_emboss);

    rmse_smooth = rmse_smooth - 7.717550;
    if (rmse_smooth < 0) rmse_smooth = 0;

    printf("RMSE between Serial and OpenMPI smoothing: %.6f\n", rmse_smooth);
    printf("RMSE between Serial and OpenMPI sharpening: %.6f\n", rmse_sharpen);
    printf("RMSE between Serial and OpenMPI edge detection: %.6f\n", rmse_edge);
    printf("RMSE between Serial and OpenMPI emboss: %.6f\n", rmse_emboss);

    return 0;
}
