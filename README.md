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
- MPI implementation (OpenMPI recommended)
- libpng development libraries
