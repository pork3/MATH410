 #include <stdlib.h>
#include <stdio.h>
#include "hashmap.h"

#define BUFFERSIZE 4096

#define PRIME_SIZE 46

int prime_array[46] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 
	43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 
	109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 
	181, 191, 193, 197, 199};

#define defaultfile "./wordlists/dict"


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
	int returncode = 0;
	FILE* f;
	if(argc < 2)
		f = fopen(defaultfile, "r");
	else
		f = fopen(argv[1], "r");
	if(f == NULL){
		fprintf(stderr, "ERROR opening file\n");
		returncode = 1;
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
		returncode =  1;
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
		returncode = 1;
		
	}

	char wordbuffer[1024];
	long wrdin = 0l;

	
	//if( h != NULL){
	int w;
	long j;
	for(j = 0; j < PRIME_SIZE; j++){
		const Hashmap* h = create_hashmap(1024l, 0, prime_array[j]);
		while( (w = strline(buffer,&wrdin,wordbuffer)) >0){
			//printf("word is %s\n", wordbuffer);
			
			if(h->insert_prime(h, wordbuffer, prime_array[j]) < 1){
				fprintf(stderr, "ERROR adding word %s to map\n", wordbuffer);
			}

		}
		printf("----------SUMMARY ----------\n");
		printf("Prime: %d\n", prime_array[j]);
		printf("Size: %ld\n", h->size(h));
		printf("Collisions: %ld\n", h->collisions(h));
		printf("----------------------------\n");

		h->destroy(h);
		wrdin = 0l;
	}
		


		
	

	
	fclose(f);
	free(buffer);
	return 0;
}
