#include "utils/pgm_io.h"

int main() {
    convert_png_to_pgm("images/input/input.png", "images/input/grayscale.pgm");
    return 0;
}

