#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/syscall.h>  
#define gettid() syscall(__NR_gettid)  

#define DELIM " \t\r\n\a"
#define READIN_BUF_SIZE 20000

pthread_attr_t attr;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
int barrier;
int indata[4096];
typedef struct parameters{
          int lpos;
          int rpos;
          int rightend;
}parameters;

void *Merge(void *para);

int main(int argc, char *argv[])
{
	int index=0;
        int numberOfElements=0;
	char *number;
        double round;
	int thread;
	int round_i;
	int start_pos;
	int sub_length=1;
        int thread_ptr=0;
	int thread_num;
	pthread_t tid[4096];
        
       

	FILE *stream;
        char input[READIN_BUF_SIZE];
	if((stream=fopen("indata.txt","r"))==NULL) {  
        printf("cannot open file\n"); exit(1);  
    	}
	fgets(input,READIN_BUF_SIZE,stream);
	
	
			number = strtok(input, DELIM);
        		while (number != NULL){
			     indata[index] = atoi(number);
			     index++;
			     number = strtok(NULL, DELIM);
			}
	
	numberOfElements=index;
	round=log10((double)numberOfElements)/log10(2);

	for(index=0; index<numberOfElements; index++){
			printf("%d ",indata[index]);
        }
	printf("\n");


	
	thread_num=numberOfElements/2;

	for(round_i=1;round_i<=(int)round;round_i++){
	 parameters index[thread_num];
	 barrier=thread_num;
	 sub_length=sub_length*2;	
	 for(start_pos=0;start_pos<thread_num;start_pos++){
	    	index[start_pos].lpos=sub_length*start_pos;
		index[start_pos].rightend=sub_length*(start_pos+1)-1;
		index[start_pos].rpos=(index[start_pos].lpos+index[start_pos].rightend+1)/2;

		pthread_attr_init(&attr);
		pthread_create(&tid[thread_ptr],&attr, Merge, (void*)&index[start_pos]);
		thread_ptr++;

	}	
		pthread_mutex_lock(&mutex);
              	while (barrier>0) {
                      pthread_cond_wait(&cond, &mutex);
              }
                       pthread_cond_broadcast(&cond);
              pthread_mutex_unlock(&mutex);
	      
		

	      thread_num=thread_num/2;
		pthread_join(tid[thread_num],NULL);
		}	
	

	
	
		//printf("\n %d",numberOfElements); 
		//printf("\n %f \n",round);
	for(index=0; index<numberOfElements; index++){
			printf("%d ",indata[index]);
        }
 	printf("\n");
}

// Function to Merge Arrays L and R into A. 
// lpos= start position of L in A
// rpos = start position of R in A
// rightend= end of the R array
void *Merge(void *para) {
    struct parameters *index= (struct parameters*)para;
    int i = index->lpos, j = index->rpos, k = 0;
    int total = index->rightend-index->lpos+1;
    int *temp = (int *)malloc(sizeof(int) * total);
    if(temp == NULL)
    {
        fprintf(stderr, "malloc error in merge function\n");
        return;
    }
    while(i < index->rpos && j <= index->rightend)
    {
        if(indata[i] <=indata[j])
            temp[k++] = indata[i++];
        else
            temp[k++] = indata[j++];
    }

    
        while(i< index->rpos)
            temp[k++] = indata[i++];
   
  
        while(j <= index->rightend)
            temp[k++] = indata[j++];

    /*Copy the temp to the source array*/
    for(i = 0, k = index->lpos; i < total; k++, i++)
        indata[k] = temp[i];
	

    free(temp);
	
	pthread_mutex_lock(&mutex);
	//printf("Thread id:%d \n", gettid()); 
	//printf("1\n");
    	barrier--;
	while (barrier>0) {
                      pthread_cond_wait(&cond, &mutex);
              }
                       pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
	//printf("2\n");
}







