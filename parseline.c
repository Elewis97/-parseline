#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define CMAX 512 /*Command line length max*/
#define PMAX 10  /*Pipeline command max*/

/********************Helper****************/

char** splitStr(char path[], const char delimiter)
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

void freeRemainingTokens(char **tokens, int i)
{
	while(*(tokens + i)) {
		free(*(tokens + i));
		i++;
	}
	free(tokens);
}

/*Test errors*/ 

bool getCommand(char arg[])
{
	int i;
	char **commands;

	commands = splitStr(arg, ' ');

	/*loop through commands*/
	i = 0;
	while(*(commands + i)) {
		/*do something*/

		/*check if arg too big*/
		if (i > PMAX) {
			freeRemainingTokens(commands, i);
			exit(EXIT_FAILURE);
		}
		i++;
	}

	freeRemainingTokens(commands, i);
	free(commands);
	return false;
}

void getStages(char arg[], int stageNum, char** tokens)
{
	bool error = false;
	printf("--------\nStage %d: ", stageNum);
	printf("\"%s\"\n", arg);
	printf("--------\n");

	/*ensure to not exceed state limit*/
	if(stageNum >= 2) {
		fprintf(stderr, "output line limit (20) exceeded.\n");
		freeRemainingTokens(tokens, stageNum);
		exit(EXIT_FAILURE);
	}

	error = getCommand(arg);

	if (error) {
		freeRemainingTokens(tokens, stageNum);
		exit(EXIT_FAILURE);
	}
}

void getLine()
{
	char line[CMAX];
	char c;
	int idx = 0;
	char **tokens;
	int i;

	printf("line: ");

	/*get command while checking if input
	exceeds command line length max (CMAX)*/
	while((c = getchar()) != '\n') {
		line[idx] = c;
		idx++;
		if (idx > CMAX) {
			fprintf(stderr,"command too long\n");
			exit(EXIT_FAILURE);
		}
	}
	line[idx] = '\0';

	/*Check if there are any commands*/
	if (strlen(line) <= 1) {
		fprintf(stderr, "No command entered\n");
		exit(EXIT_FAILURE);
	}

	/*Parse commands by pipeline*/
	tokens = splitStr(line, '|');
	
	/*exit if there aren't any commands*/
	if(!tokens) {
		fprintf(stderr, "No arguments outputed\n");
		exit(EXIT_FAILURE);
	}

	/*loop through stages*/
	i = 0;
	while(*(tokens + i)) {
		getStages(*(tokens + i), i, tokens);
		free(*(tokens + i));
		i++;
	}

	free(tokens);
}

int main()
{
	/*get line*/
	getLine();	

	printf("Hello, World!\n");
	return 0;
}
