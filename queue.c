#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

void enqueue(Queue* q, void* object){

	Node* temp = malloc(sizeof(struct Node));
	temp->data = object;
	temp->next = NULL;

	if(q->head == NULL){
		q->head = temp;
		q->tail = q->head;
	}else{
		q->tail->next = temp;
		q->tail = q->tail->next;
	
	}
		q->size += 1;

	return;
}


void* dequeue(Queue* q){
	
	Node* object;
	if(q->head == NULL)
		return NULL;
	else{
		object = q->head;
		q->head = q->head->next;
		q->size -= 1;
	}

	void* data = object->data;
	free(object);

	return data;
}

Queue* createQueue(){
	
	Queue* q = malloc(sizeof(Queue));
	pthread_mutex_init(&(q->mutex), NULL);	
	pthread_mutex_init(&(q->delay_lock), NULL);	
	pthread_cond_init(&q->condition, 0);
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;

	return q;
}


void destroyQueue(Queue* q){

	while(q->head != NULL){
		Node* temp = dequeue(q);
	}

	free(q);

	return;
}
