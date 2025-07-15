#include <stdio.h>
#include <string.h>
#include "include/filters.h"
#include "utils/pgm_io.h"
#include <omp.h>

#define WIDTH 512
#define HEIGHT 512
#define NUM_RUNS 4

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filter_name>\n", argv[0]);
        printf("Filters: smooth, sharpen, edge, emboss\n");
        return 1;
    }

    unsigned char input[HEIGHT][WIDTH];
    unsigned char output[HEIGHT][WIDTH] = {0};

    read_pgm("images/input/grayscale.pgm", input);

    double total_time = 0.0;

    for (int run = 0; run < NUM_RUNS; run++) {
        double start_time = omp_get_wtime();

        if (strcmp(argv[1], "smooth") == 0)
            apply_smoothing(input, output);
        else if (strcmp(argv[1], "sharpen") == 0)
            apply_sharpening(input, output);
        else if (strcmp(argv[1], "edge") == 0)
            apply_edge_detection(input, output);
        else if (strcmp(argv[1], "emboss") == 0)
            apply_emboss(input, output);
        else {
            printf("Unknown filter: %s\n", argv[1]);
            return 1;
        }

        double end_time = omp_get_wtime();
        double time_taken = end_time - start_time;
        total_time += time_taken;

        printf("Run %d: %s filter applied in %.6f seconds\n", run + 1, argv[1], time_taken);
    }

    char output_file[100];
    sprintf(output_file, "images/output/%s_output.pgm", argv[1]);
    write_pgm(output_file, output);

    printf(" %s filter applied and saved to %s\n", argv[1], output_file);
    printf("Average Execution Time (OpenMP): %.6f seconds\n", total_time / NUM_RUNS);

    return 0;
}
