// Server side C/C++ program to demonstrate Socket programming
#include "server.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include "minunit.h"
#define PORT 8080
int tests_run = 0;

int server_fd, new_socket, valread;
struct sockaddr_in address;

char buffer[1024] = { 0 };

int getDigits(int num) {
	int number = num;
	int count = 0;
	while (number > 0) {
		number = number / 10;
		count++;
	}
	return count;
}

static char *test_getDigits1() {
	char *err_msg = "Wrong number of digits";
	int num1 = 100;
	mu_assert(err_msg, getDigits(num1) == 3);
	return 0;
}
static char * test_getDigits2() {
	char *err_msg = "Wrong number of digits";
	int num1 = 20;
	mu_assert(err_msg, getDigits(num1) == 2);
	return 0;
}
static char * test_getDigits() {
	mu_run_test(test_getDigits1);
	mu_run_test(test_getDigits2);
	return 0;
}
/**
 * Take in a string and parse it, then compute equation's value
 * @param buffer Char buffer that holds the equation
 * @param len Length of buffer
 * @return char value of equation
 */
int computeEquation(char *buffer, int len) {
	int result;
	int termOne = 0, termTwo = 0, answer;
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
void init_server() {
	printf("Server starting up!\n");

	int opt = 1;
	int addrlen = sizeof(address);

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
	printf("Server has connected!\n\n");
}
static char * test_servermsg() {


}
int main(int argc, char const *argv[]) {

	char *test = test_getDigits();
	if (test != 0) {
		printf("%s\n", test);
	} else {
		printf("ALL TESTS PASSED\n");
	}
	init_server();
	printf("Tests run: %d\n", tests_run);
	valread = read(new_socket, buffer, 1024);
	printf("Message received: %s\n", buffer);
	char *sever_message = "This is from the server";
	send(new_socket, sever_message, strlen(sever_message), 0);
	printf("Server message sent!\n");
	memset(buffer,0,1024);

	int eqvalread = read(new_socket, buffer, 1024);
	int val = computeEquation(buffer, strlen(buffer));
	int digits = getDigits(val);

	char *response = (char *) malloc((sizeof(char) * getDigits(val)) + 1);
	for (int i = digits - 1; i > -1; i--) {
		int c = val % 10 + '0';
		*(response + i) = c;
		val = val / 10;
	}
	*(response + getDigits(val) - 1) = '\0';
	send(new_socket, response, strlen(response), 0);
	printf("Serving stopping!\n");

	return 0;
}
