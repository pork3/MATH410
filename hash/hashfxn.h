#ifndef _INCLUDED_HASHFXN_H_
#define _INCLUDED_HASHFXN_H_

/*list of hash functions to be used by the caller*/

unsigned long bit_hash(char* string);

unsigned long hash_string_mult(char* string, int prime);

unsigned long hash_string_add(char* string);

unsigned long djb2(char* string);

#endif