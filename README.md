This is a repository for a statistical analysis of hash functions done for MATH410 at the University of Oregon, it is a combination of C for the hashing/generating data, and R for interpreting the data.

## Introduction 

A [hashmap](https://en.wikipedia.org/wiki/Hash_table) is a way of storing items for fast access later, a simple hash function takes a key (usually a string) and executes a set of steps to turn the key into a number that stores this value into a table for later use.

Hashmaps are only as good as their hash function, as things like number of collisions (two keys sharing the say table number) can drastically slow this down.

I plan on examining common hashing techniques and running an analysis on 'ease of implementation' , storage needs, running time, and number of collisions

This test will be implemented in C for the hashtable and hash function, and R for the analysis of data. Although there are multiple ways to implement a hash table, the only table I will use for this experiment is [hashing with chaining](https://en.wikipedia.org/wiki/Hash_table#Separate_chaining_with_linked_lists)

I will be using a variety of machines to collect the data, one being a Raspberry pi with a ARM processor, the others being AMD or Intell processors running x86 instruction set. 

## Data Collection Process

*	I will be testing multiple hash functions
	*	[ELF HASH](https://en.wikipedia.org/wiki/PJW_hash_function)
	*	[String Summation](https://en.wikipedia.org/wiki/Hash_function#Hashing_variable-length_data)
	*	[String Product(testing against first 64 primes)](https://en.wikipedia.org/wiki/Hash_function#Hashing_variable-length_data)
	*	[Pearson Hash](https://en.wikipedia.org/wiki/Pearson_hashing)

*	Main data points
	*	Number of collisions
	*	Running time 



## Installation
	git clone https://github.com/pork3/MATH410.git
	make
	./maptest -filename

