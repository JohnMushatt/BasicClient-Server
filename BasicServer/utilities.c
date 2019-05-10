/*
 * utilities.c
 *
 *  Created on: May 9, 2019
 *      Author: jemushatt
 */


#include "utilities.h"
int getDigits(int num) {
	int number = num;
	int count = 0;
	while (number > 0) {
		number = number / 10;
		count++;
	}
	return count;
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

int computeEquation(char *buffer, int len) {
	int result;
	int termOne = 0, termTwo = 0;
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
