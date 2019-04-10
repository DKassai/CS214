#include <stdlib.h>
#include "../DLL.h"



int main(){
    DLL_node * ptr;
    DLL * list= DLLCreate();
  //  kill_DLL(list);
  printf("-------------------------ADD INTO LIST ----------------------\n");
    DLLinsert(list,"hello world");
    DLLinsert(list,"next line");
    DLLinsert(list, "the third try");
    

    for (ptr=list->head; ptr!=NULL; ptr=ptr->next){
        printf("%s\n", ptr->data);
    }

printf("----------------------DELETE PART----------------------\n");
   // DLLdelete(list,list->head);
    
       
    for (ptr=list->head; ptr!=NULL; ptr=ptr->next){
        printf("%s\n", ptr->data);
    }


printf("--------------------------KILL THE LIST----------------------\n");
   
    DLLdestroy(list);
    //free(list->head);
  //  printf("%s\n", list->head->data);

    

}
