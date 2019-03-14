#include <stdlib.h>
#include "mymalloc.h"
#include <time.h>
#include <sys/time.h>




// make sure at the end of each test case that you free what is in the case 
// this is where it comes to check if there is metadata that is in the place 

// to free if the thign is not equal to zero that means there is something to free there 


        struct timeval tv ;

 int main(int argv, char** argc){


    // Test A 

        gettimeofday(&tv,NULL);
        time_t start = tv.tv_sec;
        suseconds_t start_ms = tv.tv_usec;
        int l=0;
	double how_long;
        while (l<100){
            int i=0;
            while (i<150){
            	char * p =  malloc(1);
            	free(p);
                i++;
            }
        l++;
        }
        gettimeofday(&tv,NULL);
        time_t end = tv.tv_sec;
        suseconds_t end_ms = tv.tv_usec;
        if (end_ms<start_ms){
            end--;
            suseconds_t temp=end_ms;
            end_ms=start_ms;
            start_ms=temp;

        }
        printf("this is the amount microseconds it takes to run testcase A %ld.%d\n", (end-start)/100, (end_ms-start_ms)/100);

   

 

    // test case B 
   
    double average =0;
    int k;
    int o;
     
        gettimeofday(&tv,NULL);
        time_t start2 = tv.tv_sec;
        suseconds_t start_ms2 = tv.tv_usec;
    l=0;
    while(l<100){
        for (k=0; k<3; k++){
            char * p[50]; 
            for (o=0;o<50;o++){
                p[o]=(char*)malloc(1);
            }   
            for (o=0;o<50;o++){
                free(p[o]);
            }
        }
        l++;
    }
    gettimeofday(&tv,NULL);
        time_t end2 = tv.tv_sec;
        suseconds_t end_ms2 = tv.tv_usec;
        if (end_ms2<start_ms2){
            end2--;
            suseconds_t temp2=end_ms2;
            end_ms2=start_ms2;
            start_ms2=temp2;

        }
        printf("this is the amount microseconds it takes to run testcase B %ld.%d\n", (end2-start2)/100, (end_ms2-start_ms2)/100);




    // test case C
        gettimeofday(&tv,NULL);
        time_t start3 = tv.tv_sec;
        suseconds_t start_ms3 = tv.tv_usec;
    l=0;
    
    while (l<100){
        int total_couter=0;
        int i=0;
        char * ptr[50];
        while (total_couter<50){
            int p = rand()%2;
            if (p==0 && total_couter==0){
                continue;
            }
            if (p==1){
                ptr[i++]=(char*)malloc(1);
                total_couter++;

            }else{
                free(ptr[--i]);
               total_couter--;
            }
        }

        while (i>0){
            free(ptr[--i]);
        }
        l++;
    }    
      gettimeofday(&tv,NULL);
        time_t end3= tv.tv_sec;
        suseconds_t end_ms3 = tv.tv_usec;
        if (end_ms3<start_ms3){
            end3--;
            suseconds_t temp3=end_ms3;
            end_ms3=start_ms3;
            start_ms3=temp3;

        }
        printf("this is the amount microseconds it takes to run testcase C %ld.%d\n", (end3-start3)/100, (end_ms3-start_ms3)/100);
   
    


    gettimeofday(&tv,NULL);
    time_t start4 = tv.tv_sec;
    suseconds_t start_ms4 = tv.tv_usec;

    l=0;
    while (l<100){
        
        int i=0;
        char* ptr[50];
        while (i<50){
            int p = rand()%2;
            if (p==0 && i==0){
                continue;
            }

            if (p){
		int malloc_size = rand()%64 + 1;
                ptr[i]=(char*)malloc(malloc_size);
		i++;

            }else{
                free(ptr[--i]);
            }
        }

        while (i>0){
            free(ptr[--i]);
        }
        l++;
    }   
 
    gettimeofday(&tv,NULL);
    time_t end4= tv.tv_sec;
    suseconds_t end_ms4 = tv.tv_usec;
    if (end_ms4<start_ms4){
        end4--;
        suseconds_t temp4=end_ms4;
        end_ms4=start_ms4;
        start_ms4=temp4;

    }
        printf("this is the amount microseconds it takes to run testcase D %ld.%d\n", (end4-start4)/100, (end_ms4-start_ms4)/100);
//        return 0;
    //





//custom case 1 
// this case is an error catch prevention. The way it is doing this is it first mallocs somethings normally of an arbitrary value we chose 65 for fun
// then it inputs something of size 4092 which should fit but since there is already something there it prints out an error becuase there is not enough space. 
// then it tries to free the same thing twice. This is why we get an error of an item not being able to be freed.
   gettimeofday(&tv,NULL);
    time_t start5 = tv.tv_sec;
    suseconds_t start_ms5 = tv.tv_usec;
    l=0;
    while (l<100){
        char * testcase;
        char * errorCase;
        int i=0;
        testcase= (char*)malloc(65);
	//Next line tries to assign more value than the memory has. Error impending.
        errorCase = (char*)malloc(4092);
	free(testcase);
	//Next line tries to free an already freed value. Error impending.
	free(testcase);
	//free(errorCase);
	
        l++;
    }
    gettimeofday(&tv,NULL);
    time_t end5= tv.tv_sec;
    suseconds_t end_ms5 = tv.tv_usec;
    if (end_ms5<start_ms5){
        end5--;
        suseconds_t temp5=end_ms5;
        end_ms5=start_ms5;
        start_ms5=temp5;

    }
        printf("this is the amount microseconds it takes to run testcase E %ld.%d\n", (end5-start5)/100, (end_ms5-start_ms5)/100);

    


// custom test case 2 
    gettimeofday(&tv,NULL);
    time_t start6 = tv.tv_sec;
    suseconds_t start_ms6 = tv.tv_usec;
    l=0;
    while (l<100){
    char* array[100];
    int i = 0;
    // this case is here so you dont free something out side of the array 
    array[i++] = (char*)malloc(rand()%5+1);
    while(i < 100){
        array[i++] = (char*)malloc(rand()%5+1);
        free(array[i-2]);
    }
    free(array[99]);
    l++;
    }
    
    gettimeofday(&tv,NULL);
    time_t end6= tv.tv_sec;
    suseconds_t end_ms6 = tv.tv_usec;   
    if (end_ms6<start_ms6){
        end6--;
        suseconds_t temp6=end_ms6;
        end_ms6=start_ms6;
        start_ms6=temp6;

    }
    printf("this is the amount microseconds it takes to run testcase F %ld.%d\n", (end6-start6)/100, (end_ms6-start_ms6)/100);
    

}


// this is the final version 




    





