#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>
#include <assert.h>
typedef struct __rett{
	int *cou;
	char *lett;
	int size;
}ret;


void *zip(void *arg){
    int v=0; 
    long long n= 1000000;
    char *argv=(char *) arg;
    //char *argv[]= &args->fileName;
    //int i= args->fileCount;
    FILE *fp;
    char newbuff[5], oldbuff[5];
    int count[2];
    count[0]=0;
    
    ret *r= malloc( sizeof(ret) );
    r->cou=(int*)malloc(n * sizeof(int));
    r->lett=(char*)malloc(n * sizeof(char));
    //int *c = (int*)malloc(n * sizeof(int));
    //char *l = (char*)malloc(n * sizeof(char));
    //printf("hi\n");
    if ((fp = fopen(argv, "r")) == NULL){
            printf("wzip: cannot open file\n");
            exit(EXIT_FAILURE);
            }
    fp = fopen(argv, "r");
    while (fread(newbuff, 1, 1, fp)){ 
	if (newbuff[0]==oldbuff[0]){
		//printf("hi hi");
		 count[0]++;
		 } 
	else {
		 if (oldbuff[0] != '\0'){
			 if (count[0] !=0)
			 {
				 //printf("%d%c\n",count[0],oldbuff[0]);
				 //fwrite(count,1,4,stdout);
				 r->cou[v]=count[0];
				 r->lett[v]=oldbuff[0];
				 v++;
				 //fwrite(oldbuff,1,1,stdout);
			 }
		 }
		 count[0]= 1;
		 strcpy(oldbuff, newbuff);
	     }
	}
	fclose(fp);
    
	//fwrite(count,1,4,stdout);
        //fwrite(oldbuff,1,1,stdout);
	 r->cou[v]=count[0];
         r->lett[v]=oldbuff[0];
         r->size=v;
	return (void *)r;
}

