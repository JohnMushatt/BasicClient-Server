/*
 * http_response.h
 *
 *  Created on: May 5, 2019
 *      Author: jemushatt
 */

#ifndef HTTP_RESPONSE_H_
#define HTTP_RESPONSE_H_
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>


typedef struct {
	char *content_type;
	char *content_size;
	char *content;

} http_response_package;

http_response_package buildResponse(char* buffer);

char *getFileBytes(const char *buffer);

char *getContent_Type(const char *buffer);
#endif /* HTTP_RESPONSE_H_ */
