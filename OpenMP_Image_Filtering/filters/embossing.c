#include <omp.h>
#include "../include/filters.h"
#include "../include/clamp.h"

void apply_emboss(unsigned char input[512][512], unsigned char output[512][512]) {
    int kernel[3][3] = {
        { -2, -1,  0 },
        { -1,  1,  1 },
        {  0,  1,  2 }
    };
    #pragma omp parallel for num_threads(8) num_threads(8)
    for (int i = 1; i < 511; i++) {
        for (int j = 1; j < 511; j++) {
            int sum = 0;
            for (int ki = -1; ki <= 1; ki++)
                for (int kj = -1; kj <= 1; kj++)
                    sum += input[i + ki][j + kj] * kernel[ki + 1][kj + 1];
            output[i][j] = clamp(sum + 128);
        }
    }
}
