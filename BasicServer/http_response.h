/*
 * http_response.h
 *
 *  Created on: May 5, 2019
 *      Author: jemushatt
 */

#ifndef HTTP_RESPONSE_H_
#define HTTP_RESPONSE_H_

typedef struct {
	char *content_type;
	char *content_size;
	char *content;

} http_response_package;

#endif /* HTTP_RESPONSE_H_ */
