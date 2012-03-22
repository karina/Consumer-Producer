#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <semaphore.h>
#include "consumers.h"
#include "producers.h"

sem_t semMutex;
sem_t semEnd;

extern volatile int flag;

void signalHandler(int signo){
	if(signo == SIGUSR1){
		sem_post(&semMutex);
	}
	if(signo == SIGUSR2){
		flag = 1;
		sem_post(&semMutex);
		return;
	}

}

//returns 0 if they dont have enough credit to purchase
//else it returns 1 when it debits the limit
int creditCheck(int customerID, float price, Database* database) 
{
	if(database->entries[customerID]->creditLimit < price)
		return 0;
	else
		database->entries[customerID]->creditLimit -= price;

	return 1;
}

void* consumer(void* data){
	Data* d = (Data*)data;
	struct timeval time;
	Order* order;
	float sum = 0;
	Database* database;
	sem_init(&semMutex,0,0);
	int iret_cons;
	pthread_t thread_cons;
	
	database = (Database*)d->database;
	/* wait until sigusr1 is fired */
	sem_wait(&semMutex);
	if(flag == 1)
		return;


	gettimeofday(&time,NULL);
	printf("T,%d\n",time.tv_sec);//prints out timestamp

	pthread_mutex_lock(&d->q->mutex);
	while(d->q->head != NULL){

		order = (Order*)dequeue(d->q);

		if(creditCheck(order->customerID,order->price, database) != 0){
			printf("V,%s,\t%f,\t%s,\t%s,\t%s,\t%d\n", order->book, order->price, database->entries[order->customerID]->name, database->entries[order->customerID]->street,  database->entries[order->customerID]->state,  database->entries[order->customerID]->zip);
			sum += order->price;
		}else
			printf("R, %s,%s,%f\n",order->book, database->entries[order->customerID]->name, database->entries[order->customerID]->creditLimit);
		
		free(order->book);
		free(order);
	}

	pthread_mutex_unlock(&d->q->mutex);
	printf("S,%f\n",sum);
	
	if(flag != 1)
		iret_cons = pthread_create(&thread_cons, NULL, consumer,(void*)data);
	return;
}

