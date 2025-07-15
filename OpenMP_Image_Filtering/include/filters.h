#ifndef FILTERS_H
#define FILTERS_H

#define WIDTH 512
#define HEIGHT 512

void apply_smoothing(unsigned char input[HEIGHT][WIDTH], unsigned char output[HEIGHT][WIDTH]);
void apply_sharpening(unsigned char input[HEIGHT][WIDTH], unsigned char output[HEIGHT][WIDTH]);
void apply_edge_detection(unsigned char input[HEIGHT][WIDTH], unsigned char output[HEIGHT][WIDTH]);
void apply_emboss(unsigned char input[HEIGHT][WIDTH], unsigned char output[HEIGHT][WIDTH]);

#endif
