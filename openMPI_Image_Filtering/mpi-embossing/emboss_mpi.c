#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 512
#define HEIGHT 512

int kernel[3][3] = {
    { -2, -1, 0 },
    { -1,  1, 1 },
    {  0,  1, 2 }
};

int clamp(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return value;
}

void read_pgm(const char *filename, unsigned char image[HEIGHT][WIDTH]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("Error opening file"); exit(1); }

    char ch;
    int width, height, max_val;
    fscanf(fp, "P2\n");
    while ((ch = fgetc(fp)) == '#') while (fgetc(fp) != '\n');
    ungetc(ch, fp);
    fscanf(fp, "%d %d\n%d\n", &width, &height, &max_val);

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            fscanf(fp, "%hhu", &image[i][j]);

    fclose(fp);
}

void write_pgm(const char *filename, unsigned char image[HEIGHT][WIDTH]) {
    FILE *fp = fopen(filename, "w");
    if (!fp) { perror("Error writing file"); exit(1); }

    fprintf(fp, "P2\n%d %d\n255\n", WIDTH, HEIGHT);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++)
            fprintf(fp, "%d ", image[i][j]);
        fprintf(fp, "\n");
    }
    fclose(fp);
}

void apply_filter(unsigned char input[HEIGHT][WIDTH],
                  unsigned char output[HEIGHT][WIDTH],
                  int start, int end) {
    for (int i = start; i < end; i++) {
        for (int j = 1; j < WIDTH - 1; j++) {
            int sum = 0;
            for (int ki = -1; ki <= 1; ki++) {
                for (int kj = -1; kj <= 1; kj++) {
                    sum += input[i + ki][j + kj] * kernel[ki + 1][kj + 1];
                }
            }
            output[i][j] = clamp(sum + 128); 
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    unsigned char image[HEIGHT][WIDTH];
    unsigned char result[HEIGHT][WIDTH] = {0};
    double start_time, end_time, max_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    if (rank == 0) read_pgm("grayscale.pgm", image);
    MPI_Bcast(image, HEIGHT * WIDTH, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    int rows_per_proc = HEIGHT / size;
    int start = rank * rows_per_proc;
    int end = (rank == size - 1) ? HEIGHT : start + rows_per_proc;
    if (start == 0) start = 1;
    if (end >= HEIGHT - 1) end = HEIGHT - 1;
    int local_rows = end - start;

    apply_filter(image, result, start, end);

    int *recvcounts = NULL;
    int *displs = NULL;
    if (rank == 0) {
        recvcounts = malloc(size * sizeof(int));
        displs = malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) {
            int s = i * rows_per_proc;
            int e = (i == size - 1) ? HEIGHT : s + rows_per_proc;
            if (s == 0) s = 1;
            if (e >= HEIGHT - 1) e = HEIGHT - 1;
            recvcounts[i] = (e - s) * WIDTH;
            displs[i] = s * WIDTH;
        }
    }

    MPI_Gatherv(&result[start][0], local_rows * WIDTH, MPI_UNSIGNED_CHAR,
                &result[0][0], recvcounts, displs, MPI_UNSIGNED_CHAR,
                0, MPI_COMM_WORLD);

   MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    double local_time = end_time - start_time;
    MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        write_pgm("emboss_output.pgm", result);
        printf("Total execution time: %.6f seconds\n", max_time);

        free(recvcounts);
        free(displs);
    }

    MPI_Finalize();
    return 0;
}

