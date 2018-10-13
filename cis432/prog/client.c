#include<stdlib.h>
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#define BUFFERSIZE 256
#define DEFAULT_PORT 8098

char* usage = "usage : -p PORT -i IPADDR ./program";

int main(int argc , char** argv){
    int port = 0;

  int option;
  int return_code = 0;
  while (( option = getopt(argc, argv, ":ip")) != -1)
  {
    switch(option)
    {
      case 'i':
        /*going to assume any ip passed is valid*/
        printf("i is %s", optarg);
        break;
      case 'p':
        /*parse the port option*/
        printf("Port assigned from default  to %s", optarg);
        port = atoi(optarg);
        break;
      case '?':
        puts(usage);
        return 1;
    }
  }

  if (port == 0)
      port = DEFAULT_PORT;

  int serverfd;
  printf("port is %d", port);
  struct sockaddr_in server;


  serverfd = socket(AF_INET, SOCK_STREAM, 0);

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(port);

  if( bind(serverfd, (struct sockaddr*)&server, sizeof(server)) < 0)
    puts("fjfjjfjfjf");

  return 0;
}