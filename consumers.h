#ifndef CONSUMERS_H
#define CONSUMERS_H

#include "queue.h"
#include "book.h"


void signalHandler(int signo);
void* consumer(void* data);

#endif
