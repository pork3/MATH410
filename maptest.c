#include <stdlib.h>
#include <stdio.h>
#include "hashmap.h"

#define BUFFERSIZE 4096

#define PRIME_SIZE 46


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