#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

struct Node{
	void* data;
	struct Node* next;
};
typedef struct Node Node;

struct Queue{
	pthread_mutex_t mutex;
	pthread_mutex_t delay_lock;
	pthread_cond_t condition;
	Node* head;
	Node* tail;
	int size;
};
typedef struct Queue Queue;

Queue* createQueue();

void enqueue(Queue* q, void* object);

void* dequeue(Queue* q);


#endif
