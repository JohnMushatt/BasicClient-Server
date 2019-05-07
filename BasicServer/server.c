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
char *int2char(int num) {
	int dig = getDigits(num);
	char *buffer = malloc(sizeof(char) * (dig + 1));
	for (int i = dig - 1; i > -1; i--) {
		*(buffer + i) = num % 10 + '0';
		num = num / 10;
	}
	*(buffer + dig + 1) = '\0';
	return buffer;
}
char *getContent_Type(const char *buffer) {
	char *content_type;
	int len = strlen(buffer);
	for (int i = 0; i < len; i++) {
		char c = *(buffer + i);
	}
	return content_type;
}
/**
 * Returns the absolute file path from the http request
 *  /index.html
 */
char *getFileBytes(const char *buffer) {
	//Allocate enough memory to hold the buffer
	char *temp = malloc(sizeof(char) * (strlen(buffer)));
	//Copy the buffer over to temp
	memcpy(temp, buffer, strlen(buffer));
	//Declare relative path
	char *relativePath;

	//Gets the 1st occurrence of " "
	char *r = strstr(temp, "/") + 1;
	//Get the next whitespace aka the end of the file request
	int index = strcspn(r, " ");
	//Allocatte memory for relative path
	relativePath = malloc(sizeof(char) * (index + 1));
	//Copy over the file name and extension
	strncpy(relativePath, r, index);
	//Base file path for ALL HTML FILES
	char *path = "/home/jemushatt/Desktop/Network Projects/BasicClient-Server/BasicServer/html/";
	//Allocate enough memory
	char *filePath = malloc(sizeof(char) * (strlen(path)+strlen(relativePath)+1));
	//Concatenate path onto file path
	strcat(filePath,path);
	//Concatenate relativePath onto file path
	strcat(filePath,relativePath);
	//Open the file
	FILE *f;
	f=fopen(path, "r");
	//If we can get the file opened
	if (f != 0) {

		//Get the files size for copying # of bytes
		int fd = fileno(f);
		struct stat buf;
		fstat(fd, &buf);
		off_t size= buf.st_size;
		char bytes[size+1];
		int attempt = fopen(bytes, sizeof(char), size, f);
		if(attempt>0) {
			return bytes;
		}
		else {
			perror("Could not copy file contents\n");
		}
	}
}
/**
 * Generates an http response given the http request
 * @param buffer Http request string
 * @return struct containing some parsed elements from the request and the proper response packages
 */
http_response_package buildResponse(char* buffer) {
	http_response_package resp;
	//resp.content_size = int2char(size);
	//resp.content_type = ""
	resp.content= getFileBytes(buffer);
	return resp;
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
		valread = read(new_socket, buffer, 30000);

		char *request = malloc(sizeof(char) * 56);
		strncpy(request, buffer, 15);
		*(request + 16) = '\0';
		FILE *file =
				fopen(
						"/home/jemushatt/Desktop/Network Projects/BasicClient-Server/BasicServer/html/index.html",
						"r");

		http_response_package resp;
		resp = buildResponse(buffer);
		printf("%s\n", buffer);
		printf("Extracted request: %s\n", request);
		//write(new_socket, hello, strlen(hello));
		//printf("------------------Hello message sent-------------------");
		close(new_socket);
	}
	return 0;
}
