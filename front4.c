#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
 int pinakas[5];
int counter =0;
 pthread_mutex_t lock;
 pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
 int tilefonites = 2; //diathesimoi tilefonites
 void *pelatis(void *x){
 int id = (int *)x;
 int rc;
 printf("Hello from pelati: %d\n",id);
 rc = pthread_mutex_lock(&lock);
 while (tilefonites == 0) {
 printf("O pelatis %d, den brike diathesimo tilefoniti.Blocked...\n", id);
 rc = pthread_cond_wait(&cond, &lock);
 }
 printf("O pelatis %d, eksipiretite.\n",id);
 
 tilefonites--;
 rc = pthread_mutex_unlock(&lock);

 sleep(5); //kane kapoia douleia me ton tilefoniti
rc = pthread_mutex_lock(&lock);
counter++;
 if(pinakas[counter]==0 && counter<=5){
printf("O pelatis %d,mphke sto pinaka sth thesi %d.\n",id,counter);
}else{
printf("O pelatis %d,phre poulo.\n",id);
}
rc = pthread_mutex_unlock(&lock);

 rc = pthread_mutex_lock(&lock);
 printf("O pelatis %d eksipiretithike epitixos! \n", id);
 tilefonites++;
 rc = pthread_cond_signal(&cond);
 rc = pthread_mutex_unlock(&lock);
 pthread_exit(NULL); //return
 }


 int main() {
for (int i = 0; i < 5; i++) {
 pinakas[i]=0;
}

 int N = 10; int rc;
 pthread_t threads[N];
 int id[N];
 pthread_mutex_init(&lock, NULL);
 for (int i = 0; i < N; i++) {
 id[i] = i+1;
 printf("Main: creating thread %d\n", i+1);
 rc = pthread_create(&threads[i], NULL, pelatis, id[i]);
 }

 for (int i = 0; i < N; i++) {
 pthread_join(threads[i], NULL);
 }
 pthread_mutex_destroy(&lock);
 pthread_cond_destroy(&cond);
 return 0;
 } 
