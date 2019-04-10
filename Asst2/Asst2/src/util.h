/*
 * util.h
 *
 * Utility functions for printing/writing to files.
 */


#ifndef UTIL_H
#define UTIL_H


/*
 * Print out the current errno with a descriptive message.
 */
void print_errno();


/*
 * Write to stdout. (Appends a newline)
 */
void output(char *fmt, ...);


/*
 * Write to stderr. (Appends a newline)
 */
void error(char *fmt, ...);


/*
 * Write to a file.
 */
int write_string(int fd, char *s);

#endif

