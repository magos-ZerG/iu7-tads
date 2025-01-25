#ifndef GRAPH_H__
#define GRAPH_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define ALLOCATION_ERROR 1
#define INVALID_VERTEX_NUM 2
#define INVALID_DATA 3

typedef struct
{
    size_t vertex_num;
    int **adj_matrix;
} graph_t;

typedef struct
{
    size_t path_len;
    int *path;
} path_t;

void destroy_graph(graph_t *graph);
int import_graph(FILE *file, graph_t *dst);
void export_graph_to_dot(FILE *file, graph_t *graph);
void find_max_simple_path(graph_t *graph, path_t *max_path);
void print_path(path_t *path);

#endif //GRAPH_H__
