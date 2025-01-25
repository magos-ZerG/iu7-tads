#ifndef PROFILING_H__
#define PROFILING_H__

#include "binary_tree.h"
#include "avl_tree.h"
#include "oh_hash_table.h"
#include "ch_hash_table.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>

#define REPEATS_NUM 1000

int time_measurement(void);

#endif //PROFILING_H__
