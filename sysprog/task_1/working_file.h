#ifndef WORKING_FILE_H
#define WORKING_FILE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE_INCREMENT 256

typedef struct
{
	char *lineptr; //указатель на начало
	int len_line; 
} lines;


//const char file_name[] = "Asm.txt";
char* read_file_in_array(char *file_name);
lines* start_of_lines(char* text, size_t* count_lines);
int* formation_int(lines *words,  size_t size_arr);

#endif