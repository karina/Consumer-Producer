#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <signal.h>
#include "book.h"
#include "producers.h"
#include "consumers.h"

extern volatile int flag;
pthread_mutex_t tmp;

DatabaseEntry* getDatabaseEntry(char* buf){
	DatabaseEntry* ent;
	char* ptr;

	ent = malloc(sizeof(DatabaseEntry));
	
	ptr = strtok(buf, "\t");
	ent->name = malloc(sizeof(char)*(strlen(ptr)+1));
	strcpy(ent->name, ptr);
	
	ent->customerID = atoi(strtok(NULL, "\t"));

	ptr = strtok(NULL, "\t");
	ent->street = malloc(sizeof(char)*(strlen(ptr)+1));
	strcpy(ent->street, ptr);

	ptr = strtok(NULL, "\t");
	ent->state = malloc(sizeof(char)*(strlen(ptr)+1));
	strcpy(ent->state, ptr);

	ent->zip = atoi(strtok(NULL, "\t"));
	
	ent->creditLimit = atof(strtok(NULL, "\t"));

	return ent;
}

Database* getDatabase(char* filepath){
	FILE* f; 
	Database* db;
	char buf[1024];
	int i;
	
	db = malloc(sizeof(Database));

	/* count the number of entries */
	f = fopen(filepath, "r");
	
	if(f == NULL)
		return NULL;

	for(db->size=0; fgets(buf, 1024, f) != NULL; db->size++);
	fclose(f);
	db->entries = malloc(sizeof(DatabaseEntry*)*db->size);

	/* create each entry and load to array */
	f = fopen(filepath, "r");
	for(i=0; fgets(buf, 1024, f) != NULL; i++)
		db->entries[i] = getDatabaseEntry(buf);
	fclose(f);

	return db;
}

void freeDatabase(Database* db){

	int i;

	for(i = 0; i < db->size;i++){
		free(db->entries[i]->name);
		free(db->entries[i]->street);
		free(db->entries[i]->state);
		free(db->entries[i]);
		
	}

	free(db->entries);
	free(db);
	
	return;
}

int main(int argc, char** argv){
	Database* db;
	Queue* q;
	pthread_t* threads;
	pthread_t thread_cons, thread_end;
	int* irets;
	int iret_cons, iret_cons2;
	int numOfThreads, i;
	Data* data;
	unsigned int interval;
	Order* o;
	
	if(argc < 4){
		printf("Not correct number of arguments\n");
		return 0;
	}
	numOfThreads = atoi(argv[1]);
	threads = malloc(sizeof(pthread_t)*numOfThreads);
	irets = malloc(sizeof(int)*numOfThreads);

	data = malloc(sizeof(struct Data));
	/* mean interval for order creation by threads */
	interval = (unsigned int)atoi(argv[2]);
	data->interval = interval;

	/* generate database from file */
	db = getDatabase(argv[3]);
	if(db == NULL)
		return 0;

	data->database = db;
	/* build queue */
	q = createQueue(); 
	data->q = q;
	
	flag = 0;

	/* make me producer threads */
	for(i=0; i<numOfThreads; i++){
		irets[i] = pthread_create( &threads[i], NULL, producer, (void*)data);
		//sleep(1);
	}
 	


	/* make me consumer threads */
	
	iret_cons = pthread_create(&thread_cons, NULL, consumer, (void*)data);

	signal(SIGUSR1, signalHandler);
	signal(SIGUSR2, signalHandler);
	
	/* join all these */
	for(i=0; i<numOfThreads; i++)
		pthread_join( threads[i], NULL);
	pthread_join( thread_cons, NULL);


	/*if SIGUSR2 before consumer is called then free queue*/
	
	while(q->head != NULL){
		o = dequeue(q);
		free(o->book);
		free(o);
	}
	
	freeDatabase(db);
	free(q);
	free(data);
	free(irets);
	free(threads);
	return 0;
	
}
