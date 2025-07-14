#include <stdio.h>
#include <string.h>
#include "utils/pgm_io.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filter_name>\n", argv[0]);
        return 1;
    }

    char pgm_file[100], png_file[100];
    sprintf(pgm_file, "images/output/%s_output.pgm", argv[1]);
    sprintf(png_file, "images/output/%s_output.png", argv[1]);

    convert_pgm_to_png(pgm_file, png_file);

    return 0;
}

