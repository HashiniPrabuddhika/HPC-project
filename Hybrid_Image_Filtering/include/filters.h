#ifndef FILTERS_H
#define FILTERS_H

#define WIDTH 512
#define HEIGHT 512

void apply_smoothing(unsigned char **in, unsigned char **out, int height, int width);
void apply_sharpening(unsigned char **in, unsigned char **out, int height, int width);
void apply_edge_detection(unsigned char **in, unsigned char **out, int height, int width);
void apply_emboss(unsigned char **in, unsigned char **out, int height, int width);

#endif
