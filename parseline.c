#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMAX 512 /*Command line length max*/
#define PMAX 10  /*Pipeline command max*/

char** splitStr(char *path, const char delimiter)
{
	char **res = 0;
	int count = 0;
	char *tempPtr = path;
	char *lastDelim = 0;
	char delim[2];
	int idx;
	char *token;

	/*init delim*/
	delim[0] = delimiter;
	delim[1] = 0;

	/*get number of elements*/
	while(*tempPtr) {
		if(delimiter == *tempPtr) {
			count ++;
			lastDelim = tempPtr;
		}
		tempPtr ++;
	}

	count += lastDelim	 < (path + strlen(path) - 1);
	count ++;

	res = malloc(sizeof(char*) * count);

	if (res) {
		idx = 0;
		token = strtok(path, delim);

		while(token) {
			assert(idx < count);
			*(res + idx++) = strdup(token);
			token = strtok(0, delim);
		}
		assert(idx == count - 1);
		*(res + idx) = 0;
	}
	return res;
}


void getCommand()
{
	char command[CMAX];
	char c;
	int idx = 0;

	printf("line: ");

	/*get command while checking if input
	exceeds command line length max (CMAX)*/
	while((c = getchar()) != '\n') {
		command[idx] = c;
		idx++;
		if (idx > CMAX) {
			fprintf(stderr,"command too long\n");
			exit(EXIT_FAILURE);
		}
	}
	
}

int main()
{
	/*get line*/
	getCommand();	

	printf("Hello, World!\n");
	return 0;
}
