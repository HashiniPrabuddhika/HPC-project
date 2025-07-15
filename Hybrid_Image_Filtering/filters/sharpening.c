#include <omp.h>
#include "../include/filters.h"

void apply_sharpening(unsigned char **input, unsigned char **output, int height, int width) {
    int kernel[3][3] = {
        {  0, -1,  0 },
        { -1,  5, -1 },
        {  0, -1,  0 }
    };

    #pragma omp parallel for num_threads(8) num_threads(8)
    for (int i = 1; i < height + 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            int sum = 0;
            for (int ki = -1; ki <= 1; ki++)
                for (int kj = -1; kj <= 1; kj++)
                    sum += kernel[ki + 1][kj + 1] * input[i + ki][j + kj];
            output[i - 1][j] = (sum < 0) ? 0 : (sum > 255) ? 255 : sum;
        }
    }
}
