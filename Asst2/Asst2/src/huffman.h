/*
 * huffman.h
 *
 * Defines a Codebook structure which can encode/decode tokens, functions for
 * creating one from tokens according to the Huffman Coding algorithm, and
 * functions for reading HuffmanCodebook files.
 */ 

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdlib.h>

#include "DLL.h"
#include "frequency.h"


/*
 * Huffman codebook structure.
 * Used to encode/decode strings to/from bitstrings.
 *
 * Example usage:
 *
 * DLL *tokens;
 * FrequencyMap *fm;
 * // Initialize tokens and fm
 * // Say tokens contains "foobar", and its encoded bitstring is "1101".
 * 
 * Codebook *codebook = CB_from_tokens(tokens, fm);
 * printf(CB_encode("foobar")); // 1101
 * 
 * CB_decode(codebook, '1');
 * CB_decode(codebook, '1');
 * CB_decode(codebook, '0');
 * printf(CB_decode(codebook, '1')); // foobar
 *
 * CB_destroy(codebook);
 */
typedef struct codebook Codebook;


/*
 * Create a new codebook.
 * Capacity is the number of tokens that the codebook can encode/decode.
 */
Codebook *CB_create(size_t capacity);


/*
 * Deallocate the codebook.
 */
void CB_destroy(Codebook *codebook);


/*
 * Return the list of distinct tokens used by the codebook.
 */
DLL *CB_tokens(Codebook *codebook);


/*
 * Create a codebook by performing the Huffman Coding algorithm on the given
 * tokens.
 * Tokens are copied into the codebook, so deallocating them will not affect
 * it.
 */
Codebook *CB_from_tokens(DLL *tokens);


/*
 * Create a codebook from a HuffmanCodebook file.
 */
Codebook *CB_from_file(char *file);


/*
 * Get the encoded bitstring for the given token.
 */
char *CB_encode(Codebook *codebook, char *token);


/*
 * Feed the codebook a bit.
 * This advances the internal state of the codebook. If the codebook reaches
 * a leaf node, it will return the decoded token, otherwise NULL.
 */
char *CB_decode(Codebook *codebook, char bit);

#endif

