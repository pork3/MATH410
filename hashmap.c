#include <stdlib.h>
#include <string.h>

#include "hashmap.h"
#include "hashfunc.h"
#include <stdio.h>

/*define different declarations of 
max size for collision*/
#define MAX_INIT_CAPACITY 1024l
#define MIN_INIT_CAPACITY 32l

#define LOAD_FACTOR .75f

typedef struct node{
	char* key;
	struct node* next;
	long count;
} node;

typedef struct mapdata{
	/*how many items hm can store*/
	long capacity;
	node** storearray;
	/*current size */
	long size;
	/*threshold before resize*/
	long threshold;	
	/*int to choose which function*/
	int hash_func;
	/*prime number*/
	int prime;
} mapdata;

static node* create_node(char* string){
	node* n = (node*)malloc(sizeof(node));
	char* s = strdup(string);
	if (n!= NULL && s!=NULL){
		n->next = NULL;
		n->count = 1l;
		n->key = s;
	}
	return n;
}

/*search used by functions using primes*/
static node* map_search_p(mapdata* map, char* string, long* i,int prime){

	long index = (long)hash_array[0](string, map->capacity, prime);
	*i = index;
	node* n ;
	for (n = map->storearray[index]; n!= NULL; n=n->next)
		if(strcmp(string , n->key) == 0){
			return n;
		}
	return n;	
}

/*reference which hash function we want through the function pointer array HERE*/
static node* map_search_2(mapdata* map, char* string, long* i, int hashchoice){
	long index;
	if(hashchoice > 0){
		index = (long)hash_array[hashchoice](string , map->capacity);
	} else {
		return NULL;
	}
	*i = index;
	node* n ;
	for (n = map->storearray[index]; n!= NULL; n=n->next)
		if(strcmp(string , n->key) == 0){
			return n;
		}
	return n;	
}

static int map_lookup(const Hashmap* h, char* string){
	int status = 0;
	long s = 0;
	mapdata* m = (mapdata*)h->self;
	/*lookup node using map search*/
	node* n = map_search_2(m, string, &s,m->hash_func);
	if(n == NULL)
		return status;
	return status =(int)s;
}


static int map_remove(const Hashmap* h, char* string){
	int status = 0;
	mapdata* map = (mapdata*) h->self;
	long index;
	/*double check to ensure entry is in map*/
	node* n = map_search_2(map, string, &index,map->hash_func);
	if(n == NULL)
		return status;
	node* c;
	node* t;
	for(t = NULL, c = map->storearray[index]; c != n ; t = c ,c=c->next)
		;
		if( t == NULL)
			map->storearray[index] = n->next;
		else
			t->next = n->next;

	free(n->key);
	free(n);
	map->size --;
	status =1;
	return status;
}

/*helper function to resize hashmap when current items * loadfactor == size*/
static int map_resize(mapdata* map){
	//printf("in the resize\n");
	int status = 0;
	long s = 2 * map->capacity;
	long i;
	node** newarr = (node**)malloc(s * sizeof(node*));
	if ( newarr != NULL){
		for(i = 0; i < s;i++)
			newarr[i] = NULL;
		long index = 0;
		for(i = 0; i < map->capacity; i++){
			node* n = map->storearray[i];
			/*if there is item*/
			while(n != NULL){
			node* nxt = n->next;
			if(map->hash_func > 0){
				index = (long)hash_array[map->hash_func](n->key, s);
			} else {
				index = (long)hash_array[map->hash_func](n->key,s,map->prime);
			}
			n->next = newarr[index];
			newarr[index] = n;
			n = nxt;
		}
	}
	} else {
		return status;
	}
	free(map->storearray);
	status = 1;
	map->storearray = newarr;
	map->capacity = s;
	map->threshold = s * LOAD_FACTOR;
	return status;
}
/*function to insert, specifically dealing with prime numbers, makes automatic testing far easier*/
static int map_insert_prime(const Hashmap* h, char* string, int prime){
	int status = 0;
	mapdata* map = (mapdata*)h->self;
	if(map->size > map->threshold)
		(void)map_resize(map);

	long index;
	node* n = map_search_p(map, string, &index, prime);
	if( n != NULL){
		n->count++;
		status = 1;
	} else {
		n = create_node(string);
	}
	if(n != NULL){
		status = 1;
		n->next = map->storearray[index];
		map->storearray[index] = n;
		map->size++;
	}
	return status;
}


/*change to take function pointer array as last argument*/
static int map_insert(const Hashmap* h, char* string){
	int status = 0;
	mapdata* map = (mapdata*)h->self;
	if(map->size > map->threshold)
		(void)map_resize(map);

	/*choose hashfunction based */
	long index;
	node* n = map_search_2(map, string, &index,map->hash_func);
	if( n != NULL){
		n->count++;
		status = 1;
	} else {
		n = create_node(string);
	}
	if(n != NULL){
		status = 1;
		n->next = map->storearray[index];
		map->storearray[index] = n;
		map->size++;
	}
	return status;
}

static void map_destroy(const Hashmap* h){
	mapdata* map = (mapdata*)h->self;
	/* traverse ll, freeing all nodes and any memory with the nodes*/
	long i;
	for(i = 0; i < map->capacity; i++){
		node* n = map->storearray[i];
		/*if there is item*/
		while(n != NULL){
			node* nxt = n->next;
			free(n->key);
			free(n);
			n = nxt;
		}
	}
	free(map->storearray);
	free(map);
	free((void*)h);
}

static long map_size(const Hashmap* h){
	mapdata* map = (mapdata*)h->self;
	 return map->size;
}

static int map_is_empty(const Hashmap* h){
	mapdata* map = (mapdata*)h->self;
	 return map->size == 0;
}

static long map_coll(const Hashmap* h){
	mapdata* map = (mapdata*)h->self;
	long occupied = 0l;
	long i;
	for(i=0; i < map->capacity; i++){
		node* n = map->storearray[i];
		if( n != NULL)
			occupied++;
	}
	return map->size - occupied;
}


const Hashmap* create_hashmap(long capacity, int h_func, int prime){
	Hashmap* hmap = (Hashmap*)malloc(sizeof(Hashmap));
	if (hmap != NULL){
		/*set up private data members 
		and ensure capacity is of proper size*/
		if (capacity > MAX_INIT_CAPACITY)	
			capacity = MAX_INIT_CAPACITY;
		if (capacity < MIN_INIT_CAPACITY)
			capacity = MIN_INIT_CAPACITY;
		mapdata* map = (mapdata*)malloc(sizeof(mapdata));
		if(map != NULL){
			node** arr = (node**)malloc(capacity * sizeof(node*));
			if(arr != NULL){
				long i;/*initialize array to null*/
				for(i = 0; i < capacity; i++){
					arr[i] = NULL;
				}
				/*setting members of the hashmap data*/
				map->size = 0l;
				map->capacity = capacity;
				map->storearray = arr;
				map->threshold = LOAD_FACTOR * capacity;
				map->hash_func = h_func;
				map->prime = prime;

				hmap->self = map;
				hmap->insert = map_insert;
				hmap->destroy = map_destroy;
				hmap->lookup = map_lookup;
				hmap->size = map_size;
				hmap->is_empty = map_is_empty;
				hmap->del = map_remove;
				hmap->collisions = map_coll;
				hmap->insert_prime = map_insert_prime;
				 /*arbritrary default*/
				//hmap->prime_insert_test = map_insert_prime_test;
				/*hashmap test functions*/
				//hmap->prime_insert_test = map_insert_product;


			/*node array malloc failed*/
			} else {
				free(map); free(hmap);
				hmap = NULL; map = NULL;
			}			
		/* mapdata malloc failed*/
		} else {
			free(hmap);
			hmap = NULL;
		}
	}
	return hmap;
}

char* key(node* n){
	return n->key;
}

long count(node* n){
	return n->count;
}