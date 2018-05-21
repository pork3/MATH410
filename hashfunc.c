#include "hashfunc.h"

#define NUMBER_OF_FUNC 4 

typedef long(*generic_hash)();

generic_hash hash_array[NUMBER_OF_FUNC] = {

	(generic_hash)bit_hash, 
	(generic_hash)hash_string,
	(generic_hash)hash_string_add,
	(generic_hash)djb2

};

/*ELF hash*/
long bit_hash(char* string, long n ){
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

/*mutliply string*/
long hash_string(char* string, long n, int prime){
	unsigned long index = 0;
	long i;
	/*check hash function*/
	for(i=0; string[i] != '\0'; i++){
		index = prime * index + (long)string[i];
	}
	return index % n;

}

/*string addition*/
long hash_string_add(char*string , long n){
	unsigned long index =0;
	long i;
	for(i=0; string[i] !='\0'; i++){
		index += (long)string[i];
	}
	return index % n;
}

/*djb2 modified*/
long djb2(char* string , long n){
	unsigned long hash = 5381;
	char c;
	while( c =*string++){
		hash = ((hash << 5) + hash) + c;
	}
	return hash % n;
} 