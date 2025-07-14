#include "../include/filters.h"

void apply_emboss(unsigned char input[HEIGHT][WIDTH], unsigned char output[HEIGHT][WIDTH]) {
    int kernel[3][3] = {
        { -2, -1, 0 },
        { -1,  1, 1 },
        {  0,  1, 2 }
    };
    apply_filter(input, output, kernel, 1);
}
