#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include"p3150190_p3150102_p3150224_res1.h"
#include <pthread.h>

int Ncust;
int seed;
int account=0;
int counterTransaction=0;
int *seatArray;
//krataei an apetyxe h oxi ena thread
int rc;
int *threadId;
int *standByTime;
int *handlingTime;
int counterSeat=0;
struct timespec startStandBy, stopStandBy;
struct timespec startHandlingTime, stopHandlingTime;
pthread_mutex_t lock;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
 


/**
 * H synartisi pou tha kaleite apo ta threads gia na 
 * vlepei kai na apothikevei theseis, ypologisei arithmo sunalagwn kai
 * prosthetei sto tamio ta leftta
*/
 void *customer(void *x){
 	int id = (int *)x;
	
 	//printf("Hello from pelati: %d\n",id);
 	rc = pthread_mutex_lock(&lock);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_lock is %d\n", rc);
			exit(-1);		
		}
	//perimenei mexri na vrei diathesimo thlefwnhti
 	while (Ntel == 0) {
 		printf("O pelatis %d, den brike diathesimo tilefoniti.Blocked...\n", id);
 		rc = pthread_cond_wait(&cond, &lock);
		if (rc != 0) {
			printf("ERROR: return code from pthread_cond_wait is %d\n", rc);
			exit(-1);		
		}
 	}
 	printf("O pelatis %d, eksipiretite.\n",id);
       
	//teleiwnei to xrono anamonis	
	if( clock_gettime( CLOCK_REALTIME, &stopStandBy) == -1 ) {
      		perror( "clock gettime" );
      		exit( EXIT_FAILURE );
    	}
	//ypologizwi to xrono anamonis
        standByTime[id-1] = ( stopStandBy.tv_sec - startStandBy.tv_sec );
        //printf("Teleisa me wra Standby time: %d\n",standByTime[id-1]);

	//epilegei tuxaio arithmo thesewn
 	int randomTicketNumber=rand_r(&seed)%Nseathigh +Nseatlow;
 	//printf("The random number for ticket is: %d\n",randomTicketNumber);


	//epilegei tuxaio xrono gia sleep
 	int randomWaitTime;
	int rand=rand_r(&seed)%tseathigh;
	if(rand>=5){
		randomWaitTime=rand;
  
         }else{
		randomWaitTime=rand +tseatlow;	
	}
 	//printf("The random number of time to wait is: %d\n",randomWaitTime);
 	Ntel--;

 	rc = pthread_mutex_unlock(&lock);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_unlock is %d\n", rc);
			exit(-1);		
		}

 	sleep(randomWaitTime); //kane kapoia douleia me ton tilefoniti perimenei gia tyxaio xrono apo 5-10 sec

	//edw elegxw an exei arketes kenes theseis
	rc = pthread_mutex_lock(&lock);
	sleep(1);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_lock is %d\n", rc);
			exit(-1);		
	}
        int checkIfSeatReserved=0;
 	int lastSeat=(counterSeat+randomTicketNumber);
	for(int i=counterSeat; i<lastSeat;i++){
		if(counterSeat>Nseat){
			//elegxw an to theatro einai gemato 
			printf("ID: %d To theatro einai gemato.\n",id);
			break;	
		}else if(Nseat-counterSeat<randomTicketNumber){ 
			//elegxw an exei arketes theseis
			printf("ID: %d H krathsh mataiwthike giati den exei arketes diathesimes theseis\n.",id);
			break;
		}else{ 
			//desmevw theseis
			seatArray[i]=id;
			//printf("Eimai to seatArray i: %d kai eimai piasmenos apo to pelati %d \n",i,seatArray[i]);
			checkIfSeatReserved=1;	
			
		}
 	}
	rc = pthread_mutex_unlock(&lock);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_unlock is %d\n", rc);
			exit(-1);		
		}
	//an desmefthkan theseis tote proxwraw sthn plhrwmh
	//teleiwnw me to searching kai to kleisimo twn thesewn
 	rc = pthread_mutex_lock(&lock);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_lock is %d\n", rc);
			exit(-1);		
		}
	if(checkIfSeatReserved==1){
		//dhmiourgei tuxaious arithmous metaksi 1-10.	
		//an aftos o arithmos<=9 tote kanei accept
		//alliws kanei reject
		int randomPercentage=rand_r(&seed)%10+1;
		//printf("Eimai to random Perc: %d\n",randomPercentage);
		if(randomPercentage>Pcardsucces){
			//elegxw an to h plhrwmh egine apodekth. An den egine apodekth tote ksedesmevw tis theseis.
			printf("ID: %d h krathsh mataiwthike giati h pistwtikh karta den egine apodekth.\n",id);
			for(int i=lastSeat-1; i>=counterSeat;i--){
				seatArray[i]=0;
			}
			
				
		}else{
			//enimerwsi arithmou sinallagis
			counterTransaction++;
			printf("ID: %d h krathsh sas oloklrwthike epitexws. O arithmos sunalaghs einai %d. Oi theseis sas einai: ",id,counterTransaction);
			for(int i=counterSeat; i<lastSeat;i++){
				printf("%d,  ",i+1);
			}
			printf(" kai to kostos synalagis einai %d.\n",Cseat*randomTicketNumber);
			//enimerwsi tou logariasmou
			account= account + (Cseat*randomTicketNumber);
			//enhmerwnw to counterSeat gia na kserw meta apo poia thesei toy pinaka exw kenes theseis
			counterSeat=lastSeat;
			//se periptwsh pou einai gemato to theatro kanw to counterSeat kati megalytero
			//wste sthn parapanv epanalhpsh na kserei oti einai gemato
			if(counterSeat==Nseat){
				counterSeat=251;
			}
	
		}

	}	
	

	

	

 	printf("O pelatis %d eksipiretithike epitixos! \n", id);
 	Ntel++;
 	rc = pthread_cond_signal(&cond);
	if (rc != 0) {
			printf("ERROR: return code from pthread_cond_signal is %d\n", rc);
			exit(-1);		
		}
	
	//ipologismos handling time
	if( clock_gettime( CLOCK_REALTIME, &stopHandlingTime) == -1 ) {
      		perror( "clock gettime" );
      		exit( EXIT_FAILURE );
	}
	handlingTime[id-1] = ( stopHandlingTime.tv_sec - startHandlingTime.tv_sec );
       // printf("Teleisa me wra Handling time: %d\n",handlingTime[id-1]);
    	

	 rc = pthread_mutex_unlock(&lock);
	if (rc != 0) {
			printf("ERROR: return code from pthread_mutex_unlock is %d\n", rc);
			exit(-1);		
		}
	/*aparaitito gia na gnwrizei o pateras oti to thread termatise swsta,
	douleuei swsta kai an kanete return kapoia timi.*/
 	pthread_exit(id); //return
 }




int main(int argc, char** argv) {
	//dhmioyrgia pinaka pthread      
	pthread_t *threads;


    //treia arguments (to prwto orisma einai to onoma tou programmatos to deftero einai
    // to Ncust kai to trito einai to seed).
	if (argc != 3) {
		printf("ERROR: Provide two arguments.\n");
		return -1;
	}
      
     Ncust=atoi(argv[1]);
     seed=atoi(argv[2]); 
     printf("Num of costumers: %d kai to seed einai: %d\n", Ncust,seed);

	//xrisi malloc gia dimiourgia tou pinaka me megethos 250.
	//desmeuoume sizeof(int) * 250 sti mnimi.
	seatArray = (int *) malloc(sizeof(int) * 250);

	//elegxos an apetyxe i malloc
	if (seatArray == NULL) {
		printf("ERROR: Malloc failed not enough memory!\n");
		return -1;
	}

   	//arxikopoihsh pinaka 250 thesewn
	for (int i = 0; i < 250; i++) {
		seatArray[i] =0;
	}
       
	//desmeysh mnhmhs gia ta threads
        threads = malloc( Ncust* sizeof(pthread_t));
	if (threads == NULL) {
		printf("NOT ENOUGH MEMORY!\n");
		return -1;
	}

      
   	
       //xrisi malloc gia dimiourgia tou pinaka me megethos Ncust.
	//desmeuoume sizeof(int) * Ncust sti mnimi.
	threadId = (int *) malloc(sizeof(int) * Ncust);

	//elegxos an apetyxe i malloc
	if (threadId == NULL) {
		printf("ERROR: Malloc failed not enough memory!\n");
		return -1;
	}

	//xrisi malloc gia dimiourgia tou pinaka me megethos Ncust.
	//desmeuoume sizeof(int) * Ncust sti mnimi.
	standByTime = (int *) malloc(sizeof(int) * Ncust);

	//elegxos an apetyxe i malloc
	if (standByTime == NULL) {
		printf("ERROR: Malloc failed not enough memory!\n");
		return -1;
	}

	//xrisi malloc gia dimiourgia tou pinaka me megethos Ncust.
	//desmeuoume sizeof(int) * Ncust sti mnimi.
	handlingTime = (int *) malloc(sizeof(int) * Ncust);

	//elegxos an apetyxe i malloc
	if (handlingTime == NULL) {
		printf("ERROR: Malloc failed not enough memory!\n");
		return -1;
	}

        //arxikopoihsh lock
        pthread_mutex_init(&lock, NULL);

	//create threads
   	for(int threadCount = 0; threadCount < Ncust; threadCount++) {
    		//printf("Main: creating thread %d\n", threadCount);
		//arxikopoihsh threadId
		threadId[threadCount] = threadCount + 1;
		//printf("Main: creating thread %d with id: %d\n", threadCount,threadId[threadCount]);
		/*dimiourgia tou thread, n allakse prin hello!!!!*/
    	        rc=pthread_create(&threads[threadCount],NULL,customer,threadId[threadCount]); 
		//dhmioyrgia xronou anamonis/pelati kai xronoy kai xronou eksipiretisis/pelatis
		handlingTime[threadCount]=0;
		standByTime[threadCount]=0;
		
		//ksekinaw kai metraw thn wra anamonis
       		if( clock_gettime( CLOCK_REALTIME, &startStandBy) == -1 ) {
    		  perror( "clock gettime" );
     		 exit( EXIT_FAILURE );
   		 }

		//ksekinaw kai metraw thn wra eksyphrethshs
       		if( clock_gettime( CLOCK_REALTIME, &startHandlingTime) == -1 ) {
    		  perror( "clock gettime" );
     		 exit( EXIT_FAILURE );
   		 }
		/*elegxos oti to thread dimiourgithike swsta.*/
    		if (rc != 0) {
    			printf("ERROR: return code from pthread_create() is %d\n", rc);
       			exit(-1);
       		}
    	}

       /*Aparaitito gia na stamatisei to thread, an den to orisete yparxei
	periptwsi na teleiwsei o pateras prin ta threads kai ara na min exoume
	to epithymito apotelesma*/
	for (int threadCount = 0; threadCount < Ncust; threadCount++) {
		rc = pthread_join(threads[threadCount], NULL);
		
		if (rc != 0) {
			printf("ERROR: return code from pthread_join() is %d\n", rc);
			exit(-1);		
		}

		//printf("Main: Thread %d returned.\n", threadId[threadCount]);
	}
        printf("Main: Gurisan ola ta thread!\n");
	//emfanisi thesis kai pelati
	//an to counter einai 251 dhladg olos o pinakas gematos
	//tote to epanaferw 250 gia na einai entaksei to tupwma
	if(counterSeat==251){
		counterSeat=Nseat;
	}
	for(int i=0;i<counterSeat;i++){
		
		printf("Thesi %d- pelatis: %d \n",i+1,seatArray[i]);
	}
	
	double sumStandBy=0;
	double sumHandling=0;
	
	for(int i=0;i<Ncust;i++){
		sumStandBy=sumStandBy+ standByTime[i];
		sumHandling= sumHandling + handlingTime[i];
	}
	
	printf("Ta sinolika esoda apo tis pwliseis einai: %d$\n",account);
	printf("O mesos xronos anamonis twn pelatwn einai: %lf sec\n",sumStandBy/Ncust);
	printf("O mesos xronos eksipiretisis twn pelatwn einai: %lf sec\n",sumHandling/Ncust);




    //panta eletherwnoume ti mnimi pou exoume desmeusei.
    free(seatArray);
    free(threads);
    free(threadId);
    free(handlingTime);
    free(standByTime); 
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

return 0;

//todo:  percentages,
}


