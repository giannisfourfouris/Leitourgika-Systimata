#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include"p3150190_p3150102_p3150224_res1.h"
#include <pthread.h>

#include <unistd.h>

int logariasmos=0;
int arithmosSunalagwn=0;
int pelates=100;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
/*
 * O counter einai ekswteriki metavliti gia na ton vlepoun ola ta threads.
*/
int counter;

pthread_mutex_t mutexcount;

/**
 * H synartisi pou tha kaleitai apo ta threads kai tha einai ypeythini gia tin ayksisi tou counter.
*/
void *increaseCounter(void *threadId) {
	int *tid = (int *)threadId;
    	int prevValue, rc;
	//kleidwma tou mutex protou allaksoume tin timi
	rc = pthread_mutex_lock(&mutexcount);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}

	printf("Thread %d: About to increase the counter.\n", *tid);
	prevValue = counter;
	counter++;
	sleep(1);
	printf("Thread %d: Previous value = %d, New value = %d.\n", *tid, prevValue, counter);

 

	rc = pthread_mutex_unlock(&mutexcount);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}

	pthread_exit(threadId);
}

int main(int argc, char** argv) {  
        
printf("Nseat: %d \n",Nseat);
printf("Ntel: %d \n",Ntel);
printf("Nseatlow: %d \n",Nseatlow);
printf("Nseathigh: %d \n",Nseathigh);
printf("tseatlow: %d \n",tseatlow);
printf("tseathigh: %d \n",tseathigh);
printf("Nseat: %lf \n",Pcardsucces);
printf("Cseat: %d \n",Cseat);
printf("logariasmos: %d \n",logariasmos);
printf("arithmosSunalagwn: %d \n",arithmosSunalagwn);

int a=time(NULL);
int num=rand_r(&a)%Nseathigh+Nseatlow;
printf("num: %d \n",num);

int rc;
	pthread_t *threads;

	threads = malloc(pelates * sizeof(pthread_t));
	if (threads == NULL) {
		printf("NOT ENOUGH MEMORY!\n");
		return -1;
	}
	//arxikopoiisi counter
	counter = 0;

	//arxikopoiisi tou mutex.
	rc = pthread_mutex_init(&mutexcount, NULL);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
 int threadIds[pelates];
void *status;
	int i = 0;
	for (i = 0; i < pelates; i++) {
		threadIds[i] = i + 1;
		printf("Main: creating thread %d\n", threadIds[i]);
		
		/*dimiourgia tou thread*/
    		rc = pthread_create(&threads[i], NULL, increaseCounter, &threadIds[i]);
                //sleep(2);
		/*elegxos oti to thread dimiourgithike swsta.*/
    		if (rc != 0) {
    			printf("ERROR: return code from pthread_create() is %d\n", rc);
       			exit(-1);
       		}

		rc = pthread_join(threads[i], &status);
            
		
		if (rc != 0) {
			printf("ERROR: return code from pthread_join() is %d\n", rc);
			exit(-1);		
		}

		printf("Main: Thread %d finished with status %d.\n", threadIds[i], *(int *)status);
	}
 
	
	

	printf("Counter value after threads finished = %d.\n", counter);

	//"katastrofi" tou mutex.
	rc = pthread_mutex_destroy(&mutexcount);
	if (rc != 0) {
   		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
      		exit(-1);
	}
free(threads);
	return 1;
}
