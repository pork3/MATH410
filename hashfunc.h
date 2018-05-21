#ifndef _INCLUDED_HASHFUNC_H_
#define _INCLUDED_HASHFUNC_H_

#define NUMBER_OF_FUNC 4 

long bit_hash(char* string, long n );

long hash_string(char* string, long n, int prime);

long hash_string_add(char*string , long n);

long djb2(char* string , long n);

typedef long(*generic_hash)();

extern generic_hash hash_array[NUMBER_OF_FUNC];


#endif