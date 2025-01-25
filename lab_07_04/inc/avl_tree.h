#ifndef AVL_TREE_H__
#define AVL_TREE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct avl_tree_node_t
{
    struct avl_tree_node_t *left;
    struct avl_tree_node_t *right;
    size_t height;
    char *data;
} avl_tree_node_t;

avl_tree_node_t *avl_create_node(char *data);

void avl_destroy_tree(avl_tree_node_t *tree);

size_t avl_height(avl_tree_node_t *tree);

avl_tree_node_t *avl_minimum(avl_tree_node_t *tree);
avl_tree_node_t *avl_maximum(avl_tree_node_t *tree);

avl_tree_node_t *avl_left_rotate(avl_tree_node_t *tree);
avl_tree_node_t *avl_right_rotate(avl_tree_node_t *tree);
int avl_get_balance(avl_tree_node_t *tree);

avl_tree_node_t *avl_insert(avl_tree_node_t *tree, avl_tree_node_t *node, int cmp(void *, void *));
avl_tree_node_t *avl_delete(avl_tree_node_t *tree, char *data, int cmp(void *, void *));
avl_tree_node_t *avl_search(avl_tree_node_t *tree, void *data, int cmp(void *, void *));

void avl_apply_pre(avl_tree_node_t *tree, void f(avl_tree_node_t*, void*), void *arg);
void avl_apply_in(avl_tree_node_t *tree, void f(avl_tree_node_t*, void*), void *arg);
void avl_apply_post(avl_tree_node_t *tree, void f(avl_tree_node_t*, void*), void *arg);

void avl_to_dot(avl_tree_node_t *tree, void *param);
void avl_export_to_dot(FILE *file, const char *tree_name, avl_tree_node_t *tree);

void avl_delete_by_first_letter(avl_tree_node_t **tree, char smb, int cmp(void *, void *));

#endif //AVL_TREE_H__
