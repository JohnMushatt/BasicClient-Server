/*
 * http_response.c
 *
 *  Created on: May 9, 2019
 *      Author: jemushatt
 */
#include "http_response.h"

/**
 * Generates an http response given the http request
 * @param buffer Http request string
 * @return struct containing some parsed elements from the request and the proper response packages
 */
http_response_package *buildResponse(char* buffer) {
	http_response_package *resp = malloc(sizeof(http_response_package));
	resp->protocol_version = "HTTP/1.1";
	//resp.content_size = int2char(size);
	getContent_Type(buffer, resp);
	getFileBytes(buffer, resp);
	printf("HTTP Response created!\n");
	return resp;
}

/**
 * Parses http request for content type based on file extension that is being requested
 * @param buffer HTTP request to be parsed
 * @return Pointer to a buffer containing either the content-type or something else
 */
void getContent_Type(const char *buffer, http_response_package *resp) {
	char *content_type;
	char *content;
	if ((content = strstr(buffer, ".html")) != 0) {
		content_type = "text/html";
	}
	resp->content_type = content_type;
}
/**
 * Prints the http_response_package struct
 */
void print_package(http_response_package *resp) {
	printf("Protocol Version: %s\nStatus Code: %s\nStatus Text: %s\n",
			resp->protocol_version, resp->status_code, resp->status_text);
	printf("Content-Type: %s\nContent-Size: %s\nContent: %s",
			resp->content_type, resp->content_size, resp->content);

}
/**
 * Formats the http_response_package into a buffer than can be sent in a socket
 */
char *formatPackage(http_response_package *resp) {
	char *buffer = malloc(sizeof(http_response_package) + 100);
	strcat(buffer, resp->protocol_version);
	strcat(buffer, " ");
	strcat(buffer, resp->status_code);
	strcat(buffer, " ");
	strcat(buffer, resp->status_text);
	strcat(buffer, "\nContent-Length: ");
	strcat(buffer, resp->content_size);
	strcat(buffer, "\nContent-Type: ");
	strcat(buffer, resp->content_type);
	strcat(buffer, "\n\n");
	strcat(buffer, resp->content);
	strcat(buffer, "\n");
	/*
	 char *buffer = " " + resp->protocol_version + " " + resp->status_code + " "
	 + resp->status_text + "\nContent-Length: " + resp->content_size
	 + "\nContent-Type: " + resp->content_type;
	 */
	printf(
			"---------------FORMATTED PACKAGE---------------\n%s\n------------------------------\n",
			buffer);
	return buffer;
}
/**
 * Gets the file from the HTTP request and parses its content into a buffer
 * @param buffer HTTP Request to parse
 * @return Buffer containing parsed file content
 */
void getFileBytes(const char *buffer, http_response_package *resp) {
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
	char *path =
			"/home/jemushatt/Desktop/Network Projects/BasicClient-Server/BasicServer/html/";
	//Allocate enough memory
	char *filePath = malloc(
			sizeof(char) * (strlen(path) + strlen(relativePath) + 1));

	//Concatenate path onto file path
	strcat(filePath, path);

	//Concatenate relativePath onto file path
	strcat(filePath, relativePath);

	//Open the file
	FILE *f;
	f = fopen(filePath, "r");
	char *bytes;

	//If we can get the file opened
	if (f != 0) {

		//Get the files size for copying # of bytes
		int fd = fileno(f);
		struct stat buf;
		fstat(fd, &buf);
		off_t size = buf.st_size;
		bytes = malloc(sizeof(char) * (size + 1));
		int attempt = fread(bytes, sizeof(char), size, f);
		//If successfully copied content, free up non-essential buffers
		if (attempt > 0) {
			resp->content = bytes;
			resp->content_size = int2char(size);
			resp->status_code = "200";
			resp->status_text = "Success";
		} else {
			perror("Could not copy file contents\n");
			resp->status_code = "404";
			resp->status_text = "Could not find requested file";
			f =
					fopen(
							"/home/jemushatt/Desktop/Network Projects/BasicClient-Server/BasicServer/html/error.html",
							"r");
			fd = fileno(f);
			fstat(fd, &buf);
			size = buf.st_size;
			bytes = malloc(sizeof(char) * (size + 1));
			int attempt = fread(bytes, sizeof(char), size, f);

			resp->content = bytes;
			resp->content_size = int2char(size);
			resp->content_type = "text/html";
		}
	} else {
		perror("Could not copy file contents\n");
		resp->status_code = "404";
		resp->status_text = "Could not find requested file";
		f =
				fopen(
						"/home/jemushatt/Desktop/Network Projects/BasicClient-Server/BasicServer/html/error.html",
						"r");
		int fd = fileno(f);
		struct stat buf;

		fstat(fd, &buf);
		off_t size = buf.st_size;
		bytes = malloc(sizeof(char) * (size + 1));
		int attempt = fread(bytes, sizeof(char), size, f);

		resp->content = bytes;
		resp->content_size = int2char(size);
		resp->content_type = "text/html";
	}
}

