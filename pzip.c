#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include "wzip11.c"


int main(int argc, char *argv[]){



    //arg args ={argv,1};
    ret *m;
    int fileName=1;
    int threadNumber = argc-1;
    //printf("%c",str); 
    pthread_t threadArray[threadNumber];
    //pthread_create(&p, NULL, zip, (void *) str);
    //zip((void *)str);
    for (int i=0;i<threadNumber;i++ )
    {
    	char *str = argv[fileName];
	int rc= pthread_create(&threadArray[i], NULL, zip, (void *) str);
	assert(rc ==0);
	fileName++;   	
    }
    for ( int i=0;i<threadNumber;i++ ){
    	 pthread_join(threadArray[i], (void **)&m); 
    	 int s=m->size;
    	 //printf("%d\n",s);
    	 for(int j=0;j<s;j++){
	    	 int countt=m->cou[j];
	    	 char letter=m->lett[j];
	    	 //printf("%d",m->cou[j]);
    	 	 //printf("%c",m->lett[j]);
    	 	 //printf("\n");
	    	 fwrite(&countt,1,4,stdout);
	    	 fwrite(&letter,1,1,stdout);
	    	 //printf("\n");

    	 }
	//r->cou[v]=count[0];
	//r->lett[v]=oldbuff[0];
	//fwrite(oldbuff,1,1,stdout);
	
    	 
     
    }
    return 0;
}
