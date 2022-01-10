#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <sys/mman.h> //Library for mmap
#include <pthread.h>
#include <sys/stat.h> //Library for struct stat
#include <sys/sysinfo.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>

//our data struct.
int done=0;
int fill=0; 
int use=0; 
int count=0;
int num_files;
int totalPages=0;
int *pagesForEachFiles;
#define max 100
int defaultSize; 

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER, full = PTHREAD_COND_INITIALIZER;
struct page
{
	char* address; //Mapping address of the file_number file + page_number
	int parentFile;
	int pageNum;
	int pageSize;
}buffer[max];
typedef struct __rett{
	int *cou;
	char *lett;
	int size;
}ret ;
ret *out[sizeof(ret)* 512000*2];

void* produce(void *arg);
void zip(struct page temp);
void *consume();

int main(int argc, char *argv[]){
//clock_t t;
    //t = clock();

    defaultSize=10000000;
    int threadNumber = get_nprocs();
    num_files = argc-1;
    pagesForEachFiles= malloc(sizeof(int)*argc);
    //created producer and concumer
    pthread_t consumersArray[threadNumber],producer;
    
    //call the producer
    int rc= pthread_create(&producer, NULL, produce, argv+1);
    assert(rc ==0);
    //out=malloc(sizeof(ret)* 512000*2);
    //create consumers    
    for (int i=0;i<threadNumber;i++ )
    {
	int rc= pthread_create(&consumersArray[i], NULL, consume, NULL);
	assert(rc ==0);
    }
     for (int i=0;i<threadNumber;i++ )
    {
	pthread_join(consumersArray[i], NULL);
    }
    pthread_join(producer, NULL);  
    //printf("hi");
    //printing stage
    for ( int i=0;i<totalPages;i++ ){
    	 int s=out[i]->size;
    	 for(int j=0;j<s;j++){
    	         int countt=out[i]->cou[j];
    	         char letter=out[i]->lett[j];
    	             while((int)out[i]->lett[j] ==(int) out[i]->lett[j+1] && j!=s-1){
    	                 countt=out[i]->cou[j]+out[i]->cou[j+1];
	    	 	 j++;
	    	 	 continue;
    	             }	

	    	 fwrite(&countt,1,4,stdout);
	    	 fwrite(&letter,1,1,stdout);

	    	     }
    	 }
    	  
    	  
    	
    	 free(pagesForEachFiles);
    	 //free(out);
    	//  t = clock() - t;
    //double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds  
    //printf("we took %f seconds to execute \n", time_taken); 
    return 0;
}

void* produce(void *arg){
                 //printf("produce says hi\n");
	char** filenames = (char **)arg;
	struct stat sb;
	char* map; 
	int file;
	for(int i=0;i<num_files;i++){
	        int pages_in_file=0; 
		int last_page_size=0;
		//printf("%s\n",filenames[i]);
		file = open(filenames[i], O_RDONLY);		
		if(file == -1){ 
			printf(" hi Error: File didn't open\n");
			exit(1);
		}
		if(fstat(file,&sb) == -1){ 
			close(file);
			printf("Error: Couldn't retrieve file stats");
			exit(1);
		}
        	if(sb.st_size==0){
               		continue;
        	}
		pages_in_file=(sb.st_size/defaultSize);
		if(((double)sb.st_size/defaultSize)>pages_in_file){ 
			pages_in_file+=1;
			last_page_size=sb.st_size%defaultSize;
		}
		else{
			last_page_size=defaultSize;
		}
		
		totalPages+=pages_in_file;
		//pagesForEachFiles[i]=pages_in_file;
		//maping each file
	       map = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, file, 0); 															  
		if (map == MAP_FAILED){
			close(file);
			printf("Error mmapping the file\n");
			exit(1);
    	}	

		for(int j=0;j<pages_in_file;j++){
			
			struct page temp;
			if(j==pages_in_file-1){ 
				temp.pageSize=last_page_size;
			}
			else{
				temp.pageSize=defaultSize;
			}
			temp.address=map;
			temp.pageNum=j;
			temp.parentFile=i;
			
			map+=defaultSize; 
			
			pthread_mutex_lock(&lock);

			//put the page in the buffer
			buffer[fill] = temp; 
  			fill = (fill + 1) % max;
  			count++;
			
			pthread_mutex_unlock(&lock);
			
			pthread_cond_signal(&full);
		}
		
		//Step 7: Close the file.
		close(file);
	}

	done=1; 
	pthread_cond_broadcast(&full); 
	return 0;
}

void *consume(){
        //printf("consume says hi\n");
	do{
		pthread_mutex_lock(&lock);
		
		while(count==0 && done==0){
		    //pthread_cond_signal(&empty);
		    pthread_cond_wait(&full,&lock); 
		}
		if(done==1 && count==0){ 
			pthread_mutex_unlock(&lock);
			return NULL;
		}
		struct page temp = buffer[use];
		use = (use + 1) % max;
	  	count--;
	  	
		if(done==0){
		    pthread_cond_signal(&empty);
		}	
		
		pthread_mutex_unlock(&lock);
		
		zip(temp);
	}while(!(done==1 && count==0));
	return NULL;
}


void zip(struct page temp){
    int v=0; 
    //int n= 100;
    char  oldbuff[5];
    oldbuff[0]='\0';
    int count[2];
    count[0]=0;   
    ret *r= malloc( sizeof(ret)*100 );
    r->cou=malloc(temp.pageSize*sizeof(int));
    r->lett=malloc(temp.pageSize*sizeof(char));
    
    for (int i=0;i<temp.pageSize;i++){ 
        //newbuff[0]= temp.address[i];
	if (temp.address[i]==oldbuff[0]){
		 count[0]++;
		 } 
	else {
		 if (oldbuff[0] != '\0'){
			 if (count[0] !=0)
			 {
				 r->cou[v]=count[0];
				 r->lett[v]=oldbuff[0];
				 v++;
			 }
		 }
		 count[0]= 1;
		 oldbuff[0]= temp.address[i];
	     }
	}
    
	 r->cou[v]=count[0];
         r->lett[v]=oldbuff[0];
         r->size=v;
         
         //calculate position
         int position=0;
	 for(int i=0;i<temp.parentFile;i++){
		position+=pagesForEachFiles[i];
	 }
	 position+=temp.parentFile;
	 out[position]=r;	 
}

