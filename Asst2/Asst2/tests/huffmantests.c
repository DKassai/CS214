#include <stdio.h>

#include "../src/DLL.h"
#include "../src/frequency.h"
#include "../src/huffman.h"


void test_A() {
    printf("Test A\n");

    DLL *tokens = DLLCreate();
    FrequencyMap *fm = FM_create(10);
    int i;

    DLLinsert(tokens, "a");
    for (i = 0; i < 5; i++) {
        FM_add(fm, "a");
    }
    DLLinsert(tokens, "dog");
    for (i = 0; i < 9; i++) {
        FM_add(fm, "dog");
    }
    DLLinsert(tokens, "cat");
    for (i = 0; i < 12; i++) {
        FM_add(fm, "cat");
    }
    DLLinsert(tokens, "button");
    for (i = 0; i < 13; i++) {
        FM_add(fm, "button");
    }
    DLLinsert(tokens, "ball");
    for (i = 0; i < 16; i++) {
        FM_add(fm, "ball");
    }
    DLLinsert(tokens, "and");
    for (i = 0; i < 45; i++) {
        FM_add(fm, "and");
    }

    Codebook *codebook = CB_from_tokens(tokens, fm);

    printf("%s: %s\n", "a", CB_encode(codebook, "a"));
    printf("%s: %s\n", "dog", CB_encode(codebook, "dog"));
    printf("%s: %s\n", "cat", CB_encode(codebook, "cat"));
    printf("%s: %s\n", "button", CB_encode(codebook, "button"));
    printf("%s: %s\n", "ball", CB_encode(codebook, "ball"));
    printf("%s: %s\n", "and", CB_encode(codebook, "and"));

    CB_decode(codebook, '1');
    CB_decode(codebook, '1');
    CB_decode(codebook, '0');
    printf("%s: %s\n", "1101", CB_decode(codebook, '1'));
    printf("%s: %s\n", "0", CB_decode(codebook, '0'));

    DLLdestroy(tokens);
    FM_destroy(fm);
    CB_destroy(codebook);
}


void test_B() {
    printf("Test B\n");

    DLL *tokens = DLLCreate();
    DLL *bitstrings = DLLCreate();

    DLLinsert(tokens, "a");
    DLLinsert(tokens, "dog");
    DLLinsert(tokens, "cat");
    DLLinsert(tokens, "button");
    DLLinsert(tokens, "ball");
    DLLinsert(tokens, "and");

    DLLinsert(bitstrings, "1100");
    DLLinsert(bitstrings, "1101");
    DLLinsert(bitstrings, "100");
    DLLinsert(bitstrings, "101");
    DLLinsert(bitstrings, "111");
    DLLinsert(bitstrings, "0");

    Codebook *codebook = CB_from_bitstrings(tokens, bitstrings);

    printf("%s: %s\n", "a", CB_encode(codebook, "a"));
    printf("%s: %s\n", "dog", CB_encode(codebook, "dog"));
    printf("%s: %s\n", "cat", CB_encode(codebook, "cat"));
    printf("%s: %s\n", "button", CB_encode(codebook, "button"));
    printf("%s: %s\n", "ball", CB_encode(codebook, "ball"));
    printf("%s: %s\n", "and", CB_encode(codebook, "and"));

    CB_decode(codebook, '1');
    CB_decode(codebook, '1');
    CB_decode(codebook, '0');
    printf("%s: %s\n", "1101", CB_decode(codebook, '1'));
    printf("%s: %s\n", "0", CB_decode(codebook, '0'));

    DLLdestroy(tokens);
    DLLdestroy(bitstrings);
    CB_destroy(codebook);
}


int main() {
    test_A();
    test_B();
    return 0;
}

