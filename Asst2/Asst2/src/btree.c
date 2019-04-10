#include <stdlib.h>

#include "btree.h"


BTree *BT_create(void *data) {
    BTree *tree = malloc(sizeof(BTree));
    if (tree == NULL) {
        return NULL;
    }
    tree->data = data;
    tree->left = NULL;
    tree->right = NULL;
    return tree;
}


void BT_destroy(BTree *tree, void (*destroy)(void *)) {
    if (tree->left != NULL) {
        BT_destroy(tree->left, destroy);
        tree->left = NULL;
    }
    if (tree->right != NULL) {
        BT_destroy(tree->right, destroy);
        tree->right = NULL;
    }
    destroy(tree->data);
    free(tree);
}


int BT_is_leaf(BTree *tree) {
    return tree->left == NULL && tree->right == NULL;
}

