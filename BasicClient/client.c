#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

int main(int argc, char const **argv) {
	struct sockaddr_in address;
	int sock = 0;
	int valread;
	struct sockaddr_in server_address;
	char *msg = "Hello from client";
	char buffer[1024] = { 0 };
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;

	}
	memset(&server_address, '0', sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *) &server_address,
			sizeof(server_address)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	send(sock,msg,strlen(msg),0);
	printf("Msg sent from client\n");
	valread = read(sock,buffer,1024);
	char *equation = "4 * 3";

	printf("Msg from server: %s\n",buffer);
	send(sock,equation,strlen(equation),0);

	printf("Equation sent from client!");
	valread = read(sock,buffer,1024);
	printf("Server calculated value: %s\n",buffer);
	return 0;
}
