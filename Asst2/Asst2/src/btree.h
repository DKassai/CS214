#ifndef BTREE_H
#define BTREE_H


/*
 * Generic binary tree.
 */
typedef struct btree {
    void *data;
    struct btree *left, *right;
} BTree;


/*
 * Create a new tree.
 */
BTree *BT_create(void *data);

/*
 * Deallocate the tree and all of its subtrees.
 * The function pointed to by destroy will be called on each tree's data as it
 * is destroyed.
 */
void BT_destroy(BTree *tree, void (*destroy)(void *));

/*
 * Determine whether the tree is a leaf node.
 */
int BT_is_leaf(BTree *tree);

#endif

