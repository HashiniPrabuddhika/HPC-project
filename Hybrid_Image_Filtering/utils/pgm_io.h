#ifndef PGM_IO_H
#define PGM_IO_H

void read_pgm(const char *filename, unsigned char image[512][512]);
void write_pgm(const char *filename, unsigned char image[512][512]);

#endif
