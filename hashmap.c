#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

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
} mapdata;

static unsigned char PseudoRandomHash[256] = {
     1,   87,   49,  12, 176, 178,  102, 166, 121, 193,   6,  84, 249, 230,  44,  163, 
     14, 197,  213, 181, 161,  85,  218,  80,  64, 239,  24, 226, 236, 142,  38,  200, 
    110, 177,  104, 103, 141, 253,  255,  50,  77, 101,  81,  18,  45,  96,  31,  222, 
     25, 107,  190,  70,  86, 237,  240,  34,  72, 242,  20, 214, 244, 227, 149,  235, 
     97, 234,   57,  22,  60, 250,   82,  175, 208,   5, 127, 199, 111,  62, 135,  248, 
    174, 169,  211,  58,  66, 154,  106, 195, 245, 171,  17, 187, 182, 179,   0,  243, 
    132,  56,  148,  75, 128, 133,  158, 100, 130, 126,  91,  13, 153, 246, 216,  219,
    119,  68,  223,  78,  83,  88,  201,  99, 122,  11,  92,  32, 136, 114,  52,   10, 
    138,  30,   48, 183, 156,  35,   61,  26, 143,  74, 251,  94, 129, 162,  63,  152, 
    170,   7,  115, 167, 241, 206,    3, 150,  55,  59, 151, 220,  90,  53,  23,  131, 
    125, 173,   15, 238,  79,  95,   89,  16, 105, 137, 225, 224, 217, 160,  37,  123, 
    118,  73,    2, 157,  46, 116,    9, 145, 134, 228, 207, 212, 202, 215,  69,  229, 
     27, 188,   67, 124, 168, 252,   42,   4,  29, 108,  21, 247,  19, 205,  39,  203, 
    233,  40,  186, 147, 198, 192,  155,  33, 164, 191,  98, 204, 165, 180, 117,   76, 
    140,  36,  210, 172,  41,  54,  159,   8, 185, 232, 113, 196, 231,  47, 146,  120, 
     51,  65,   28, 144, 254, 221,   93, 189, 194, 139, 112,  43,  71, 109, 184,  209}; 

int prime_array[46] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 
	43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 
	109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 
	181, 191, 193, 197, 199};

#define shift 31L

/*Pearson hash*/
static long PearsonHash(char* string){
	long i;
	long strsize = strlen(string);
	char* h;
	long index;
	h = (char*)malloc(strsize * sizeof(char));
	for(i =0; i < strsize; i++)
		h[i] = 0;
	for(i = 1; i < strsize;i++){
		h[i] = PseudoRandomHash[(h[i-1] ^ string[i])& 0xFF];
		index = h[i];
	}
	free(h);
	return index;
}

/*ELF hash*/
static long bit_hash(char* string, long n ){
	long index = 0;
	while(*string !='\0'){
		index = (index << 4) + *(string++);
		long g = index & 0xF0000000l;
		if( g != 0 )
			index ^= g >>24;
		index &= ~g;
	}
	return index;
}


static long hash_string(char* string, long n){
	unsigned long index = 0;
	//printf("n is %ld", n);
	long i;
	/*check hash function*/
	for(i=0; string[i] != '\0'; i++){
		index = shift * index + (long)string[i];
	}
	//printf("Index is %ld\n",index%n);
	return index % n;

}

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

static node* map_search_2(mapdata* map, char* string, long* i){
	long index = hash_string(string, map->capacity);
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
	/*lookup node using map search*/
	node* n = map_search_2((mapdata*)h->self, string, &s);
	if(n == NULL)
		return status;
	return status =(int)s;
}


static int map_remove(const Hashmap* h, char* string){
	int status = 0;
	mapdata* map = (mapdata*) h->self;
	long index;
	/*double check to ensure entry is in map*/
	node* n = map_search_2(map, string, &index);
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
			index  = hash_string(n->key , s);
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

/*change to take function pointer array as last argument*/
static int map_insert(const Hashmap* h, char* string){
	int status = 0;
	mapdata* map = (mapdata*)h->self;
	if(map->size > map->threshold)
		(void)map_resize(map);

	long index;
	node* n = map_search_2(map, string, &index);
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
	long i = 0;
	for(i; i < map->capacity; i++){
		node* n = map->storearray[i];
		if( n != NULL)
			occupied++;
	}
	return map->size - occupied;
}


const Hashmap* create_hashmap(long capacity){
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

				hmap->self = map;
				hmap->insert = map_insert;
				hmap->destroy = map_destroy;
				hmap->lookup = map_lookup;
				hmap->size = map_size;
				hmap->is_empty = map_is_empty;
				hmap->del = map_remove;
				hmap->collisions = map_coll;
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