/*
 * utilities.h
 *
 *  Created on: May 9, 2019
 *      Author: jemushatt
 */

#ifndef UTILITIES_H_
#include <stdlib.h>
#include <ctype.h>

#define UTILITIES_H_
/**
 * Get the # of digitits in a given number
 * @param num # to count digits
 * @param # of digits in num
 */

int getDigits(int num);
/**
 * Converts integer to char
 * @param num Number to convert
 * @return Pointer to buffer containing newly converted char version of integer
 */

char *int2char(int num);
/**
 * Take in a string and parse it, then compute equation's value
 * @param buffer Char buffer that holds the equation
 * @param len Length of buffer
 * @return char value of equation
 */
int computeEquation(char* buffer, int len);

#endif /* UTILITIES_H_ */
