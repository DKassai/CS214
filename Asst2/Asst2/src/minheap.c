#include <stdlib.h>

#include "minheap.h"
#include "util.h"


struct minheap {
    // Maximum number of elements the heap can contain
    size_t capacity;
    // Number of elements the heap currently contains
    size_t size;
    // Ordering function on the heap elements
    int (*comparator)(void *, void *);
    // Actual contents of the heap
    void **items;
};


MinHeap *MH_create(size_t capacity, int (*comparator)(void *, void*)) {
    MinHeap *heap = malloc(sizeof(MinHeap));
    if (heap == NULL) {
        return NULL;
    }
    heap->capacity = capacity;
    heap->size = 0;
    heap->comparator = comparator;
    heap->items = malloc(capacity * sizeof(void *));
    return heap;
}


void MH_destroy(MinHeap *heap) {
    free(heap->items);
    free(heap);
}


size_t MH_size(MinHeap *heap) {
    return heap->size;
}


int MH_parent(int i) {
    return (i - 1) / 2;
}


int MH_left_child(int i) {
    return 2 * i + 1;
}


int MH_right_child(int i) {
    return 2 * i + 2;
}


/*
 * Sift the last element in the heap upwards until the ordering property is
 * satisfied.
 */
void MH_sift_up(MinHeap *heap) {
    int i = MH_size(heap) - 1;
    int parent;

    while (i > 0) {
        parent = MH_parent(i);
        
        if (heap->comparator(heap->items[i], heap->items[parent]) <= 0) {
            void *temp = heap->items[parent];
            heap->items[parent] = heap->items[i];
            heap->items[i] = temp;
            i = parent;
        } else {
            break;
        }
    }
}


/*
 * Sift the first element in the heap downwards until the ordering property is
 * satisfied.
 */
void MH_sift_down(MinHeap *heap) {
    int i = 0;
    int child;

    while (MH_left_child(i) < MH_size(heap)) {
        int left = MH_left_child(i), right = MH_right_child(i);

        if (right < MH_size(heap)) {
            // Node has 2 children, determine min child
            int cmp = heap->comparator(heap->items[left], heap->items[right]);
            child = cmp <= 0 ? left : right;
        } else {
            child = left;
        }

        if (heap->comparator(heap->items[child], heap->items[i]) <= 0) {
            void *temp = heap->items[child];
            heap->items[child] = heap->items[i];
            heap->items[i] = temp;
            i = child;
        } else {
            break;
        }
    }
}


void MH_insert(MinHeap *heap, void *item) {
    if (MH_size(heap) == heap->capacity) {
        error("Attempted to insert into a full heap");
    }

    heap->items[heap->size++] = item;
    MH_sift_up(heap);
}


void *MH_pop(MinHeap *heap) {
    if (MH_size(heap) == 0) {
        error("Attempted to pop from an empty heap");
    }

    void *item = heap->items[0];
    heap->items[0] = heap->items[--heap->size];
    MH_sift_down(heap);
    return item;
}

