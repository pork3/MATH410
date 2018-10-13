#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFERSIZE 256
#define DEFAULT_PORT 8888

char* usage = "usage : -p PORT -i IPADDR ./program";

void message(*struct sockaddr_in socket, char* message, int filedesc){

	int len = strlen(message) + 1;
	char* buffer = (char*)malloc(len * sizeof(char))
	while (i != NULL){
		buffer[i] = message[i];
		i++
	} buffer[len] = '\0';

	if (writeflag = write(filedesc, message, len) < 0){
		int write_err = errno;
		fprintf(stderr, "Error writing to client %d\n",write_err);
	}
	free(buffer);
	return;
}


int main(int argc, char** argv){
	unsigned short port = 0;
	int option;
	int return_code = 0;
	while (( option = getopt(argc, argv, ":ip")) != -1)
	{
		switch(option)
		{
			case 'i':
				/*going to assume any ip passed is valid*/
				printf("i is %s", optarg);
				port = atoi(optarg);
				break;
			case 'p':
				/*parse the port option*/
				printf("Port assigned from default  to %s", optarg);
				break;
			case '?':
				puts(usage);
				return 1;
		}
	}
	if(port == 0){
		port = DEFAULT_PORT;
	}

	/*options parsed now set up the socket*/
	struct sockaddr_in serv_socket;

	int server_fd;

	if ( (server_fd = socket(AF_INET, SOCK_STREAM, 0) )< 0 ){
		int sock_er = errno;
		fprintf(stderr,"Error occured while creating socket, reutrned %d\n", sock_er);
		strerror(sock_er);
		return 1;
	}
	
	serv_socket.sin_family = AF_INET;
	serv_socket.sin_addr.s_addr = INADDR_ANY; /*these two make it use whatever available*/
	serv_socket.sin_port = htons(port);
	memset(&(serv_socket.sin_zero), '\0', 8);

	if ( bind(server_fd, (struct sockaddr *) &serv_socket, sizeof(serv_socket)) == -1){
		int bind_er = errno;
		fprintf(stderr, "Error occured while binding socket , returned %d\n", bind_er );
		return 1;
	}

	if ( listen(server_fd, 5) < 0){
		int listen_er = errno;
		fprintf(stderr,"Error on socket listen , returned %d\n", listen_er);
		return 1;
	}

	int clinetfd;
	struct sockaddr_in cli_socket;
	int clientlen = sizeof(cli_socket);

	if ( clinetfd = accept(server_fd ,(struct sockaddr*) &cli_socket, &clientlen) < 0 ){
		int accept_er = errno;
		fprintf(stderr,"Error, unable to accept client, returned %d\n", accept_er);
	} 

	char server_buffer[BUFFERSIZE];
	int i = 0;
	for(i ; i < BUFFERSIZE; i++)
		server_buffer[i] = 0;

	int writeflag;
	if (writeflag = write(clinetfd, "Test\n", 6) < 0){
		int write_err = errno;
		fprintf(stderr, "Error writing to client %d\n",write_err);
	}

	
	return 0;
}