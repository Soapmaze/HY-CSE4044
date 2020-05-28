#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#define PORT 9090

char *secret = "You find a Secret Message!\n";
unsigned int target = 0x11223344;

void myprintf(char *msg)
{
	// Copy the ebp value into framep, and print out
	uintptr_t framep;
	asm("movl %%ebp, %0" : "=r"(framep));
	printf("The ebp value inside myprintf() is: 0x%.8x\n", framep);
	printf("The address of the 'msg' argument: 0x%.8x\n", (unsigned)&msg);
	printf(msg);
	printf("The value of the 'target' variable (after): 0x%.8x\n", target);
}

void helper()
{
	printf("The address of the secret: 0x%.8x\n", (unsigned) secret);
	printf("The address fo the 'target' variable: 0x%.8x\n", (unsigned) &target);
	printf("The value of the 'target' variable (before): 0x%.8x\n",target);
}

void main()
{
	struct sockaddr_in server;
	struct sockaddr_in client;
	int clientLen;
	char buf[1500];

	printf("The address of the input array: 0x%.8x\n", (unsigned) buf);

	helper();

	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	memset((char *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);

	if(bind(sock, (struct sockaddr *) &server, sizeof(server)) < 0)
		perror("ERROR on binding");

	while(1)
	{
		bzero(buf, 1500);
		recvfrom(sock, buf, 1500-1, 0, (struct sockaddr *) &client, &clientLen);
		myprintf(buf);
	}
	close(sock);
}
