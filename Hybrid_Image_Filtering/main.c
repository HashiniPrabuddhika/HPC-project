#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/pgm_io.h"
#include "include/filters.h"

#define WIDTH 512
#define HEIGHT 512

unsigned char** allocate_2D(int height, int width) {
    unsigned char **array = malloc(height * sizeof(unsigned char*));
    for (int i = 0; i < height; i++)
        array[i] = malloc(width * sizeof(unsigned char));
    return array;
}

void free_2D(unsigned char **array, int height) {
    for (int i = 0; i < height; i++)
        free(array[i]);
    free(array);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filter_name>\n", argv[0]);
        return 1;
    }

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunk = HEIGHT / size;

    unsigned char **input = allocate_2D(chunk + 2, WIDTH);
    unsigned char **output = allocate_2D(chunk, WIDTH);

    if (rank == 0) {
        unsigned char full_input[HEIGHT][WIDTH];
        read_pgm("images/input/grayscale.pgm", full_input);

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < chunk + 2; j++) {
                int src_row = i * chunk + j - 1;
                unsigned char temp_row[WIDTH];
                for (int k = 0; k < WIDTH; k++) {
                    temp_row[k] = (src_row < 0 || src_row >= HEIGHT) ? 0 : full_input[src_row][k];
                }
                if (i == 0)
                    memcpy(input[j], temp_row, WIDTH);
                else
                    MPI_Send(temp_row, WIDTH, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        for (int i = 0; i < chunk + 2; i++) {
            MPI_Recv(input[i], WIDTH, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    if (strcmp(argv[1], "smooth") == 0)
        apply_smoothing(input, output, chunk, WIDTH);
    else if (strcmp(argv[1], "sharpen") == 0)
        apply_sharpening(input, output, chunk, WIDTH);
    else if (strcmp(argv[1], "edge") == 0)
        apply_edge_detection(input, output, chunk, WIDTH);
    else if (strcmp(argv[1], "emboss") == 0)
        apply_emboss(input, output, chunk, WIDTH);
    else {
        if (rank == 0)
            printf("❌ Unknown filter: %s\n", argv[1]);
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        unsigned char full_output[HEIGHT][WIDTH];
        for (int i = 0; i < chunk; i++)
            memcpy(full_output[i], output[i], WIDTH);
        for (int i = 1; i < size; i++) {
            unsigned char *recv_buffer = malloc(chunk * WIDTH);
            MPI_Recv(recv_buffer, chunk * WIDTH, MPI_UNSIGNED_CHAR, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int r = 0; r < chunk; r++)
                memcpy(full_output[i * chunk + r], recv_buffer + r * WIDTH, WIDTH);
            free(recv_buffer);

        }
        char output_file[100];
        sprintf(output_file, "images/output/%s_output.pgm", argv[1]);
        write_pgm(output_file, full_output);
        printf("✅ %s filter saved to %s\n", argv[1], output_file);
    } else {
            unsigned char *send_buffer = malloc(chunk * WIDTH);
            for (int r = 0; r < chunk; r++)
                memcpy(send_buffer + r * WIDTH, output[r], WIDTH);
            MPI_Send(send_buffer, chunk * WIDTH, MPI_UNSIGNED_CHAR, 0, 1, MPI_COMM_WORLD);
            free(send_buffer);
            }

    free_2D(input, chunk + 2);
    free_2D(output, chunk);
    MPI_Finalize();
    return 0;
}
