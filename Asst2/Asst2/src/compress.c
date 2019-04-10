#include <fcntl.h>
#include <string.h>

#include "compress.h"
#include "DLL.h"
#include "tokenize.h"
#include "util.h"


int build_codebook(DLL *tokens) {
    // Build codebook
    Codebook *codebook = CB_from_tokens(tokens);
    if (codebook == NULL) {
        error("Error while building codebook.");
        return 0;
    }

    // Open file
    int file = open("HuffmanCodebook", O_WRONLY | O_CREAT | O_TRUNC, 00600);
    if (file == -1) {
        error("Failed to create HuffmanCodebook file");
        CB_destroy(codebook);
        return 0;
    }

    // Write codebook to file
    // Iterate through the tokens, get the encoded bitstring from the
    // codebook, then write both to a line
    DLL_node *node = CB_tokens(codebook)->head;
    while (node != NULL) {
        char *token = node->data;
        char *encoded = CB_encode(codebook, token);
        char *line;
        asprintf(&line, "%s\t%s\n", encoded, token);

        if (!write_string(file, line)) {
            error("Failed to write to HuffmanCodebook");
            print_errno();
            free(line);
            close(file);
            CB_destroy(codebook);
            return 0;
        }

        free(line);
        node = node->next;
    }
    // Write trailing newline
    if (!write_string(file, "\n")) {
        error("Failed to write to HuffmanCodebook");
        print_errno();
        close(file);
        CB_destroy(codebook);
        return 0;
    }

    // Cleanup
    close(file);
    CB_destroy(codebook);
    return 1;
}


int compress(char *file, Codebook *codebook) {
    // Read and tokenize file
    char *content = Get_string(file);
    if (content == NULL) {
        error("Failed to read %s", file);
        print_errno();
        return 0;
    }
    DLL *tokens = get_tokens(content);
    free(content);

    // Open .hcz file
    char *outfile;
    asprintf(&outfile, "%s.hcz", file);
    int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 00600);
    free(outfile);
    if (fd == -1) {
        error("Failed to open output file");
        print_errno();
        DLLdestroy(tokens);
        return 0;
    }

    // Encode and write tokens
    DLL_node *node = tokens->head;
    while (node != NULL) {
        char *encoded = CB_encode(codebook, node->data);

        if (encoded == NULL) {
            error("%s cannot be encoded", node->data);
            DLLdestroy(tokens);
            close(fd);
            return 0;
        }

        if (!write_string(fd, encoded)) {
            error("Failed to write to output file");
            print_errno();
            DLLdestroy(tokens);
            close(fd);
            return 0;
        }

        free(node->data);
        node = node->next;
    }

    close(fd);
    DLLdestroy(tokens);
    return 1;
}


int decompress(char *file, Codebook *codebook) {
    char *content = Get_string(file);
    if (content == NULL) {
        error("Failed to read %s", file);
        print_errno();
        return 0;
    }

    // Output file: Input file without .hcz extension
    char *output_file = malloc(strlen(file) - 3);
    output_file[strlen(file) - 3] = '\0';
    strncpy(output_file, file, strlen(file) - 4);
    int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 00600);
    free(output_file);
    if (fd == -1) {
        error("Failed to write to output file");
        print_errno();
        free(content);
        return 0;
    }

    int i;
    for (i = 0; i < strlen(content); i++) {
        char *token = CB_decode(codebook, content[i]);
        if (token != NULL) {
            if (!write_string(fd, token)) {
                error("Failed to write to output file");
                print_errno();
                free(content);
                close(fd);
                return 0;
            }
        }
    }

    free(content);
    close(fd);
    return 1;
} 

