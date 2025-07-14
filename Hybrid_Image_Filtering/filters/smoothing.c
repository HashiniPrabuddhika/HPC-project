#include <omp.h>
#include "../include/filters.h"

void apply_smoothing(unsigned char **input, unsigned char **output, int height, int width) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < height; i++) {  // i = 0 to height - 1
        for (int j = 1; j < width - 1; j++) {
            int sum = 0;
            for (int ki = -1; ki <= 1; ki++) {
                for (int kj = -1; kj <= 1; kj++) {
                    sum += input[i + ki + 1][j + kj]; // +1 because input includes halo
                }
            }
            int val = sum / 9;
            output[i][j] = (val < 0) ? 0 : (val > 255 ? 255 : val);
        }
    }
}
