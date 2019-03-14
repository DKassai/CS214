#include <stdio.h>

// this is a counter for how much of the 4096 you have used 
//static size_t mem_used = 0;
//checks how many nodes are in the heap
//static int amount_of_nodes_used=0;



#define malloc( x ) mymalloc( x, __FILE__, __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )


#define Malloc_size 4096
static char myblock[Malloc_size];


void metadata(size_t requested, int loc, int left);

// this is the function declaration for the mymalloc 
void * mymalloc(size_t requested_size, char* string, size_t line);


// this is the function definition for myFree
void myfree ( char *  ToBeFreed, char* File, size_t line);












