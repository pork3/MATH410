#ifndef _INCLUDED_HASHFUNC_H_
#define _INCLUDED_HASHFUNC_H_


long PearsonHash(char* string, long n);

long bit_hash(char* string, long n );

long hash_string(char* string, long n, int prime);

long hash_string_add(char*string , long n);

long djb2(char* string , long n);

#endif