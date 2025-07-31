# High Performance Computing Image Processing Project

This project implements parallel image filtering operations using multiple programming paradigms:

- Serial Implementation: Traditional sequential processing
- OpenMP version: Shared memory parallelism
- MPI version: Distributed memory parallelism
- Hybrid (MPI + OpenMP)version: Two-level parallelism combining both approaches

#### The project performs four main image filtering operations: 
- smoothing
- sharpening
- edge detection
- embossing
  on 512×512 images.
  
### Performance Results
The hybrid approach achieved the best performance with speedups of:

- Smoothing: 2.94× speedup
- Sharpening: 3.79× speedup
- Edge Detection: 2.93× speedup
- Embossing: 4.00× speedup

All parallel implementations maintain 100% accuracy (RMSE = 0.000) compared to serial versions.

## Prerequisites

- GCC compiler with OpenMP support
- MPI implementation 
- libpng development libraries

## Installation of Dependencies
Ubuntu/Debian:
```bash
bashsudo apt-get update
sudo apt-get install gcc libpng-dev openmpi-bin openmpi-common libopenmpi-dev
```

## Project Structure
``` bash
HPC-project/
├── compare
├── Hybrid_Image_Filtering
├── OpenMP_Image_Filtering
├── openMPI_Image_Filtering
├── Serial_Image_Filtering

```
## Usage Instructions
1. Serial Implementation
   ``` bash
   cd Serial_Image_Filtering
   ```

### Step 1: Convert PNG to PGM
``` bash
gcc -o convert_png utils/png_to_pgm.c convert.c utils/pgm_io.c -lpng -Iinclude
./convert_png
``` 
### Step 2: Compile and run serial filters
``` bash
gcc -o serial_filter main.c filters/*.c utils/pgm_io.c -Iinclude
```
### Apply filters
``` bash
./serial_filter smooth
./serial_filter sharpen
./serial_filter edge
./serial_filter emboss
```
### Step 3: Convert results back to PNG
``` bash
gcc -o save_png save_png.c utils/pgm_to_png.c utils/pgm_io.c -lpng
./save_png smooth
./save_png sharpen
./save_png edge
./save_png emboss
```
2. OpenMP Implementation
     ``` bash
   cd OpenMP_Image_Filtering
   ```
### Convert input image
 ``` bash
gcc convert/convert_png_to_pgm.c -lpng -o convert_png_to_pgm
./convert_png_to_pgm images/input/input.png images/input/grayscale.pgm
```

### Compile with OpenMP support
 ``` bash
gcc -fopenmp main.c filters/*.c utils/pgm_io.c -o run_filter -Iinclude -Iutils
```

### Run filters
 ``` bash
./run_filter smooth
./run_filter sharpen
./run_filter edge
./run_filter emboss
```

### Convert outputs to PNG
 ``` bash
gcc convert/convert_pgm_to_png.c -lpng -o convert_pgm_to_png
./convert_pgm_to_png images/output/smooth_output.pgm images/output/smooth_output.png
./convert_pgm_to_png images/output/sharpen_output.pgm images/output/sharpen_output.png
./convert_pgm_to_png images/output/edge_output.pgm images/output/edge_output.png
./convert_pgm_to_png images/output/emboss_output.pgm images/output/emboss_output.png
```

3. MPI Implementation
  
### MPI Smoothing
  ``` bash
    cd openMPI_Image_Filtering/mpi-smoothing
   ```

``` bash
bashgcc convert_png.c -o convert_png -lpng
./convert_png
mpicc openMPI.c -o openMPI
mpirun -np 4 ./openMPI
convert smoothed_output.pgm smoothed_output.png
```

### MPI Sharpening

  ``` bash
    cd openMPI_Image_Filtering/mpi-sharpen
   ```

``` bash
bashgcc -o convert convert_png_to_pgm.c -lpng
./convert
mpicc -o sharpen_mpi sharpen_mpi.c
mpirun -np 4 ./sharpen_mpi
gcc convert_pgm_to_png.c -o convert_pgm_to_png -lpng
./convert_pgm_to_png
```
### MPI Embossing

  ``` bash
    cd openMPI_Image_Filtering/mpi-embossing
   ```

``` bash
bashgcc -o convert convert_png_to_pgm.c -lpng
./convert
mpicc emboss_mpi.c -o emboss_mpi
mpirun -np 4 ./emboss_mpi
gcc convert_pgm_to_png.c -o convert_pgm_to_png -lpng
./convert_pgm_to_png
```
### MPI Edge Detection

  ``` bash
    cd openMPI_Image_Filtering/mpi-edgedetection
   ```

``` bash
bashgcc -o convert convert_png_to_pgm.c -lpng
./convert
mpicc edge_mpi.c -o edge_mpi -lm
mpirun -np 4 ./edge_mpi
gcc convert_pgm_to_png.c -o convert_pgm_to_png -lpng
./convert_pgm_to_png
```


5. Hybrid Implementation (MPI + OpenMP)
 ``` bash
   cd Hybrid_Image_Filtering
   ```
### Convert input
``` bash
gcc convert/convert_png_to_pgm.c -lpng -o convert_png_to_pgm
./convert_png_to_pgm images/input/input.png images/input/grayscale.pgm
```

# Compile hybrid version
``` bash
mpicc -fopenmp -o hybrid_filter main.c filters/*.c utils/pgm_io.c -Iinclude -Iutils
```

# Set OpenMP threads and run with MPI
``` bash
export OMP_NUM_THREADS=4
mpirun -np 4 ./hybrid_filter smooth
mpirun -np 4 ./hybrid_filter sharpen
mpirun -np 4 ./hybrid_filter edge
mpirun -np 4 ./hybrid_filter emboss
```

# Convert outputs
``` bash
gcc convert/convert_pgm_to_png.c -lpng -o convert_pgm_to_png
./convert_pgm_to_png images/output/smooth_output.pgm images/output/smooth_output.png
./convert_pgm_to_png images/output/sharpen_output.pgm images/output/sharpen_output.png
./convert_pgm_to_png images/output/edge_output.pgm images/output/edge_output.png
./convert_pgm_to_png images/output/emboss_output.pgm images/output/emboss_output.png
```

5. Performance Comparison
   
## OpenMP Comparison
``` bash
    cd compare/openMP
   ```

``` bash
gcc compare_openMP_Smoothing.c -o compare_openMP_Smoothing -lm
./compare_openMP_Smoothing
```

## OpenMPI Comparison
``` bash
    cd compare/openMPI
   ```

``` bash
gcc compare_openMPI_Smoothing.c -o compare_openMPI_Smoothing -lm
./compare_openMPI_Smoothing
```
## Hybrid Comparison

``` bash
    cd compare/Hybrid
   ```
``` bash
gcc compare_hybrid_Smoothing.c -o compare_hybrid_Smoothing -lm
./compare_hybrid_Smoothing
```

## Configuration Options
### Thread Scaling

- OpenMP: Modify OMP_NUM_THREADS environment variable
- MPI: Adjust -np parameter in mpirun commands
- Hybrid: Configure both MPI processes and OpenMP threads


### Performance Analysis
| Filter         | Serial (s) | OpenMP (s) | MPI (s) | Hybrid (s) |
|----------------|------------|------------|---------|-------------|
| Smoothing      | 0.008325   | 0.003640   | 0.005461| 0.002831    |
| Sharpening     | 0.007928   | 0.005312   | 0.003879| 0.002092    |
| Edge Detection | 0.010623   | 0.004645   | 0.016929| 0.003626    |
| Embossing      | 0.008597   | 0.004633   | 0.016875| 0.002149    |




### Perfect accuracy maintained across all parallel implementations (RMSE = 0.000)

| Filter Method   | OpenMP RMSE | OpenMP Accuracy (%) | MPI RMSE | MPI Accuracy (%) | Hybrid RMSE | Hybrid Accuracy (%) |
|-----------------|-------------|----------------------|----------|-------------------|-------------|----------------------|
| Smoothing       | 0.000000    | 100                  | 0.000000 | 100               | 0.000000    | 100                  |
| Sharpening      | 0.000000    | 100                  | 0.000000 | 100               | 0.000000    | 100                  |
| Edge Detection  | 0.000000    | 100                  | 0.000000 | 100               | 0.000000    | 100                  |
| Embossing       | 0.000000    | 100                  | 0.000000 | 100               | 0.000000    | 100                  |


- All parallel implementations produce identical results to the serial version (RMSE = 0.000), confirming that parallelization maintains computational accuracy while improving performance.
