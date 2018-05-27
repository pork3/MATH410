#ifndef _INCLUDED_HASH_H_
#define _INCLUDED_HASH_H_

typedef struct Hashmap Hashmap;

int map_insert(Hashmap* h , char* string, unsigned long (*hashfxn)());

void map_destroy(Hashmap* h);

unsigned long map_size(Hashmap* h);

int map_is_empty(Hashmap* h);

unsigned long map_collisions(Hashmap* h);

Hashmap* create_hashmap(unsigned long capacity);

#endif