/*
 * compress.h
 *
 * Core app functionality: build HuffmanCodebook files, compress files, and
 * decompress files.
 */

#ifndef COMPRESS_H
#define COMPRESS_H

#include "DLL.h"
#include "huffman.h"

/*
 * Write a HuffmanCodebook file from the given tokens.
 * Returns nonzero on success, 0 on failure.
 */
int build_codebook(DLL *tokens);


/*
 * Compress the input file into a .hcz file.
 * Returns nonzero on success, 0 on failure.
 */
int compress(char *file, Codebook *codebook);


/*
 * Decompress the input file.
 * Returns nonzero on success, 0 on failure.
 */
int decompress(char *file, Codebook *codebook);

#endif

