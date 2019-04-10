// Necessary to enable reentrant hash table functions
#define _GNU_SOURCE

#include <ctype.h>
#include <search.h>
#include <stdlib.h>
#include <string.h>

#include "btree.h"
#include "DLL.h"
#include "frequency.h"
#include "huffman.h"
#include "minheap.h"
#include "tokenize.h"
#include "util.h"


/*
 * Huffman codebook.
 * Encapsulates a Huffman tree to encode/decode strings.
 * In order to prevent the O(N) time it would take to encode a string
 * by traversing the tree looking for it, each encoded bitstring is
 * preemptively cached in a hash table so that it can be retrieved in O(1)
 * time.
 * Decoding a string from a bitstring is done by traversing the tree in
 * O(logN) time.
 */
struct codebook {
    DLL *tokens;
    BTree *tree;
    struct hsearch_data encoder;
    BTree *node;
};


/*
 * Creates an uninitialized codebook.
 * codebook->tree is NULL because the method for populating it can be either
 * the actual Huffman algorithm or just reading a HuffmanCodebook file.
 */
Codebook *CB_create(size_t capacity) {
    Codebook *codebook = malloc(sizeof(Codebook));
    if (codebook == NULL) {
        return NULL;
    }
    memset(&codebook->encoder, 0, sizeof(codebook->encoder));
    if (!hcreate_r(capacity, &codebook->encoder)) {
        free(codebook);
        return NULL;
    }
    codebook->tokens = NULL;
    codebook->tree = NULL;
    codebook->node = NULL;
    return codebook;
}


void CB_destroy(Codebook *codebook) {
    DLL_node *node = codebook->tokens->head;
    while (node != NULL) {
        char *token = node->data;
        ENTRY entry, *entry_p;
        entry.key = token;
        hsearch_r(entry, FIND, &entry_p, &codebook->encoder);
        free(entry_p->data);
        free(token);
        node = node->next;
    }
    DLLdestroy(codebook->tokens);
    BT_destroy(codebook->tree, free);
    hdestroy_r(&codebook->encoder);
    free(codebook);
}


DLL *CB_tokens(Codebook *codebook) {
    return codebook->tokens;
}


/*
 * Cache an encoded value.
 */
void CB_cache(Codebook *codebook, char *token, char *encoded) {
    ENTRY entry, *entry_p;
    entry.key = token;
    entry.data = encoded;
    hsearch_r(entry, ENTER, &entry_p, &codebook->encoder);
}


/*
 * Data to be attached to each node in a Huffman tree.
 */
typedef struct {
    char *token;
    int frequency;
} HData;


/*
 * Shortcut to access HData from a tree (looks better than explicitly casting)
 */
HData *BT_get_hdata(BTree *tree) {
    return (HData *)(tree->data);
}


/*
 * Comparator used for nodes in a Huffman tree.
 */
int BT_comparator(void *left, void *right) {
    HData *ldata = BT_get_hdata((BTree *)left);
    HData *rdata = BT_get_hdata((BTree *)right);
    
    if (ldata->frequency > rdata->frequency) {
        return 1;
    } else if (ldata->frequency == rdata->frequency) {
        return 0;
    } else {
        return -1;
    }
}


/*
 * Huffman Algorithm
 * Create and return a Huffman tree for the tokens.
 * tokens is a non-empty list of distinct tokens.
 * fm is a frequency map for the given tokens.
 */
BTree *huffman_tree(DLL *tokens, FrequencyMap *fm) {
    // Build min heap
    // Create a BTree from each token and insert into the heap.
    MinHeap *heap = MH_create(tokens->size, BT_comparator);
    DLL_node *node;
    for (node = tokens->head; node != NULL; node = node->next) {
        HData *data = malloc(sizeof(HData));
        data->token = node->data;
        data->frequency = FM_frequency(fm, node->data);
        BTree *tree = BT_create(data);
        MH_insert(heap, tree);
    }

    // Reduce min heap to single tree
    // Pop 2, merge into one, insert back into heap
    // Repeat until only one tree left
    while (MH_size(heap) > 1) {
        BTree *tree, *left, *right;
        left = MH_pop(heap);
        right = MH_pop(heap);

        HData *data = malloc(sizeof(HData));
        data->token = NULL;
        int lfrequency = BT_get_hdata(left)->frequency;
        int rfrequency = BT_get_hdata(right)->frequency;
        data->frequency = lfrequency + rfrequency;

        tree = BT_create(data);
        tree->left = left;
        tree->right = right;
        MH_insert(heap, tree);
    }

    BTree *tree = MH_pop(heap);

    // Edge case if there is only one token, which would result in a
    // single-node tree with no branches so no way to encode
    // Create a dummy parent node so one branch will exist, encoding that one
    // token as "0"
    if (BT_is_leaf(tree)) {
        BTree *dummy = BT_create(malloc(sizeof(HData)));
        dummy->left = tree;
        tree = dummy;
    }

    MH_destroy(heap);
    return tree;
}


/*
 * Insert a token into the given Huffman tree, given its bitstring.
 */
void huffman_insert(BTree *tree, char *token, char *bitstring) {
    BTree *node = tree;
    int i;
    for (i = 0; i < strlen(bitstring); i++) {
        char c = bitstring[i];
        if (c == '0') {
            if (node->left == NULL) {
                HData *data = malloc(sizeof(HData));
                node->left = BT_create(data);
            }
            node = node->left;
        } else if (c == '1') {
            if (node->right == NULL) {
                HData *data = malloc(sizeof(HData));
                node->right = BT_create(data);
            }
            node = node->right;
        }
    }
    BT_get_hdata(node)->token = token;
}


/*
 * Recursively populate the codebook cache from the tree.
 * Prefix contains the bitstring leading up to the current node.
 */
void CB_cache_r(Codebook *codebook, BTree *tree, char *prefix) {
    if (tree->left != NULL) {
        char *encoded;
        asprintf(&encoded, "%s0", prefix);
        if (BT_is_leaf(tree->left)) {
            char *token = BT_get_hdata(tree->left)->token;
            CB_cache(codebook, token, encoded);
        } else {
            CB_cache_r(codebook, tree->left, encoded);
            free(encoded);
        }
    }

    if (tree->right != NULL) {
        char *encoded;
        asprintf(&encoded, "%s1", prefix);
        if (BT_is_leaf(tree->right)) {
            char *token = BT_get_hdata(tree->right)->token;
            CB_cache(codebook, token, encoded);
        } else {
            CB_cache_r(codebook, tree->right, encoded);
            free(encoded);
        }
    }
}


Codebook *CB_from_tokens(DLL *tokens) {
    FrequencyMap *fm = FM_create(tokens->size);
    DLL *distinct = DLLCreate();
    DLL_node *node = tokens->head;
    while (node != NULL) {
        char *token = node->data;
        if (FM_frequency(fm, token) == 0) {
            char *token_copy = malloc((strlen(token) + 1) * sizeof(char));
            strcpy(token_copy, token);
            DLLinsert(distinct, token_copy);
        }
        FM_add(fm, token);
        node = node->next;
    }

    Codebook *codebook = CB_create(distinct->size);
    codebook->tokens = distinct;
    if (distinct->size == 0) {
        // Empty tree
        codebook->tree = BT_create(malloc(sizeof(HData)));
    } else {
        codebook->tree = huffman_tree(distinct, fm);
    }
    CB_cache_r(codebook, codebook->tree, "");
    return codebook;
}


/*
 * Create a codebook from a list of tokens and a list of their encodings.
 */
Codebook *CB_from_bitstrings(DLL *tokens, DLL *bitstrings) {
    if (tokens->size == 0 || bitstrings->size == 0 || tokens->size != bitstrings->size) {
        return NULL;
    }
    Codebook *codebook = CB_create(tokens->size);
    BTree *tree = BT_create(malloc(sizeof(HData)));
    DLL_node *token_n = tokens->head;
    DLL_node *bitstring_n = bitstrings->head;
    while (token_n != NULL && bitstring_n != NULL) {
        huffman_insert(tree, token_n->data, bitstring_n->data);
        CB_cache(codebook, token_n->data, bitstring_n->data);
        token_n = token_n->next;
        bitstring_n = bitstring_n->next;
    }
    codebook->tokens = tokens;
    codebook->tree = tree;
    return codebook;
}


Codebook *CB_from_file(char *file) {
    char *content = Get_string(file);
    if (content == NULL) {
        error("Failed to read %s", file);
        print_errno();
        return NULL;
    }

    DLL *tokens = DLLCreate();
    DLL *bitstrings = DLLCreate();
    int i = 0, j, length;
    while (content[i] != '\n') {
        // Read next bitstring
        j = i;
        do {
            if (i >= strlen(content) || !(content[i] == '1' || content[i] == '0')) {
                error("Bitstring expected but none was found");
                DLL_node *node = tokens->head;
                while (node != NULL) {
                    free(node->data);
                    node = node->next;
                }
                node = bitstrings->head;
                while (node != NULL) {
                    free(node->data);
                    node = node->next;
                }
                DLLdestroy(tokens);
                DLLdestroy(bitstrings);
                free(content);
                return NULL;
            }
            i++;
        } while (content[i] != '\t');
        length = i - j;
        char *bitstring = malloc((length + 1) * sizeof(char));
        bitstring[length] = '\0';
        strncpy(bitstring, content + j, length);
        DLLinsert(bitstrings, bitstring);
        // Skip tab character
        i++;
        // Read next token
        j = i;
        // Using a do-while loop ensures that at least one character will be
        // read, removing the need to handle tabs/newlines etc. as special
        // cases
        do {
            if (i >= strlen(content)) {
                error("Token expected but none was found");
                DLL_node *node = tokens->head;
                while (node != NULL) {
                    free(node->data);
                    node = node->next;
                }
                node = bitstrings->head;
                while (node != NULL) {
                    free(node->data);
                    node = node->next;
                }
                DLLdestroy(tokens);
                DLLdestroy(bitstrings);
                free(content);
                return NULL;
            }
            i++;
        } while (content[i] != '\n');
        length = i - j;
        char *token = malloc((length + 1) * sizeof(char));
        token[length] = '\0';
        strncpy(token, content + j, length);
        DLLinsert(tokens, token);
        // Skip newline character
        i++;
    }

    return CB_from_bitstrings(tokens, bitstrings);
}    


char *CB_encode(Codebook *codebook, char *token) {
    ENTRY entry, *entry_p;
    entry.key = token;
    if (!hsearch_r(entry, FIND, &entry_p, &codebook->encoder)) {
        return NULL;
    }
    return entry_p->data;
}


char *CB_decode(Codebook *codebook, char bit) {
    if (codebook->node == NULL) {
        codebook->node = codebook->tree;
    }

    if (bit == '0') {
        codebook->node = codebook->node->left;
    } else if (bit == '1') {
        codebook->node = codebook->node->right;
    }

    if (BT_is_leaf(codebook->node)) {
        char *token = BT_get_hdata(codebook->node)->token;
        codebook->node = codebook->tree;
        return token;
    } else {
        return NULL;
    }
}

