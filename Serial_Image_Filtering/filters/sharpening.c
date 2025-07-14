#include "../include/filters.h"

void apply_sharpening(unsigned char input[HEIGHT][WIDTH], unsigned char output[HEIGHT][WIDTH]) {
    int kernel[3][3] = {
        {  0, -1,  0},
        { -1,  5, -1},
        {  0, -1,  0}
    };
    apply_filter(input, output, kernel, 1);
}
