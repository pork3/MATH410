#include <stdlib.h>
#include <stdio.h>
#include "hashmap.h"
#include <errno.h>

#define BUFFERSIZE 4096

char* usage = "usage : <filename>";

int main (int argc, char** argv){

	FILE* f;
	size_t len = 0;
	long read;
	char* line = NULL;
	if(argc < 2){
		fprintf(stderr, "ERROR: %s\n",usage);
		return 1;
	}

	f = fopen(argv[1], "r");
	if (f == NULL){
		fprintf(stderr, "ERROR: %d\n", errno);
		return 1;
	}
	const Hashmap* h = create_hashmap(1024l);
	char *key;
	while ((read = getline(&line, &len, f))!= -1){
		printf("line was %s\n", line);
		sscanf("%s",line);
		key = line;
		h->insert(h, key);


	}

	free(line);
	long c = h->collisions(h);
	long size = h->size(h);
	printf("Map size was %ld\n", size);
	printf("Number of collisions: %ld\n", c);
	h->destroy(h);
	/*
    const Hashmap* h = create_hashmap(1l);
    printf("testing if map is empty %d\n",h->is_empty(h) );
	h->insert(h, "swag");
	h->insert(h, "fjfjf");
	int y = h->lookup(h, "swag");
	printf("we want this to be 0 %d\n",y );
	h->destroy(h);
	*/
	fclose(f);
	return 0;
}