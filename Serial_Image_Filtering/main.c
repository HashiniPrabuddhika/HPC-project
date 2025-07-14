#include <stdio.h>
#include <string.h>
#include "utils/pgm_io.h"
#include "include/filters.h"
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filter_name>\n", argv[0]);
        return 1;
    }

    unsigned char input[HEIGHT][WIDTH], output[HEIGHT][WIDTH];
    read_pgm("images/input/grayscale.pgm", input);
    clock_t start = clock();

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

    clock_t end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    char output_file[100];
    sprintf(output_file, "images/output/%s_output.pgm", argv[1]);
    write_pgm(output_file, output);
    printf("Output written to %s\n", output_file);
    printf("Execution Time: %.6f seconds\n", time_taken);
    return 0;
}

