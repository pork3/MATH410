#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
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



	//if( h != NULL){
	int w;
	long j;
	/*timekeeping */
	struct timespec stop,start;
	

	for(j = 0; j < 10; j++){
	/*want table size to be power of 2*/
	Hashmap* h = create_hashmap(1024L);
	clock_gettime(CLOCK_MONOTONIC_RAW , &start);
		while( (w = strline(buffer,&wrdin,wordbuffer)) >0){
			//printf("word is %s\n", wordbuffer);

			map_insert(h,wordbuffer, hash_array[2]);

		}
	clock_gettime(CLOCK_MONOTONIC_RAW , &stop);
	//printf("Collisions %ld\n",map_collisions(h));
	long delta_us = (stop.tv_sec - start.tv_sec) * 1000000 + (stop.tv_nsec - start.tv_nsec) / 1000;
	printf("ELF\t%ld\t%ld\t\t%ld\n",map_size(h),map_collisions(h),delta_us);
	
	map_destroy(h);
	wrdin = 0l;
	}


	fclose(f);
	free(buffer);
	return 0;
}
