#include <omp.h>
#include "../include/filters.h"

void apply_smoothing(unsigned char input[512][512], unsigned char output[512][512]) {
    #pragma omp parallel for collapse(2)
    for (int i = 1; i < 511; i++) {
        for (int j = 1; j < 511; j++) {
            int sum = 0;
            for (int ki = -1; ki <= 1; ki++)
                for (int kj = -1; kj <= 1; kj++)
                    sum += input[i + ki][j + kj];
            output[i][j] = sum / 9;
        }
    }
}

