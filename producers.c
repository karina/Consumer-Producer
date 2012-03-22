#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "book.h"
#include "producers.h"

extern volatile int flag;

Order* generateOrder(){
	char* adjectives[] = {	"Huge", "Dark", "Melodic", "Beautiful", "Ugly", 
							"Old-fashioned", "Bitter", "Ignorant", "Apple Obsessed", "Angry", 
							"Rubbish", "Bulky", "Narrow", "Wicked", "Attractive"
							}; 
	char* verbs[] = {	" Running", " Fighting", " Playing", " Spanking", " Jumping", 
						" Competing", " Struggling", " Rebelling", " Stalking", " Yearning", 
						" Hacking", " Programming", " Tranporting", " Trainspotting", " Performing"
						};
	char* nouns[] = {	" Dog", " Cat", " Man", " Woman", " Horse", 
						" Tree", " Chair", " Painting", " Ghost", " Boxer", 
						" Programmer", " Language", " Firefighter", " Student", " Professor"
						};
	
	char* ptr;

	struct timeval t;

	gettimeofday(&t, NULL);

	/* randomly generate everything */
	srand48(pthread_self() * t.tv_sec + t.tv_usec);
	int i;
	int rand1 = drand48()*15;
	int rand2 = drand48()*15;
	int rand3 = drand48()*15;
	int rand4 = drand48()*50+1;
	float rand5 = drand48()*200;
	float rand6 = drand48()*100;

	Order* o = malloc(sizeof(Order));
	o->book = malloc(sizeof(char)*(strlen(adjectives[rand1])+strlen(verbs[rand2])+strlen(nouns[rand3])+1));
	i=0;

	/*** POINTER VOODOO MAGIC ***/
	for(ptr = adjectives[rand1]; *ptr != '\0'; ptr++){
		//printf("%c", *ptr);
		o->book[i] = *ptr;
		i++;
	
	}
	for(ptr = verbs[rand2]; *ptr != '\0'; ptr++){
		//printf("%c", *ptr);
		o->book[i] = *ptr;
		i++;
	}
	for(ptr = nouns[rand3]; *ptr != '\0'; ptr++){
		//printf("%c", *ptr);	
		o->book[i] = *ptr;
		i++;
	}
	o->book[i] = '\0';
	/*** END POINTER VOODOO MAGIC ***/

	o->customerID = rand4;
	o->price = rand5 + rand6/100;
	
	return o;
}

void* producer(void* data){
	float delay;
	Order* o;
	int i;
	long int remainder_nano;
	Queue* q;
	double interval;
	struct timeval t;
	Data* d = (Data*)data;
	
	q = d->q;
	interval = d->interval;
	int j = 0;

	while(flag != 1){
		
		pthread_mutex_lock(&(q->delay_lock));
		gettimeofday(&t, NULL);
		srand48((int)pthread_self() * t.tv_sec + t.tv_usec);
		/* poisson random interval delay */
		delay = -interval*log(1.0-drand48());
		pthread_mutex_unlock(&(q->delay_lock));
		
//		remainder_nano = (long int) ((delay - (long int)delay) * 1000000); //Always less than a billion

//		sleep(delay); //converting seconds to nanoseconds
//		printf("remainder = %ld, whole = %d", remainder_nano, (unsigned int)delay);
//		usleep(remainder_nano);

		usleep(delay*1000000);
		
		pthread_mutex_lock(&(q->mutex));
		
		o = generateOrder();
		enqueue(q, o);
		printf("%d,%s,%d,%f\n",t.tv_sec,o->book,o->customerID,o->price);//prints out timestamp
		j++;
		pthread_mutex_unlock(&(q->mutex));
	}
	return;
}
