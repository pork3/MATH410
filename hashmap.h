#ifndef _HASHMAP_H_
#define _HASHMAP_H_

typedef struct Hashmap Hashmap;

const Hashmap* create_hashmap(long capacity);

struct Hashmap{

	void* self;

	int (*insert)(const Hashmap* h, char* string);

	void (*destroy)(const Hashmap* h);
	
	int (*lookup)(const Hashmap* h, char* string);

	long (*size)(const Hashmap* h);

	int (*is_empty)(const Hashmap* h);

	int (*del)(const Hashmap* h, char* string);

	long (*collisions)(const Hashmap* h);


};

#endif