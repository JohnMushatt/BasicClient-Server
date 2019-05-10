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
#include <sys/types.h>
#include <sys/stat.h>
#include "http_response.h"
#include <linux/limits.h>
#include "utilities.h"
#define PORT 8080
int tests_run = 0;

int server_fd, new_socket, valread;
struct sockaddr_in address;

char buffer[1024] = { 0 };



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


int main(int argc, char const *argv[]) {

	char *test = test_getDigits();
	if (test != 0) {
		printf("%s\n", test);
	} else {
		printf("ALL TESTS PASSED\n");
	}
	printf("Tests run: %d\n", tests_run);
	init_server();
	int addrlen = sizeof(address);
	while (1) {

		char buffer[30000] = { 0 };
		if (valread > 0) {
			valread = read(new_socket, buffer, 30000);

			char *request = malloc(sizeof(char) * 56);
			strncpy(request, buffer, 15);
			*(request + 16) = '\0';

			http_response_package resp;
			resp = buildResponse(buffer);
			printf("%s\n", buffer);
			close(new_socket);
		}
		else {
			printf("waiting for new connection\n");
			sleep(5);
		}
	}
	return 0;
}
