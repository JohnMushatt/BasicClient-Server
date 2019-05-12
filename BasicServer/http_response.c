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
	http_response_package *resp;
	//resp.content_size = int2char(size);
	getContent_Type(buffer,resp);
	getFileBytes(buffer,resp);
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
	if (content = strstr(buffer, ".html") != 0) {
		content_type = "text/html";
	}
	resp->content_type=content_type;
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
			resp->content=bytes;
			resp->content_size=size;
		} else {
			perror("Could not copy file contents\n");
		}
	} else {
		perror("Could not copy file contents\n");
	}
}

