#ifndef PRODUCERS_H
#define PRODUCERS_H

#include "queue.h"

struct Order{

	float price;
	int customerID;
	char* book;
};
typedef struct Order Order;

struct Data{
	unsigned int interval;
	Queue* q;
	void* database;
};
typedef struct Data Data;

volatile int flag;

void* producer(void* data);

#endif
