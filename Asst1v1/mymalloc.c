#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include"mymalloc.h"

void metadata(size_t requested, int loc, int left){
 
    if (left==1){
        myblock[loc] = requested / 64 + 64;
        myblock[loc+1] = requested % 64 + 1;
	return;
    }else if (left){
        myblock[loc+2+requested] = left / 64;
        myblock[loc+3+requested] = left % 64;
    }
    	myblock[loc+1] = requested % 64;
    	myblock[loc] = (requested / 64) + 64;
    
}

//change location our metadata block points to to a value
void* mymalloc(size_t requested_size, char* file_name, size_t line ){
    // we need to set the first two bytes for the meta data 
    // then the next two 
    // checking if the data is equal to 100 and 86 

    if (!((int)myblock[1] == 100) || !((int)myblock[0] == 86)){
      if (requested_size<=4092){
          int first = 86;
          int second = 100;   
        myblock[0]= (char)first;
        myblock[1]= (char)second;
           // then create two blocks for meta data
           // we have the first blocks value and we have a 1 that means used 
           // this is how we will
           char* location = &myblock[4];
           metadata(requested_size, 2, 4092-requested_size);
           return location;
      }
      fprintf(stderr,"Error: Not enough space in heap: %s, line %zu.\n",file_name,line);
      return NULL;

    }
    // starting from the first location which would be mydata[2] if it is greater than or equal 
    int i = 2;
    int space = 0;
    int start = 0;


    while (i < 4096){
            if ((int)myblock[i] < 64){
            if (!start){
                start = i;
                space-=2;
            }
            space += ((int)myblock[i])*64+(int)myblock[i+1]+2;
            i += ((int)myblock[i])*64+(int)myblock[i+1]+2;
        } else{
            start = 0;
            space = 0;
            i += ((int)myblock[i]-64)*64+(int)myblock[i+1]+2;
        }
        if (space >= requested_size){
            int leftover = space - requested_size;
            char* returned_loc = &myblock[start+2];
            metadata(requested_size, start, leftover);
            return returned_loc;
        }
    }
    fprintf(stderr,"Error: Not enough space in heap: %s, line %zu.\n",file_name,line);
    return 0;
}

//Change first bit (our significand) of metadata to 0 instead of 1;
void myfree(char* Asir, char* file_name, size_t line){
    if (Asir==0){ 	
	fprintf(stderr,"Error: Item is not able to be freed: %s, line %zu.\n",file_name,line);
	return;
    }
    char* MetaBlock = (Asir-2);
    if (!((int)*(MetaBlock) < 64)){
        *MetaBlock -= 64;
    } else{
	fprintf(stderr,"Error: Item is not able to be freed: %s, line %zu.\n",file_name,line);
    }
}

//this is the final version!!!
