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
#include "utilities.h"

typedef struct {
	char *protocol_version;
	char *status_code;
	char *status_text;
	char *content_type;
	char *content_size;
	char *content;

} http_response_package;

http_response_package *buildResponse(char* buffer);

void getFileBytes(const char *buffer,http_response_package *resp);

void getContent_Type(const char *buffer,http_response_package *resp);
void print_package(http_response_package *resp);
char *formatPackage(http_response_package *resp);
#endif /* HTTP_RESPONSE_H_ */
