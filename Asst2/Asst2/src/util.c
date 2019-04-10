#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"


/*
 * Print to the given stream.
 */
void vfprintln(int file, char *fmt, va_list ap) {
    char *output;
    vasprintf(&output, fmt, ap);
    int written;
    int to_write = strlen(output);
    while ((written = write(file, output, to_write)) > 0) {
        to_write -= written;
    }
    write(file, "\n", 1);
    free(output);
}


void output(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintln(STDOUT_FILENO, fmt, ap);
    va_end(ap);
}


void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintln(STDERR_FILENO, fmt, ap);
    va_end(ap);
}


void print_errno() {
    error("Error no. %d: %s", errno, strerror(errno));
}


int write_string(int fd, char *s) {
    int to_write = strlen(s);
    int written = 0;
    int status;

    do {
        status = write(fd, s + written, to_write);
        written += status;
        to_write -= status;
    } while (to_write > 0 && status > 0);

    return status != -1;
}

