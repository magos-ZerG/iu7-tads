#include "graph.h"

void destroy_graph(graph_t *graph)
{
    for (size_t i = 0; i < graph->vertex_num; i++)
        free((graph->adj_matrix)[i]);
    free(graph->adj_matrix);
    graph->adj_matrix = NULL;
    graph->vertex_num = 0;
}

int import_graph(FILE *file, graph_t *dst)
{
    int rc = fscanf(file, "%zu", &(dst->vertex_num));
    if (rc != 1 || dst->vertex_num <= 0)
        return INVALID_VERTEX_NUM;

    dst->adj_matrix = malloc(sizeof(int *) * (dst->vertex_num));
    if (dst->adj_matrix == NULL)
        return ALLOCATION_ERROR;
    
    for (size_t i = 0; i < dst->vertex_num; i++)
    {
        (dst->adj_matrix)[i] = malloc(sizeof(int) * (dst->vertex_num));
        if ((dst->adj_matrix)[i] == NULL)
        {
            for (size_t j = 0; j < i; j++)
                free((dst->adj_matrix)[j]);
            free(dst->adj_matrix);
            return ALLOCATION_ERROR;
        }
    }

    for (size_t i = 0; i < dst->vertex_num; i++)
        for (size_t j = 0; j < dst->vertex_num; j++)
        {
            rc = fscanf(file, "%d", &((dst->adj_matrix)[i][j]));
            if (rc != 1 || ((dst->adj_matrix)[i][j] != 0 && (dst->adj_matrix)[i][j] != 1))
            {
                destroy_graph(dst);
                return ALLOCATION_ERROR;
            }
        }

    return EXIT_SUCCESS;
}

void export_graph_to_dot(FILE *file, graph_t *graph)
{
    size_t flag;
    fprintf(file, "digraph MyGraph {\n");

    for (size_t i = 0; i < graph->vertex_num; i++)
    {
        flag = 0;
        for (size_t j = 0; j < graph->vertex_num; j++)
            if ((graph->adj_matrix)[i][j] != 0)
            {
                flag = 1;
                fprintf(file, "%zu -> %zu;\n", i + 1, j + 1);
            }
        if (!flag)
            fprintf(file, "%zu;\n", i + 1);
    }

    fprintf(file, "}\n");
}

static void adj_matrix_dfs(graph_t *graph, int vertex, path_t *max_path, path_t *curr_path, char *visited)
{
    visited[vertex] = 1;
    curr_path->path[(curr_path->path_len)++] = vertex;

    if (curr_path->path_len > max_path->path_len)
    {
        memcpy(max_path->path, curr_path->path, sizeof(int) * curr_path->path_len);
        max_path->path_len = curr_path->path_len;
    }

    for (size_t i = 0; i < graph->vertex_num && max_path->path_len < graph->vertex_num; i++)
        if ((graph->adj_matrix)[vertex][i] == 1 && !(visited)[i])
            adj_matrix_dfs(graph, i, max_path, curr_path, visited);

    visited[vertex] = 0;
    (curr_path->path_len)--;
}

void find_max_simple_path(graph_t *graph, path_t *max_path)
{
    path_t curr_path;
    char *visited;

    for (size_t i = 0; i < graph->vertex_num && max_path->path_len < graph->vertex_num; i++)
    {
        curr_path.path = calloc(graph->vertex_num * graph->vertex_num, sizeof(int));
        curr_path.path_len = 0;
        visited = calloc(graph->vertex_num * graph->vertex_num, sizeof(char));

        adj_matrix_dfs(graph, i, max_path, &curr_path, visited);

        free(curr_path.path);
        free(visited);
    }
}

void print_path(path_t *path)
{
    printf("%d", path->path[0] + 1);
    for (size_t i = 1; i < path->path_len; ++i)
    {
        printf("->");
        printf("%d", path->path[i] + 1);
    }
    printf("\n");
}
