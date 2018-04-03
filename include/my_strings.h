#ifndef MY_STRINGS_H
#define MY_STRINGS_H

#include <stdio.h>

int my_strcmp (char* , char* );  // compare two strings 

void my_strcut (char* , char , unsigned int , char* , int );  // cut substring using given delimiter and given delimiter number

void my_strcat (char* , char* , char* , int ); // concatenate two strings

void my_itoa ( int , char*); // convert signed int to string

void str_append ( char* , char* , int); // append string

uint32_t my_atoi(char*); // convert string to int

#endif
