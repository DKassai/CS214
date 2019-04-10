/*
 * fileCompressor driver
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/compress.h"
#include "src/huffman.h"
#include "src/util.h"
#include "src/DLL.h"
#include "src/tokenize.h"

/*
 * Return the index of the flag if it exists, -1 otherwise
 */
int find_flag(int argc, char *argv[], char *flag) {
    int i;
    for (i = 0; i < argc; i++) {
        if (strcmp(flag, argv[i]) == 0) {
            return i;
        }
    }
    return -1;
}


/*
 * Set the given boolean variable to 1 if the flag has been set, 0 otherwise
 */
void set_flag(int argc, char *argv[], char *flag, int *variable) {
    int i = find_flag(argc, argv, flag);
    *variable = i != -1;
}


int main(int argc, char *argv[]) {
    // Global input flags
    int BUILD_CODEBOOK = 0, COMPRESS = 0, DECOMPRESS = 0, RECURSIVE = 0;
    char *INPUT_FILE = NULL;
    char *CODEBOOK_FILE = NULL;
    // String description of action to be performed
    char *ACTION = NULL;

    set_flag(argc, argv, "-b", &BUILD_CODEBOOK);
    set_flag(argc, argv, "-c", &COMPRESS);
    set_flag(argc, argv, "-d", &DECOMPRESS);
    set_flag(argc, argv, "-R", &RECURSIVE);

    // Validate flag combinations

    if (!(BUILD_CODEBOOK || COMPRESS || DECOMPRESS)) {
        error("No action selected");
        error("-b: build codebook");
        error("-c: compress");
        error("-d: decompress");
        return 0;
    }

    if ((BUILD_CODEBOOK && COMPRESS) || (BUILD_CODEBOOK && DECOMPRESS) || (COMPRESS && DECOMPRESS)) {
        error("Only one action is allowed at a time.");
        return 0;
    }

    if (argc != (BUILD_CODEBOOK ? 3 : 4) + RECURSIVE) {
        error("Wrong number of arguments");
        return 0;
    }

    if (COMPRESS || DECOMPRESS) {
        INPUT_FILE = argv[argc - 2];
        CODEBOOK_FILE = argv[argc - 1];
    } else {
        INPUT_FILE = argv[argc - 1];
    }

    // Output header

    if (COMPRESS) {
        ACTION = "compress";
    } else if (DECOMPRESS) {
        ACTION = "decompress";
    } else if (BUILD_CODEBOOK) {
        ACTION = "build codebook";
    }

    output("--------------------");
    output("Action: %s", ACTION);
    output("Input file: %s", INPUT_FILE);
    output("Codebook file: %s", BUILD_CODEBOOK ? "none" : CODEBOOK_FILE);
    output("Recursive: %s", RECURSIVE ? "yes" : "no");
    output("--------------------");

    // Build list of all input files to operate on.

    DLL *files;
    if (RECURSIVE) {
        files = it_dir(INPUT_FILE);
        if (files == NULL) {
            error("Failed to read directory %s", INPUT_FILE);
            print_errno();
            return 0;
        }
    } else {
        files = DLLCreate();
        DLLinsert(files, INPUT_FILE);
    }

    // Perform operation.

    if (COMPRESS) {
        Codebook *codebook = CB_from_file(CODEBOOK_FILE);
        if (codebook == NULL) {
            error("Error reading codebook file.");
            print_errno();
            return 0;
        }
        DLL_node *node = files->head;
        while (node != NULL) {
            char *file = node->data;
            // Don't abort entire operation if one file fails to compress;
            // continue with the rest
            int success = compress(file, codebook);
            if (success) {
                output("Compressed %s", file);
            } else {
                error("Failed to compress %s", file);
            }
            node = node->next;
        }
        DLLdestroy(files);
        CB_destroy(codebook);
        return 0;
    }

    if (DECOMPRESS) {
        Codebook *codebook = CB_from_file(CODEBOOK_FILE);
        if (codebook == NULL) {
            error("Error reading codebook file.");
            print_errno();
            return 0;
        }
        DLL_node *node = files->head;
        while (node != NULL) {
            char *file = node->data;
            // Don't abort entire operation if one file fails to decompress;
            // continue with the rest
            int success = decompress(file, codebook);
            if (success) {
                output("Decompressed %s", file);
            } else {
                error("Failed to decompress %s", file);
            }
            node = node->next;
        }
        DLLdestroy(files);
        CB_destroy(codebook);
        return 0;
    }

    if (BUILD_CODEBOOK) {
        // Build list of all tokens across every input file
        DLL *tokens = DLLCreate();
        // Iterate through files
        DLL_node *file_node = files->head;
        while (file_node != NULL) {
            char *file = file_node->data;
            char *content = Get_string(file);
            if (content == NULL) {
                // Failing to read a single input file will invalidate the
                // entire Huffman tree, so abort the operation.
                error("Failed to read %s", file);
                print_errno();
                DLLdestroy(tokens);
                DLLdestroy(files);
                return 0;
            }
            DLL *file_tokens = get_tokens(content);
            free(content);
            // Iterate through tokens in file and append to global token list
            DLL_node *token_node = file_tokens->head;
            while (token_node != NULL) {
                char *token = token_node->data;
                DLLinsert(tokens, token);
                token_node = token_node->next;
            }
            output("Read %s", file);
            DLLdestroy(file_tokens);
            file_node = file_node->next;
        }
        int success = build_codebook(tokens);
        if (success) {
            output("Built HuffmanCodebook file.");
        } else {
            error("Failed to build HuffmanCodebook file.");
            print_errno();
        }
        DLL_node *node = tokens->head;
        while (node != NULL) {
            free(node->data);
            node = node->next;
        }
        DLLdestroy(tokens);
        DLLdestroy(files);
        return 0;
    }

    return 0;
}

