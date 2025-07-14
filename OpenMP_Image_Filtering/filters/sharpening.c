#include <omp.h>
#include "../include/filters.h"

int clamp(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return value;
}

void apply_sharpening(unsigned char input[512][512], unsigned char output[512][512]) {
    int kernel[3][3] = {
        {  0, -1,  0 },
        { -1,  5, -1 },
        {  0, -1,  0 }
    };
    #pragma omp parallel for collapse(2)
    for (int i = 1; i < 511; i++) {
        for (int j = 1; j < 511; j++) {
            int sum = 0;
            for (int ki = -1; ki <= 1; ki++)
                for (int kj = -1; kj <= 1; kj++)
                    sum += input[i + ki][j + kj] * kernel[ki + 1][kj + 1];
            output[i][j] = clamp(sum);
        }
    }
}
