#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

typedef struct node{
    char * data;
    struct node*next;
} node;


int compare (node * head, node * ptr){
    // can only check as long as the shortest word so need to find the shortest word 
    char * shortestword;
    if (strlen(head->data)<strlen(ptr->data)){
        shortestword= head->data;

        
    }else {
        shortestword= ptr->data;

    }

// sees what the uppercase letters are and if they are there or not 
    int i;
    for (i=0;i<strlen(shortestword);i++){
        if (isupper(head->data[i]) != isupper(ptr->data[i])){
            // check if this is two different ways of returning
            if (isupper(head->data[i])){
                return 1;
            }
            else {
                return -1;
            }
        }
      
        if (head->data[i]<ptr->data[i]){
            return -1;
        }else if (head->data[i]==ptr->data[i]){
            continue;
        }else {
            return 1;
        }
    }

    if (head->data==shortestword){
        return 1;
    }else {
        return -1;
    }
    
    return 0;

}


node *  insert(node * head, node * current){
   
   

    node * first;

    node * first_node_of_LL = head;

// this is when there is only one item in the linked list and needs to input another  
         if (head->next==NULL|| compare(current,head)==1){
            
            int inserted = compare(current,head);

            if (inserted==1 || inserted==0 ){

            current->next=head;
            first_node_of_LL=current;

             }
            else {
                head->next=current;

              
            }
    // when there are more than 2 items in the linked list and need to compare and put in the proper spot 
        }else{
            first = first_node_of_LL;
            head=head->next;

            // this is where it runs through the code and compares to the nodes to see where to put it in
            while (first->next!= NULL){
                int inserted = compare(current,head);
                if (inserted==1 || inserted==0){
                    current->next=head;
                    first->next=current;
                    return first_node_of_LL;
                }

                head=head->next;
              
                first=first->next;
            }
           first->next=current;
        }


    

    return first_node_of_LL;
}

void free_nodes(node * ptr){
// this is to free the data from the linked list 
    if (ptr->next!=NULL){
        free_nodes(ptr->next);
        ptr->next=NULL;
    }
    free(ptr->data);
    free(ptr);

}
  

int main (int argc, char** argv){

// gets the argument from the input
    char * input_string= argv[1];

    if (input_string==NULL){
        return 0;
    }
// checks if the argument is only non alphabetical letters 
    int k;
    int counter;
    for (k=0; k<strlen(input_string);k++){
        if (!isalpha(input_string[k])){
        counter++;
        }
        if (counter==strlen(input_string)){
            return 0;
        }
    }
    // checks if the length of the argument is 0
    if (strlen(input_string)==0){
        return 0;
    }

    node* ptr = NULL;

    node* head= NULL;

// part of the main where you break down the argument into individual words to input into the linked list
    int i=0; 
    int j=0;
    for (i=0;i<strlen(input_string);i++){
        if(isalpha(input_string[i])){
            j=i;
        while((isalpha(input_string[i]))){
            i++;
        }
        // dynamically malloc space for the amount of linked lists 
        char * word = malloc(sizeof(char)*(i-j+1));
        strncpy(word,input_string+j,i-j);
        ptr = ( node*)malloc(sizeof( node)); 
        ptr->data=word;
        ptr->next=NULL;

        // if the linked list is null put the first item in the head
        if (head==NULL){
            head=ptr;

        }
        else {
            // where you insert and compare to put into the linked list. 
           head = insert(head,ptr);    
        }
    }

    }  
    node * start_of_list=head;
   

//finally print out the sorted linked list 
    while (start_of_list!=NULL){
        printf("%s\n", start_of_list->data);
        start_of_list=start_of_list->next;
    }

    free_nodes(head);
    
}



