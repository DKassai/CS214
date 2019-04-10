#include <stdlib.h>

#include "DLL.h"


DLL * DLLCreate (){
    DLL* list = malloc (sizeof(DLL));
    list->head=NULL;
    list->tail=NULL;
    list->size=0;
    return list;
}
// put at the front of the list so it will always be a o(1) insert 
void DLLinsert (DLL * list, char * data){
    DLL_node * tail = list->tail; 

    DLL_node * next_inserted= NULL;
   

    // this is when there is nothing in the linked list 
    if (tail==NULL){
        tail = (DLL_node*)malloc(sizeof(DLL_node)); 
        list->head=tail;
        list->tail=tail;
        tail->data= data;
        tail->next=NULL;
        tail->prev=NULL;
        list->size=1;
        return;
        
    }
    
    // when there is one or more things in the linked list
    next_inserted = (DLL_node*)malloc(sizeof(DLL_node)); 
    tail->next=next_inserted;
    next_inserted->prev=tail;
    list->tail=next_inserted;
    next_inserted->data=data;
    next_inserted->next= NULL;
    list->size+=1;    
}

//this frees the entire DLL
void DLLdestroy(DLL* list){
    DLL_node * ptr = list->head;
    while (ptr!=NULL){
        DLL_node * next = ptr->next;
        free(ptr);
        ptr=next;
    }
    free(list);
}
