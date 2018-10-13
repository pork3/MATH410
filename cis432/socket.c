#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
//#include "config.h"
#include <getopt.h>

#define BUFFERSIZE 256

char* usage = "usage : -p PORT -i IPADDR ./program"

int main(int argc, char** argv){
	int port;
	int option;
	int return_code = 0;
	while (( option = getopt(argc, argv, "ip:")) != -1)
	{
		switch(option)
		{
			case 'i':
				/*going to assume any ip passed is valid*/
				printf("");
			break;
			case 'p':
				/*parse the port option*/
				fprintf("Port assigned from default  to %s", optarg)
			case '?':
				puts(usage);
				return 1;
		}
	}
	/*options parsed now set up the socket*/
	struct sockaddr_in serv_socket;

	int socket_fd;

	if (socket_fd = socket(AF_INET, SOCK_STREAM, 0) < 0 ){
		fprintf(stderr,"Error occured while creating socket, reutrned %d", errno);
	}
	char server_buffer[BUFFERSIZE];
	int i = 0;
	for(i ; i < BUFFERSIZE; i++)
		server_buffer[i] = 0;








	return 0;
}