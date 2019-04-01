/**
	Ylopoiiste ena programma to opoio tha dimiourgei 5
	threads ta opoia tha moirazontai enan koino metriti (counter) o opoio 
	tha exei arxiki timi 0. Skopos tis askisis einai me to peras tis ektelesis
	olwn twn threads o counter na exei timi 5 dhladh to kathe thread tha prepei
	na ayksanei ton counter kata 1. Epipleon, kata ti diarkeia tis ektelesis 
    	to kathe thread na emfanizei tin proigoumeni kai tin trexousa timi tou 
	counter. I teliki timi tha prepei na ektypwnetai apo ti main synartisi tou 		programmatos meta tin ektelesi twn threads. 
	Aparaitites synartiseis tis pthread.h vivliothikis:
	int pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
		void (*start_routine)(void), void *arg);
	void pthread_exit(void *value_ptr);
	int pthread_join(pthread_t thread, void **value_ptr);
	int pthread_mutex_init(pthread_mutex_t *mutex, 
    	const pthread_mutexattr_t *attr);
	int pthread_mutex_destroy(pthread_mutex_t *mutex);
	int pthread_mutex_lock(pthread_mutex_t *mutex);
	int pthread_mutex_unlock(pthread_mutex_t *mutex);
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * O counter einai ekswteriki metavliti gia na ton vlepoun ola ta threads.
*/
int counter;

/*
 * To mutex pou tha xrisimopoiithei gia tin prostasia tis koinis metavlitis, einai
 * ekswteriki metavliti prokeimenou na einai orato apo ta threads.
*/
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

/**
 * H main synartisi pou tha arxikopoiei ton counter kai ta threads. Tha perimenei
 * tin oloklirwsi twn threads kai tha typwnei tin teliki timi tou counter.
*/
int main(int argc, char *argv[]) {
	
	int rc;
	pthread_t threads[10];
	int threadIds[10];
	//arxikopoiisi counter
	counter = 0;

	//arxikopoiisi tou mutex.
	rc = pthread_mutex_init(&mutexcount, NULL);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}

	int i = 0;
	for (i = 0; i < 5; i++) {
		threadIds[i] = i + 1;
		printf("Main: creating thread %d\n", threadIds[i]);
		
		/*dimiourgia tou thread*/
    		rc = pthread_create(&threads[i], NULL, increaseCounter, &threadIds[i]);

		/*elegxos oti to thread dimiourgithike swsta.*/
    		if (rc != 0) {
    			printf("ERROR: return code from pthread_create() is %d\n", rc);
       			exit(-1);
       		}
	}
 
	void *status;
	for (i = 0; i < 5; i++) {
		rc = pthread_join(threads[i], &status);
		
		if (rc != 0) {
			printf("ERROR: return code from pthread_join() is %d\n", rc);
			exit(-1);		
		}

		printf("Main: Thread %d finished with status %d.\n", threadIds[i], *(int *)status);
	}

	printf("Counter value after threads finished = %d.\n", counter);
for (i = 5; i < 10; i++) {
		threadIds[i] = i + 1;
		printf("Main: creating thread %d\n", threadIds[i]);
		
		/*dimiourgia tou thread*/
    		rc = pthread_create(&threads[i], NULL, increaseCounter, &threadIds[i]);

		/*elegxos oti to thread dimiourgithike swsta.*/
    		if (rc != 0) {
    			printf("ERROR: return code from pthread_create() is %d\n", rc);
       			exit(-1);
       		}
	}
 
	
	for (i = 5; i < 10; i++) {
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

	return 1;
}