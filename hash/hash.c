#include <stdlib.h>
#include <string.h>

#include "hash.h"
//#include "hashfunc.h"

#define LOAD_FACTOR .75f
#define MIN_CAPACITY 32L
#define MAX_CAPACITY 0x100000000 /*2^32*/

typedef struct Node{
	char* key;
	struct Node* next;
	int count;
} Node;

typedef struct Hashmap{
	unsigned long capacity;
	unsigned long size;
	float threshold;
	Node** buckets;
} Hashmap;

static Node* create_node(char* string){
	Node* n = (Node*)malloc(sizeof(Node));
	char* s = strdup(string);
	if (n!= NULL && s!=NULL){
		n->next = NULL;
		n->count = 1l;
		n->key = s;
	}
	return n;
}

static Node* map_search(Hashmap* h,char* string , unsigned long* i, unsigned long (*hashfxn)()){
	unsigned long index;
	if (hashfxn != NULL){
		index = (unsigned long)hashfxn(string) % h->capacity;
	} else {
		return NULL;
	}
	*i = index;
	Node* n;
	for(n = h->buckets[index]; n!= NULL; n=n->next){
		if(strcmp(string , n->key)== 0){
			return n;
		}
	}
	return n;
}

static int map_resize(Hashmap* h , unsigned long (*hashfxn)()){
	int status = 0;
	unsigned long ns = 2 * h->size;
	unsigned long i,index;
	Node** newarr = (Node**)malloc(ns*sizeof(Node*));
	if (newarr != NULL){
		for(i = 0; i < ns; i++){
			newarr[i] = NULL;
		}
	index = 0;
	Node* nxt;
	for(i = 0; i < h->capacity; i++){
		Node* n = h->buckets[i];
		while(n != NULL){
			nxt = n->next;
			index = (unsigned long)hashfxn(n->key) % h->capacity;
			n->next = newarr[index];
			newarr[index] = n;
			n = nxt;
			}
		}
	} else {
		return status;
	}
	free(h->buckets);
	status = 1;
	h->buckets = newarr;
	h->capacity = ns;
	h->threshold = ns * LOAD_FACTOR;
	return status;
}

int map_insert(Hashmap* h , char* string, unsigned long (*hashfxn)()){
	int status =0;
	if(h->size > h->threshold)
		/*change to not be void and take hash fxn*/
		(void)map_resize(h , hashfxn);
	unsigned long index;
	Node* n = map_search(h,string,&index, hashfxn);

	if (n != NULL){
		n->count++;
		status = 1;
	} else {
		n =  create_node(string);
		if(n != NULL){
			status = 1;
			n->next = h->buckets[index];
			h->buckets[index] = n;
			h->size++;
		}
	}
	return status;
}

void map_destroy(Hashmap* h){
	unsigned long i;
	for(i =0; i < h->capacity; i++){
		Node* n = h->buckets[i];
		while(n != NULL){
			Node* nxt = n->next;
			free(n->key);
			free(n);
			n=nxt;
		}
	}
	free(h->buckets);
	free(h);
}

unsigned long map_size(Hashmap* h){
	return h->size;
}

int map_is_empty(Hashmap* h){
	return h->size == 0;
}

static unsigned long next_pow_2(unsigned long x){
	/*want table size to be power of 2
	calculate next power of 2 on capacity*/
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return ++x;
}

unsigned long map_collisions(Hashmap* h){
	unsigned long occupied = 0L;
	unsigned long i;
	for(i = 0; i < h->capacity; i++){
		Node* n = h->buckets[i];
		if(n != NULL)
			occupied++;
	}
	return h->size - occupied;
}

Hashmap* create_hashmap(unsigned long capacity){

	Hashmap* h = (Hashmap*)malloc(sizeof(Hashmap));
	if(h != NULL){
		if (capacity < MIN_CAPACITY)
			capacity = MIN_CAPACITY;
		if (capacity > MAX_CAPACITY)
			capacity = MAX_CAPACITY;
		capacity = next_pow_2(capacity);
		/*create buckets to store nodes*/
		Node** arr = (Node**)malloc( capacity * sizeof(Node*));
		if(arr != NULL){
			unsigned long i;
			for(i = 0; i < capacity; i++){
				arr[i] = NULL;
			}	
			h->size = 0L;
			h->capacity = capacity;
			h->buckets = arr;
			h->threshold = LOAD_FACTOR * capacity;
		} else {
			free(h);
			h = NULL;
		}
	} else {
		return h;
	}
	return h;
}