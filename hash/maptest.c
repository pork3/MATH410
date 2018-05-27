#include <stdlib.h>
#include <stdio.h>
#include "hash.h"
#include "hashfxn.h"

#define NUM_FXN 4
#define BUFFERSIZE 4096
#define defaultfile "../wordlists/dict"

typedef long(*generic_hash)();

generic_hash hash_array[NUM_FXN] = {
		(generic_hash)bit_hash,
		(generic_hash)hash_string_mult,
		(generic_hash)hash_string_add,
		(generic_hash)djb2
};


long strline(char buffer[], long* pos, char word[]){
	char c;
	long i = 0;
	while( (c = (word[i] = buffer[*pos]))!= '\0'){
		word[i] = buffer[*pos];
		(*pos)++;
		i++;
		if(c == '\n'){
			word[i] = '\0';
			return i;
		}
		
	}
	return -1;
}


int main(int argc , char** argv){
	FILE* f;
	if(argc < 2)
		f = fopen(defaultfile, "r");
	else
		f = fopen(argv[1], "r");
	if(f == NULL){
		fprintf(stderr, "ERROR opening file\n");
		//returncode = 1;
	}
	/*data members to for file read*/
	long size = 0;
	char* buffer = NULL;

	/*obtain size of file*/
	if (fseek(f, size, SEEK_END) == 0){
		size = ftell(f);
		rewind(f); /*go to beginning*/
	} else {
		fprintf(stderr, "ERROR reading file...\n");
		//returncode =  1;
	}
	buffer = (char*)malloc((size + 1)* sizeof(char));
	if( buffer != NULL){
		long i;
		for(i = 0; i < size; i++){ 
			buffer[i] = NULL;
		}
		buffer[size] = '\0';		
	}

	/*read file into buffer*/
	if(fread(buffer, size, 1, f) != 1){
		fprintf(stderr, "ERROR reading file\n");
		//returncode = 1;
		
	}

	char wordbuffer[1024];
	long wrdin = 0l;

	/*want table size to be power of 2*/
	Hashmap* h = create_hashmap(1024L);

	//if( h != NULL){
	int w;
	long j;
	for(j = 0; j < 1; j++){
		while( (w = strline(buffer,&wrdin,wordbuffer)) >0){
			printf("word is %s\n", wordbuffer);
			map_insert(h,wordbuffer, hash_array[1]);

		}
	}
		


	printf("Size is %d\n",map_size(h));		
	printf("Collisions are : %d\n",map_collisions(h));

	
	fclose(f);
	free(buffer);

	map_destroy(h);
	return 0;
}
