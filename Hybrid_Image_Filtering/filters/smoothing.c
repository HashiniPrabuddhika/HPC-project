#include <omp.h>
#include "../include/filters.h"

void apply_smoothing(unsigned char **input, unsigned char **output, int height, int width) {
    int kernel[3][3] = {
        { 1, 2, 1 },
        { 2, 4, 2 },
        { 1, 2, 1 }
    };
    int kernel_sum = 16;

    #pragma omp parallel for num_threads(8) num_threads(8)
    for (int i = 0; i < height; i++) {
        for (int j = 1; j < width - 1; j++) {
            int sum = 0;
            for (int ki = -1; ki <= 1; ki++) {
                for (int kj = -1; kj <= 1; kj++) {
                    sum += input[i + ki + 1][j + kj] * kernel[ki + 1][kj + 1];
                }
            }
            int val = sum / kernel_sum;
            output[i][j] = (val < 0) ? 0 : (val > 255 ? 255 : val);
        }
    }
}
