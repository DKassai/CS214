#include <stdlib.h>
//#include "../DLL.h"
#include "../FinalToken.h"


int main (int argc , char** argv){
   // this is for reading a file;
    char * intake = argv[1];
    char * string = Get_string(intake);

    DLL * output;
    printf("%s\n", Get_string(intake));
    printf("-----------------------\n");

    // this is getting the words from the actual function 
    output = get_tokens(string);
    
    DLL_node * ptr;
    for (ptr=output->head; ptr!=NULL; ptr=ptr->next){
       printf("%s\n", ptr->data);
   }






}