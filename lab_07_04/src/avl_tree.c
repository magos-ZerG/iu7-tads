#include "avl_tree.h"

avl_tree_node_t* avl_create_node(char *data)
{
    avl_tree_node_t *node = malloc(sizeof(avl_tree_node_t));
    if (node)
    {
        node->left = NULL;
        node->right = NULL;
        node->height = 1;
        node->data = strdup(data);
    }
    return node;
}

void avl_destroy_tree(avl_tree_node_t *tree)
{
    if (tree == NULL)
        return;

    avl_destroy_tree(tree->left);
    avl_destroy_tree(tree->right);

    free(tree->data);
    free(tree);
    tree = NULL;
}

size_t avl_height(avl_tree_node_t *tree)
{
    if (tree == NULL)
        return 0;
    return tree->height;
}

avl_tree_node_t *avl_minimum(avl_tree_node_t *tree)
{
    if (tree->left == NULL)
        return tree;
    return avl_minimum(tree->left);
}

avl_tree_node_t *avl_maximum(avl_tree_node_t *tree)
{
    if (tree->right == NULL)
        return tree;
    return avl_maximum(tree->right);
}

avl_tree_node_t *avl_left_rotate(avl_tree_node_t *tree)
{
    avl_tree_node_t *new_tree = tree->right;
    avl_tree_node_t *tmp = new_tree->left;

    new_tree->left = tree;
    tree->right = tmp;

    tree->height = MAX(avl_height(tree->left), avl_height(tree->right)) + 1;
    new_tree->height = MAX(avl_height(new_tree->left), avl_height(new_tree->right)) + 1;

    return new_tree;
}

avl_tree_node_t *avl_right_rotate(avl_tree_node_t *tree)
{
    avl_tree_node_t *new_tree = tree->left;
    avl_tree_node_t *tmp = new_tree->right;

    new_tree->right = tree;
    tree->left = tmp;

    tree->height = MAX(avl_height(tree->left), avl_height(tree->right)) + 1;
    new_tree->height = MAX(avl_height(new_tree->left), avl_height(new_tree->right)) + 1;

    return new_tree;
}

int avl_get_balance(avl_tree_node_t *tree)
{
    if (tree == NULL)
        return 0;
    return avl_height(tree->left) - avl_height(tree->right);
}

avl_tree_node_t* avl_insert(avl_tree_node_t *tree, avl_tree_node_t *node, int cmp(void *, void *))
{
    if (tree == NULL)
        return node;

    int rc = cmp(node->data, tree->data);

    if (rc < 0)
        tree->left = avl_insert(tree->left, node, cmp);
    else if (rc > 0)
        tree->right = avl_insert(tree->right, node, cmp);
    else
        return tree;

    tree->height = 1 + MAX(avl_height(tree->left), avl_height(tree->right));

    int balance = avl_get_balance(tree);

    if (balance > 1 && cmp(node->data, tree->left->data) < 0)
        return avl_right_rotate(tree);

    if (balance < -1 && cmp(node->data, tree->right->data) > 0)
        return avl_left_rotate(tree);

    if (balance > 1 && cmp(node->data, tree->left->data) > 0)
    {
        tree->left = avl_left_rotate(tree->left);
        return avl_right_rotate(tree);
    }

    if (balance < -1 && cmp(node->data, tree->right->data) < 0)
    {
        tree->right = avl_right_rotate(tree->right);
        return avl_left_rotate(tree);
    }

    return tree;
}

avl_tree_node_t *avl_delete(avl_tree_node_t *tree, char *data, int cmp(void *, void *))
{
    if (tree == NULL)
        return tree;

    avl_tree_node_t *tmp;
    int rc = cmp(tree->data, data);

    if (rc > 0)
        tree->left = avl_delete(tree->left, data, cmp);
    else if (rc < 0)
        tree->right = avl_delete(tree->right, data, cmp);
    else if (tree->left != NULL && tree->right != NULL)
    {
        free(tree->data);
        tree->data = strdup(avl_minimum(tree->right)->data);
        tree->right = avl_delete(tree->right, tree->data, cmp);
    }
    else
    {
        if (tree->left != NULL)
        {
            tmp = tree;
            tree = tree->left;
            free(tmp->data);
            free(tmp);
        }
        else if (tree->right != NULL)
        {
            tmp = tree;
            tree = tree->right;
            free(tmp->data);
            free(tmp);
        }
        else
        {
            tmp = tree;
            tree = NULL;
            free(tmp->data);
            free(tmp);
        }
    }

    if (tree == NULL)
      return tree;

    tree->height = 1 + MAX(avl_height(tree->left), avl_height(tree->right));

    int balance = avl_get_balance(tree);

    if (balance > 1 && avl_get_balance(tree->left) >= 0)
        return avl_right_rotate(tree);

    if (balance > 1 && avl_get_balance(tree->left) < 0)
    {
        tree->left = avl_left_rotate(tree->left);
        return avl_right_rotate(tree);
    }

    if (balance < -1 && avl_get_balance(tree->right) <= 0)
        return avl_left_rotate(tree);

    if (balance < -1 && avl_get_balance(tree->right) > 0)
    {
        tree->right = avl_right_rotate(tree->right);
        return avl_left_rotate(tree);
    }

    return tree;
}

avl_tree_node_t *avl_search(avl_tree_node_t *tree, void *data, int cmp(void *, void *))
{
    if (tree == NULL)
        return NULL;
    int rc = cmp(data, tree->data);
    if (rc == 0)
        return tree;
    else if (rc < 0)
        return avl_search(tree->left, data, cmp);
    else
        return avl_search(tree->right, data, cmp);
}

void avl_apply_pre(avl_tree_node_t *tree, void f(avl_tree_node_t*, void*), void *arg)
{
    if (tree == NULL)
        return;
    f(tree, arg);
    avl_apply_pre(tree->left, f, arg);
    avl_apply_pre(tree->right, f, arg);
}

void avl_apply_in(avl_tree_node_t *tree, void f(avl_tree_node_t*, void*), void *arg)
{
    if (tree == NULL)
        return;
    avl_apply_in(tree->left, f, arg);
    f(tree, arg);
    avl_apply_in(tree->right, f, arg);
}

void avl_apply_post(avl_tree_node_t *tree, void f(avl_tree_node_t*, void*), void *arg)
{
    if (tree == NULL)
        return;
    avl_apply_post(tree->left, f, arg);
    avl_apply_post(tree->right, f, arg);
    f(tree, arg);
}

void avl_to_dot(avl_tree_node_t *tree, void *param)
{
    FILE *file = param;
    if (tree->left)
        fprintf(file, "%s -> %s [color=blue];\n", tree->data, tree->left->data);
    if (tree->right)
        fprintf(file, "%s -> %s [color=red];\n", tree->data, tree->right->data);
}

void avl_export_to_dot(FILE *file, const char *tree_name, avl_tree_node_t *tree)
{
    fprintf(file, "digraph %s {\n", tree_name);
    avl_apply_pre(tree, avl_to_dot, file);
    fprintf(file, "}\n");
}

void avl_delete_by_first_letter(avl_tree_node_t **tree, char smb, int cmp(void *, void *))
{
    if (*tree == NULL)
        return;

    avl_delete_by_first_letter(&((*tree)->left), smb, cmp);
    avl_delete_by_first_letter(&((*tree)->right), smb, cmp);

    if (((*tree)->data)[0] == smb)
        *tree = avl_delete(*tree, (*tree)->data, cmp);
}
