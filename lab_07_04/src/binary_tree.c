#include "binary_tree.h"

tree_node_t* create_node(char *data)
{
    tree_node_t *node = malloc(sizeof(tree_node_t));
    if (node)
    {
        node->left = NULL;
        node->right = NULL;
        node->data = strdup(data);
    }
    return node;
}

void destroy_tree(tree_node_t *tree)
{
    if (tree == NULL)
        return;

    destroy_tree(tree->left);
    destroy_tree(tree->right);

    free(tree->data);
    free(tree);
    tree = NULL;
}

tree_node_t *minimum(tree_node_t *tree)
{
    if (tree->left == NULL)
        return tree;
    return minimum(tree->left);
}

tree_node_t *maximum(tree_node_t *tree)
{
    if (tree->right == NULL)
        return tree;
    return maximum(tree->right);
}

tree_node_t* insert(tree_node_t *tree, tree_node_t *node, int cmp(void *, void *))
{
    if (tree == NULL)
        return node;
    int rc = cmp(node->data, tree->data);
    if (rc == 0)
        assert(0);
    else if (rc < 0)
        tree->left = insert(tree->left, node, cmp);
    else
        tree->right = insert(tree->right, node, cmp);
    return tree;
}

tree_node_t *delete(tree_node_t *tree, char *data, int cmp(void *, void *))
{
    if (tree == NULL)
        return tree;

    tree_node_t *tmp;
    int rc = cmp(tree->data, data);

    if (rc > 0)
        tree->left = delete(tree->left, data, cmp);
    else if (rc < 0)
        tree->right = delete(tree->right, data, cmp);
    else if (tree->left != NULL && tree->right != NULL)
    {
        free(tree->data);
        tree->data = strdup(minimum(tree->right)->data);
        tree->right = delete(tree->right, tree->data, cmp);
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
    return tree;
}

tree_node_t *search(tree_node_t *tree, void *data, int cmp(void *, void *))
{
    if (tree == NULL)
        return NULL;
    int rc = cmp(data, tree->data);
    if (rc == 0)
        return tree;
    else if (rc < 0)
        return search(tree->left, data, cmp);
    else
        return search(tree->right, data, cmp);
}

void apply_pre(tree_node_t *tree, void f(tree_node_t*, void*), void *arg)
{
    if (tree == NULL)
        return;
    f(tree, arg);
    apply_pre(tree->left, f, arg);
    apply_pre(tree->right, f, arg);
}

void apply_in(tree_node_t *tree, void f(tree_node_t*, void*), void *arg)
{
    if (tree == NULL)
        return;
    apply_in(tree->left, f, arg);
    f(tree, arg);
    apply_in(tree->right, f, arg);
}

void apply_post(tree_node_t *tree, void f(tree_node_t*, void*), void *arg)
{
    if (tree == NULL)
        return;
    apply_post(tree->left, f, arg);
    apply_post(tree->right, f, arg);
    f(tree, arg);
}

void to_dot(tree_node_t *tree, void *param)
{
    FILE *file = param;
    if (tree->left)
        fprintf(file, "%s -> %s [color=blue];\n", tree->data, tree->left->data);
    if (tree->right)
        fprintf(file, "%s -> %s [color=red];\n", tree->data, tree->right->data);
}

void export_to_dot(FILE *file, const char *tree_name, tree_node_t *tree)
{
    fprintf(file, "digraph %s {\n", tree_name);
    apply_pre(tree, to_dot, file);
    fprintf(file, "}\n");
}

void delete_by_first_letter(tree_node_t **tree, char smb, int cmp(void *, void *))
{
    if (*tree == NULL)
        return;

    delete_by_first_letter(&((*tree)->left), smb, cmp);
    delete_by_first_letter(&((*tree)->right), smb, cmp);

    if (((*tree)->data)[0] == smb)
        *tree = delete(*tree, (*tree)->data, cmp);
}
