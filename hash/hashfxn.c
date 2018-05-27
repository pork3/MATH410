#include "hashfxn.h"

/*ELF hash*/
unsigned long bit_hash(char* string){
	unsigned long index = 0;
	while(*string != '\0'){
		index = (index << 4) + *(string++);
		unsigned long g = index & 0xF0000000L;
		if(g != 0)
			index ^= g >> 24;
		index &= ~ g;
	}
	return index;
}

/*Multiply String*/
unsigned long hash_string_mult(char* string, int prime){
	unsigned long index = 0;
	unsigned long i;
	for( i=0; string[i] != '\0'; i++){
		index += prime * index + (unsigned long)string[i];
	}
	return index;
}

/*Additive Hash*/
unsigned long hash_string_add(char* string){
	unsigned long index = 0;
	unsigned long i;
	for(i=0; string[i]!='\0';i++){
		index += (unsigned long)string[i];
	}
	return index;
}

/*djb2 modified */
unsigned long djb2(char* string){
	unsigned long hash = 5381;
	char c;
	while(c =*string++){
		hash = ((hash<< 5) + hash) + c;
	}
	return hash;
}