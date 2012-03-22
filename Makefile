COMPILER = gcc
CCFLAGS = -g

all: book

book: book.o queue.o consumers.o producers.o
	$(COMPILER) $(CCFLAGS) -o book book.o queue.o consumers.o producers.o -lm -lpthread
consumers.o: consumers.c consumers.h producers.h book.h queue.h
	$(COMPILER) $(CCFLAGS) -c consumers.c
producers.o: producers.c producers.h book.h queue.h
	$(COMPILER) $(CCFLAGS) -c producers.c
book.o: book.c book.h queue.h
	$(COMPILER) $(CCFLAGS) -c book.c
queue.o: queue.c queue.h
	$(COMPILER) $(CCFLAGS) -c queue.c

clean:
	rm -f book
	rm -f *.o
