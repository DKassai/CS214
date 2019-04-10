#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "../src/compress.h"
#include "../src/DLL.h"
#include "../src/huffman.h"


void test_build_codebook() {
    DLL *tokens = DLLCreate();
    int i;
    for (i = 0; i < 5; i++) {
        DLLinsert(tokens, "a");
    }
    for (i = 0; i < 9; i++) {
        DLLinsert(tokens, "dog");
    }
    for (i = 0; i < 12; i++) {
        DLLinsert(tokens, "cat");
    }
    for (i = 0; i < 13; i++) {
        DLLinsert(tokens, "button");
    }
    for (i = 0; i < 16; i++) {
        DLLinsert(tokens, "ball");
    }
    for (i = 0; i < 45; i++) {
        DLLinsert(tokens, "and");
    }
    for (i = 0; i < 80; i++) {
        DLLinsert(tokens, " ");
    }

    int status = build_codebook(tokens);
    if (!status) {
        printf("Error %d: %s\n", errno, strerror(errno));
    }
    DLLdestroy(tokens);
}


void test_compress() {
    Codebook *codebook = CB_from_file("HuffmanCodebook");
    if (!compress("Tests/Data/words.1.txt", codebook)) {
        printf("Error %d: %s\n", errno, strerror(errno));
    }
    CB_destroy(codebook);
}


void test_decompress() {
    Codebook *codebook = CB_from_file("HuffmanCodebook");
    if (!decompress("Tests/Data/bits.1.txt.hcz", codebook)) {
        printf("Error %d: %s\n", errno, strerror(errno));
    }
    CB_destroy(codebook);
}


int main() {
    test_build_codebook();
    test_compress();
    test_decompress();
    return 0;
}

