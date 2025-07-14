#include "../include/filters.h"

void apply_edge_detection(unsigned char input[HEIGHT][WIDTH], unsigned char output[HEIGHT][WIDTH]) {
    int kernel[3][3] = {
        { -1, -1, -1 },
        { -1,  8, -1 },
        { -1, -1, -1 }
    };
    apply_filter(input, output, kernel, 1);
}

