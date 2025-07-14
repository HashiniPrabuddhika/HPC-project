#include <omp.h>
#include "../include/filters.h"

void apply_emboss(unsigned char **input, unsigned char **output, int height, int width) {
    int kernel[3][3] = {
        {-2, -1, 0},
        {-1,  1, 1},
        { 0,  1, 2}
    };

    #pragma omp parallel for collapse(2)
    for (int i = 1; i < height + 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            int sum = 0;
            for (int ki = -1; ki <= 1; ki++)
                for (int kj = -1; kj <= 1; kj++)
                    sum += kernel[ki + 1][kj + 1] * input[i + ki][j + kj];
            int val = sum + 128;
            output[i - 1][j] = (val < 0) ? 0 : (val > 255) ? 255 : val;
        }
    }
}
