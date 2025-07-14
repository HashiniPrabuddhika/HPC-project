#ifndef FILTERS_H
#define FILTERS_H

void apply_smoothing(unsigned char input[512][512], unsigned char output[512][512]);
void apply_sharpening(unsigned char input[512][512], unsigned char output[512][512]);
void apply_edge_detection(unsigned char input[512][512], unsigned char output[512][512]);
void apply_emboss(unsigned char input[512][512], unsigned char output[512][512]);

#endif
