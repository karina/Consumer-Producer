#ifndef BOOK_H
#define BOOK_H


struct DatabaseEntry{

	char* name;
	int customerID;
	char* street;
	char* state;
	int zip;
	float creditLimit;
};
typedef struct DatabaseEntry DatabaseEntry;

struct Database{
	int size;
	struct DatabaseEntry** entries;
};
typedef struct Database Database;

void freeDatabase(Database* db);
#endif
