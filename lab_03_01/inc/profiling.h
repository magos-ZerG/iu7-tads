#ifndef PROFILING_H__
#define PROFILING_H__

#include "matrix.h"
#include "csr_matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>

#define REPEATS_NUM 100

int time_measurement();

int matrixes_init(matrix_t *matrix, csr_matrix_t *csr_matrix, double density);

#endif // PROFILING_H__
