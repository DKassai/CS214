#include <stdio.h>

#include "../src/minheap.h"


int int_comparator(void *a, void *b) {
    int x = (size_t)a, y = (size_t)b;
    return x < y ? -1 : x > y;
}


int main() {
    MinHeap *heap = MH_create(10, int_comparator);

    MH_insert(heap, (void *)5);
    MH_insert(heap, (void *)3);
    MH_insert(heap, (void *)1);
    MH_insert(heap, (void *)10);
    MH_insert(heap, (void *)3);
    MH_insert(heap, (void *)7);

    printf("Expected: 1 3 3 5 7 10\n");

    while (MH_size(heap) > 0) {
        printf("%d\n", (size_t)MH_pop(heap));
    }

    MH_destroy(heap);
    return 0;
}

