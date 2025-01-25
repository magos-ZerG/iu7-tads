#ifndef BINARY_TREE_H__
#define BINARY_TREE_H__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef struct tree_node_t
{
    struct tree_node_t *left;
    struct tree_node_t *right;
    char *data;
} tree_node_t;

tree_node_t* create_node(char *data);
void destroy_tree(tree_node_t *tree);

tree_node_t *minimum(tree_node_t *tree);
tree_node_t *maximum(tree_node_t *tree);

tree_node_t* insert(tree_node_t *tree, tree_node_t *node, int cmp(void *, void *));
tree_node_t *delete(tree_node_t *tree, char *data, int cmp(void *, void *));
tree_node_t *search(tree_node_t *tree, void *data, int cmp(void *, void *));

void apply_pre(tree_node_t *tree, void f(tree_node_t*, void*), void *arg);
void apply_in(tree_node_t *tree, void f(tree_node_t*, void*), void *arg);
void apply_post(tree_node_t *tree, void f(tree_node_t*, void*), void *arg);

void to_dot(tree_node_t *tree, void *param);
void export_to_dot(FILE *file, const char *tree_name, tree_node_t *tree);

void delete_by_first_letter(tree_node_t **tree, char smb, int cmp(void *, void *));

#endif //BINARY_TREE_H__
