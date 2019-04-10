#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>

#include "DLL.h"
#include "tokenize.h"


// this takes in the file and then takes the words and puts it into a string
char * Get_string(char * file_name){
    char * input = file_name;
    int file = open(input, O_RDONLY);
    if (file==-1){
        //printf("%d: %s\n", errno, strerror(errno));
        return NULL;
    }
    int length = lseek(file, 0, SEEK_END);
    lseek(file,0, SEEK_SET);
    char * contents = malloc(length*sizeof(char)+1);
    int status = read(file, contents, length);
    if (status == -1){
      //  printf("%d: %s\n", errno, strerror(errno));
        free(contents);
        close(file);
        return NULL;
    }
    //free(contents);
    close(file);
    contents[length]='\0';
    return contents;
 
}

// this breaks down the string into exact words based on if it is a space or it is a control 
DLL * get_tokens( char * string){
    char * input = string;
    DLL * list = DLLCreate();
    int i,j;
   // for (i=0;i<strlen(input);i++)
    i=0;
    while(i<strlen(input)){
        if ((!iscntrl(input[i]) && !isspace(input[i]))){
            j=i;
            while (!iscntrl(input[i]) && !isspace(input[i]) && i<strlen(input)){
                i++;
            }
            // it could be i-j+1
            char * new_token= malloc(sizeof(char)* (i-j+2));
            strncpy(new_token,input+j,i-j+1);
            new_token[i-j]='\0';
            DLLinsert(list,new_token);
        }else {
            char * delimiter = malloc(sizeof(char)*2);
            strncpy(delimiter, input+i,1);
            delimiter[1] = '\0';
            DLLinsert(list,delimiter);
            i++;
        }

    }
    return list;
}


DLL * it_dir(char * directory_name){
    DIR * directory = opendir(directory_name);
    if (directory==NULL){
        return NULL;
    }
    DLL * files = DLLCreate();
    struct dirent * entry;
    int status;
    while ((entry=readdir(directory))!=NULL){
        if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0){
            continue;
        }
        char * entry_name;
        asprintf(&entry_name, "%s/%s", directory_name,entry->d_name);
        if (entry->d_type== DT_REG){

            DLLinsert(files, entry_name);
         
        }else if (entry->d_type == DT_DIR){
            DLL * dir_files = it_dir(entry_name);
            DLL_node * node = dir_files->head;
            while (node != NULL) {
                DLLinsert(files, node->data);
                node = node->next;
            }
            DLLdestroy(dir_files);
            free(entry_name);
        }
    }
    return files;
}


