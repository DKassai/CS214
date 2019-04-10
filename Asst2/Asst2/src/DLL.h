/*
 * DLL.h
 *
 * String-holding linked list implementation.
 */

#ifndef DLL_H
#define DLL_H


typedef struct DLL_node{
     struct DLL_node *prev, *next;
     char * data;
} DLL_node;

typedef struct   {
    DLL_node * head, * tail;
    int size;
} DLL;


/*
 * Insert an item at the end of the list.
 */
void DLLinsert (DLL * list, char * data);

/*
 * Delete an item from the list.
 */
void DLLdelete( DLL * list, DLL_node * data); 

/*
 * Create a new list.
 */
DLL * DLLCreate();

/*
 * Deallocate the list.
 * Does not deallocate its contents.
 */
void DLLdestroy(DLL* list);


#endif

