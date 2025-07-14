#include "../include/filters.h"

void apply_filter(unsigned char input[HEIGHT][WIDTH], unsigned char output[HEIGHT][WIDTH], int kernel[3][3], int divisor) {
    for (int i = 1; i < HEIGHT - 1; i++) {
        for (int j = 1; j < WIDTH - 1; j++) {
            int sum = 0;
            for (int ki = -1; ki <= 1; ki++) {
                for (int kj = -1; kj <= 1; kj++) {
                    sum += input[i + ki][j + kj] * kernel[ki + 1][kj + 1];
                }
            }
            sum /= divisor;
            if (sum < 0) sum = 0;
            if (sum > 255) sum = 255;
            output[i][j] = (unsigned char)sum;
        }
    }
}
