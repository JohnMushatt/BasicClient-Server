// Server side C/C++ program to demonstrate Socket programming
#include "server.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#define PORT 8080

/**
 * Take in a string and parse it, then compute equation's value
 * @param buffer Char buffer that holds the equation
 * @param len Length of buffer
 * @return char value of equation
 */
int getDigits(int num) {
	int number = num;
	int count = 0;
	while(number>0) {
		number = number / 10;
		count++;
	}
	return count;
}
int *computeEquation(char *buffer, int len) {
	int result;
	int termOne =0, termTwo=0, answer;
	char operation;
	for (int i = 0; i < len; i++) {
		//Get term one
		if (isdigit(buffer[i]) && termOne == 0) {
			termOne = buffer[i] - '0';
		}
		//Get 2nd term
		else if (isdigit(buffer[i])) {
			termTwo = buffer[i] - '0';
		}
		//Get operation
		else if (buffer[i] == '*' || buffer[i] == '+' || buffer[i] == '-'
				|| buffer[i] == '/') {
			operation = buffer[i];
		}

	}
	//Compute answer
	if (operation == '*') {
		result = termOne * termTwo;
	} else if (operation == '+') {
		result = termOne + termTwo;
	} else if (operation == '-') {
		result = termOne - termTwo;
	}
	return result;

}
int main(int argc, char const *argv[]) {
	printf("Server starting up!\n");
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	char *hello = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
			sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *) &address,
			(socklen_t*) &addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	valread = read(new_socket, buffer, 1024);
	printf("%s\n", buffer);
	send(new_socket, hello, strlen(hello), 0);
	printf("Hello message sent from server\n");
	char *eqBuffer[1024] = { 0 };
	int eqvalread = read(new_socket, eqBuffer, 1024);
	int val = computeEquation(eqBuffer, strlen(eqBuffer));
	//printf("Computed value: %d", val);

	char *response;
	int start = getDigits(val)-1;
	for(start; start >0;start--) {
		response[start] = val/10;
	}
	send(new_socket, response, strlen(response), 0);
	//printf("Send new value!");
	return 0;
}
