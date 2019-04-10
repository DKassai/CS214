// Necessary to enable reentrant hash table functions
#define _GNU_SOURCE

#include <search.h>
#include <stdlib.h>
#include <string.h>

#include "frequency.h"


/*
 * Uses a hash table internally to map tokens to frequencies.
 * While less efficient than an AVL because enough memory must be allocated
 * up front for all tokens, a hash table is easier to code and provides O(1)
 * lookup time. Additionally, hsearch_r is documented to be more efficient
 * when the table is given more space than necessary.
 */
struct frequency_map {
    struct hsearch_data table;
};


FrequencyMap *FM_create(size_t capacity) {
    FrequencyMap *fm = malloc(sizeof(FrequencyMap));
    if (fm == NULL) {
        return NULL;
    }
    memset(&fm->table, 0, sizeof(fm->table));
    hcreate_r(capacity, &fm->table);
    return fm;
}


void FM_destroy(FrequencyMap *fm) {
    hdestroy_r(&fm->table);
    free(fm);
}

// Cast void * to size_t, then to int to avoid warnings

void FM_add(FrequencyMap *fm, char *token) {
    ENTRY entry, *entry_p;
    entry.key = token;
    entry.data = 0;
    hsearch_r(entry, ENTER, &entry_p, &fm->table);
    if (entry_p != NULL) {
        entry_p->data = (void *)(((size_t)entry_p->data) + 1);
    }
}


int FM_frequency(FrequencyMap *fm, char *token) {
    ENTRY entry, *entry_p;
    entry.key = token;
    hsearch_r(entry, FIND, &entry_p, &fm->table);
    return entry_p ? (size_t)(entry_p->data) : 0;
}

