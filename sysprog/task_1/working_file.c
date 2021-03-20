#include "working_file.h"



char* read_file_in_array(char* file_name)
{
	int lenght_file = 0;
	char *text = NULL;
	FILE *fp = NULL;
	fp = fopen((const char*) file_name, "r");

	if (fp == NULL)
	{
		perror("Can't open file");
		return NULL;
	}

	/* formation of source text in text[] */
	fseek(fp, 0, SEEK_END);

	if ((lenght_file = ftell(fp)) == -1L)
	{
		perror("ftell err");
		return NULL;
	}

	fseek(fp, 0, SEEK_SET);

	text = (char *) calloc(lenght_file + 1, sizeof(char));

	if (text == NULL)
	{
		perror("nope memory");
		return  NULL;	
	}


	long int read_b = fread(text, sizeof(char), lenght_file, fp);

	while (read_b != lenght_file)
	{
		read_b += fread(text, sizeof(char), lenght_file, fp);
	}

	fclose(fp);
	return text;
}


/* formation array pointers to start of lines text */

lines* start_of_lines(char* text, size_t* count_lines)
{
	lines *line = NULL;
	lines *supp = NULL; //промежуточный указатель
	int n = SIZE_INCREMENT; //initial size array
	line = (lines *) calloc(SIZE_INCREMENT, sizeof(lines));

	if (line == NULL)
	{
		perror("nope memory");
		return  NULL;
	}

	char *s = text;
	char *s_next = strchr(s, ' '); //next pointer after word

	int i = 0;
	line[i].len_line = s_next - s;
	line[i].lineptr = s;
	line[i].lineptr[line[i].len_line] = '\0';
	i++;
	s = s_next;
	if ((s_next = strchr(s + 1, ' ')) == NULL)
	{
		perror("nope strings");
		return NULL;
	}

	while (s_next != NULL)
	{
		line[i].len_line = s_next - s - 1;
		line[i].lineptr = s + 1;
		line[i].lineptr[line[i].len_line] = '\0';
		s = s_next;
		s_next = strchr(s+1, ' ');

		i++;
		if (i == n)
		{
			n += SIZE_INCREMENT;
			supp = (lines *) realloc(line, n * sizeof(lines));
			if (supp == NULL)
			{
				perror("nope memory");
				return  NULL;
			}
			line = supp;
		}
	}

	if (s_next == NULL) //последний может быть и без пробела на конце
	{
		s_next = strchr(s + 1, '\0');
		line[i].len_line = s_next - s - 1;
		line[i].lineptr = s + 1;
		line[i].lineptr[line[i].len_line] = '\0';
		i++;
	}

	supp = (lines *) realloc(line, i * sizeof(lines));

	if (supp == NULL)
	{
		perror("nope memory");
		return NULL;
	}

	line = supp;
	
	*count_lines = i; //всего строк

	return line;
}


int* formation_int(lines* words, size_t size_arr)
{
	int *int_array = (int *) calloc(size_arr, sizeof(int));

	for (int i = 0; i < size_arr; i++)
	{
		int_array[i] = atoi(words[i].lineptr);
	}

	return int_array;
}