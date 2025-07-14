#ifndef FILTERS_H
#define FILTERS_H

#include "../utils/pgm_io.h"

void apply_smoothing(unsigned char input[HEIGHT][WIDTH], unsigned char output[HEIGHT][WIDTH]);
void apply_sharpening(unsigned char input[HEIGHT][WIDTH], unsigned char output[HEIGHT][WIDTH]);
void apply_edge_detection(unsigned char input[HEIGHT][WIDTH], unsigned char output[HEIGHT][WIDTH]);
void apply_emboss(unsigned char input[HEIGHT][WIDTH], unsigned char output[HEIGHT][WIDTH]);

void apply_filter(unsigned char input[HEIGHT][WIDTH], unsigned char output[HEIGHT][WIDTH], int kernel[3][3], int divisor);

#endif
