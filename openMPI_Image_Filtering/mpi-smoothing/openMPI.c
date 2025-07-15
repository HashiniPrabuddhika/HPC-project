// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <mpi.h>

// #define KERNEL_SIZE 3
// #define KERNEL_RADIUS 1
// #define GAUSSIAN_SUM 16

// int gaussian[KERNEL_SIZE][KERNEL_SIZE] = {
//     { 1, 2, 1 },
//     { 2, 4, 2 },
//     { 1, 2, 1 }
// };

// int smooth_pixel(unsigned char **image, int x, int y, int height, int width) {
//     int sum = 0;

//     for (int i = -KERNEL_RADIUS; i <= KERNEL_RADIUS; i++) {
//         for (int j = -KERNEL_RADIUS; j <= KERNEL_RADIUS; j++) {
//             int ni = x + i;
//             int nj = y + j;
//             if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
//                 sum += image[ni][nj] * gaussian[i + KERNEL_RADIUS][j + KERNEL_RADIUS];
//             }
//         }
//     }
//     return sum / GAUSSIAN_SUM;
// }

// int main(int argc, char *argv[]) {
//     int rank, size;
//     int width, height, maxval;
//     double start_time, end_time, max_time;

//     MPI_Init(&argc, &argv);
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &size);

//     unsigned char **image = NULL;

//     if (rank == 0) {
//         FILE *fp = fopen("grayscale.pgm", "r");
//         if (!fp) {
//             perror("Can't open grayscale.pgm");
//             MPI_Abort(MPI_COMM_WORLD, 1);
//         }

//         char magic[3];
//         fscanf(fp, "%s", magic);
//         fscanf(fp, "%d %d", &width, &height);
//         fscanf(fp, "%d", &maxval);

//         image = malloc(height * sizeof(unsigned char *));
//         for (int i = 0; i < height; i++) {
//             image[i] = malloc(width);
//             for (int j = 0; j < width; j++) {
//                 fscanf(fp, "%hhu", &image[i][j]);
//             }
//         }
//         fclose(fp);
//     }

//     MPI_Barrier(MPI_COMM_WORLD);
//     start_time = MPI_Wtime();

//     MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
//     MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);

//     int rows_per_proc = height / size;
//     int extra = height % size;

//     int start = rank * rows_per_proc + (rank < extra ? rank : extra);
//     int local_rows = rows_per_proc + (rank < extra ? 1 : 0);

//     unsigned char **local = malloc((local_rows + 2 * KERNEL_RADIUS) * sizeof(unsigned char *));
//     for (int i = 0; i < local_rows + 2 * KERNEL_RADIUS; i++) {
//         local[i] = calloc(width, sizeof(unsigned char));
//     }

//     if (rank == 0) {
//         for (int p = 0; p < size; p++) {
//             int p_start = p * rows_per_proc + (p < extra ? p : extra);
//             int p_rows = rows_per_proc + (p < extra ? 1 : 0);

//             for (int i = 0; i < p_rows; i++) {
//                 if (p == 0) {
//                     memcpy(local[i + KERNEL_RADIUS], image[p_start + i], width);
//                 } else {
//                     MPI_Send(image[p_start + i], width, MPI_UNSIGNED_CHAR, p, 0, MPI_COMM_WORLD);
//                 }
//             }
//         }
//     } else {
//         for (int i = 0; i < local_rows; i++) {
//             MPI_Recv(local[i + KERNEL_RADIUS], width, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//         }
//     }

//     if (rank > 0)
//         MPI_Sendrecv(local[KERNEL_RADIUS], width, MPI_UNSIGNED_CHAR, rank - 1, 0,
//                      local[0], width, MPI_UNSIGNED_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

//     if (rank < size - 1)
//         MPI_Sendrecv(local[KERNEL_RADIUS + local_rows - 1], width, MPI_UNSIGNED_CHAR, rank + 1, 0,
//                      local[KERNEL_RADIUS + local_rows], width, MPI_UNSIGNED_CHAR, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

//     unsigned char **smoothed = malloc(local_rows * sizeof(unsigned char *));
//     for (int i = 0; i < local_rows; i++) {
//         smoothed[i] = malloc(width);
//         for (int j = 0; j < width; j++) {
//             smoothed[i][j] = smooth_pixel(local, i + KERNEL_RADIUS, j, local_rows + 2 * KERNEL_RADIUS, width);
//         }
//     }

//     if (rank == 0) {
//         unsigned char **result = malloc(height * sizeof(unsigned char *));
//         for (int i = 0; i < height; i++)
//             result[i] = malloc(width);

//         for (int i = 0; i < local_rows; i++)
//             memcpy(result[start + i], smoothed[i], width);

//         for (int p = 1; p < size; p++) {
//             int p_start = p * rows_per_proc + (p < extra ? p : extra);
//             int p_rows = rows_per_proc + (p < extra ? 1 : 0);

//             for (int i = 0; i < p_rows; i++) {
//                 MPI_Recv(result[p_start + i], width, MPI_UNSIGNED_CHAR, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//             }
//         }

//         FILE *out = fopen("smoothed_output.pgm", "w");
//         fprintf(out, "P2\n%d %d\n255\n", width, height);
//         for (int i = 0; i < height; i++) {
//             for (int j = 0; j < width; j++) {
//                 fprintf(out, "%d ", result[i][j]);
//             }
//             fprintf(out, "\n");
//             free(result[i]);
//         }
//         free(result);
//         fclose(out);
//         printf("Smoothing complete. Saved as smoothed_output.pgm\n");
//     } else {
//         for (int i = 0; i < local_rows; i++) {
//             MPI_Send(smoothed[i], width, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
//         }
//     }

//     MPI_Barrier(MPI_COMM_WORLD);
//     end_time = MPI_Wtime();

//     double local_time = end_time - start_time;
//     MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

//     if (rank == 0) {
//         printf("Total execution time: %.6f seconds\n", max_time);
//     }

//     for (int i = 0; i < local_rows + 2 * KERNEL_RADIUS; i++) free(local[i]);
//     free(local);
//     for (int i = 0; i < local_rows; i++) free(smoothed[i]);
//     free(smoothed);

//     MPI_Finalize();
//     return 0;
// }





#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define WIDTH 512
#define HEIGHT 512
#define KERNEL_SIZE 3
#define KERNEL_RADIUS 1
#define GAUSSIAN_SUM 16

int gaussian[KERNEL_SIZE][KERNEL_SIZE] = {
    { 1, 2, 1 },
    { 2, 4, 2 },
    { 1, 2, 1 }
};

int smooth_pixel(unsigned char *image, int x, int y, int height, int width) {
    int sum = 0;
    for (int i = -KERNEL_RADIUS; i <= KERNEL_RADIUS; i++) {
        for (int j = -KERNEL_RADIUS; j <= KERNEL_RADIUS; j++) {
            int ni = x + i;
            int nj = y + j;
            if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
                sum += image[ni * width + nj] * gaussian[i + KERNEL_RADIUS][j + KERNEL_RADIUS];
            }
        }
    }
    return sum / GAUSSIAN_SUM;
}

int main(int argc, char *argv[]) {
    int rank, size;
    int width = WIDTH, height = HEIGHT;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    unsigned char *full_image = NULL;
    if (rank == 0) {
        FILE *fp = fopen("grayscale.pgm", "r");
        if (!fp) {
            perror("Cannot open grayscale.pgm");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        char magic[3];
        int maxval;
        fscanf(fp, "%s", magic);
        fscanf(fp, "%d %d", &width, &height);
        fscanf(fp, "%d", &maxval);

        full_image = (unsigned char *)malloc(width * height);
        for (int i = 0; i < width * height; i++) {
            fscanf(fp, "%hhu", &full_image[i]);
        }
        fclose(fp);
    }

    MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int *sendcounts = malloc(size * sizeof(int));
    int *displs = malloc(size * sizeof(int));
    int rows_per_proc = height / size;
    int extra = height % size;

    int local_rows = rows_per_proc + (rank < extra ? 1 : 0);
    int start_row = rank * rows_per_proc + (rank < extra ? rank : extra);
    int padded_rows = local_rows + 2 * KERNEL_RADIUS;

    for (int i = 0; i < size; i++) {
        int rows = rows_per_proc + (i < extra ? 1 : 0);
        sendcounts[i] = rows * width;
        displs[i] = (i * rows_per_proc + (i < extra ? i : extra)) * width;
    }

    unsigned char *local_padded = calloc(padded_rows * width, sizeof(unsigned char));
    unsigned char *local_output = malloc(local_rows * width);

    MPI_Scatterv(full_image, sendcounts, displs, MPI_UNSIGNED_CHAR,
                 &local_padded[KERNEL_RADIUS * width], local_rows * width, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    if (rank > 0)
        MPI_Sendrecv(&local_padded[KERNEL_RADIUS * width], width, MPI_UNSIGNED_CHAR, rank - 1, 0,
                     local_padded, width, MPI_UNSIGNED_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    if (rank < size - 1)
        MPI_Sendrecv(&local_padded[(KERNEL_RADIUS + local_rows - 1) * width], width, MPI_UNSIGNED_CHAR, rank + 1, 0,
                     &local_padded[(KERNEL_RADIUS + local_rows) * width], width, MPI_UNSIGNED_CHAR, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    double start = MPI_Wtime();

    for (int i = 0; i < local_rows; i++) {
        for (int j = 0; j < width; j++) {
            local_output[i * width + j] = smooth_pixel(local_padded, i + KERNEL_RADIUS, j, padded_rows, width);
        }
    }

    double end = MPI_Wtime();
    double local_time = end - start;
    double max_time;
    MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    unsigned char *final_result = NULL;
    if (rank == 0)
        final_result = malloc(width * height);

    MPI_Gatherv(local_output, local_rows * width, MPI_UNSIGNED_CHAR,
                final_result, sendcounts, displs, MPI_UNSIGNED_CHAR,
                0, MPI_COMM_WORLD);

    if (rank == 0) {
        FILE *out = fopen("smoothed_output.pgm", "w");
        fprintf(out, "P2\n%d %d\n255\n", width, height);
        for (int i = 0; i < width * height; i++) {
            fprintf(out, "%d ", final_result[i]);
            if ((i + 1) % width == 0) fprintf(out, "\n");
        }
        fclose(out);
        printf("Smoothing complete. Saved as smoothed_output.pgm\n");
        printf("Total execution time: %.6f seconds\n", max_time);
    }

    free(sendcounts);
    free(displs);
    free(local_padded);
    free(local_output);
    if (rank == 0) {
        free(full_image);
        free(final_result);
    }

    MPI_Finalize();
    return 0;
}
