#include <stdlib.h>
#include <stdio.h>
#include "hashmap.h"

#define BUFFERSIZE 4096

#define PRIME_SIZE 46

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

	const Hashmap* h = create_hashmap(1024l);
	//if( h != NULL){
	int w;
	long j;
	for(j = 0; j < 1; j++){
		while( w = strline(buffer,&wrdin,wordbuffer) >0){
			printf("word is %s\n", wordbuffer);
			
			if(h->insert(h, wordbuffer) < 1){
				fprintf(stderr, "ERROR adding word %s to map\n", wordbuffer);
			}

		}
		printf("----------SUMMARY ----------\n");
		printf("Prime: %d\n", PRIME_SIZE);
		printf("Size: %ld\n", h->size(h));
		printf("Collisions: %ld\n", h->collisions(h));
		printf("----------------------------\n");

		h->destroy(h);
	}
		


		
	

	
	fclose(f);
	free(buffer);
	return 0;
}



/*
int main (int argc, char** argv){

	FILE* f;
	size_t len = 0;
	long read;
	char* defaultfile = "./wordlists/dict";
	char* line = NULL;
	if(argc < 2){
		f = fopen("wordlists/dict", "r");
	} else {
		f = fopen(argv[1], "r");
		if (f == NULL){
			fprintf(stderr, "ERROR: Unable to open file");
			return 1;
		}
	}
	const Hashmap* h = create_hashmap(1024l);
	int i;

	for(i = 0; i < 1; i++){
		while ((read = getline(&line, &len, f))!= -1){
		printf("line was %s\n", line);
		sscanf("%s",line);
		h->insert(h, line);

	}

		free(line);
		long c = h->collisions(h);
		long size = h->size(h);
		printf("Statistic %d\n", i);
		printf("Map size was %ld\n", size);
		printf("Number of collisions: %ld\n\n", c);
		h->destroy(h);

	}

	fclose(f);
	return 0;
}
*/