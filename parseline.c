#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define CMAX 512 /*Command line length max*/
#define PMAX 10  /*Pipeline command max*/

struct Stage {
	char input[CMAX];
	char output[CMAX];
	int argc;
	char argv[CMAX];
};

/********************Helper****************/

struct Stage *initStage()
{
	struct Stage *stage = malloc(sizeof(struct Stage));
	char temp[CMAX];
	int i;

	for (i = 0; i < CMAX; i++) {
		temp[i] = '\0';
	}

	strcpy(stage->input, temp);
	strcpy(stage->output, temp);
	strcpy(stage->argv, temp);

	return stage;
}

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

void displayStage(struct Stage *stage)
{
	printf("%10s: %s\n", "input", stage->input);
	printf("%10s: %s\n", "output", stage->output);
	printf("%10s: %d\n", "argc", stage->argc);
	printf("%10s: %s\n", "argv", stage->argv);
}

<<<<<<< HEAD
int get_redir(char **args_v, char *redir) {

	int i = 0;
	while(i < 10 && args_v[i] != NULL) {
		if(strcmp(args_v[i], redir) == 0) {
			return i;
		}
	}
	return -1;
}

void check_double_redirect(char arg[], char **args_v, char *sym) {

	char *redir = NULL;

	redir = strstr(arg, sym);
	if(redir != NULL) {
		redir = strstr(arg, sym);
		if(redir != NULL) {
			fprintf(stderr, "%s: bad input redirection", args_v[0]);
			exit(0);
		}
	}
}


struct Stage *fillCommand(char arg[], char **tokens, int tokIdx, int len)
=======
struct Stage *fillCommand(char arg[], char **tokens, int tokNum,
	int len)
>>>>>>> d76943a2698f8a75d24283a7c75f1498bc0bc4c3
{
	struct Stage *stage = initStage();
	int count = 0;
	char *token;
	char tempArgv[CMAX];
	int i;
	char arg_cpy[512];
	char *args_v[10] = {NULL};
	int redir_in = -1;
	int redir_out = -1;

	i = 0;
	strcpy(arg_cpy, arg);
	token = strtok(arg_cpy, " ");
	while(token != NULL) {
		if(i > 10) {
			fprinf(stderr, "commands too long\n");
		}
		args_v[i] = token;
		strtok(NULL, " ");
		i++;
	}

	check_double_redirect(arg, args_v, "<");
	check_double_redirect(arg, args_v, ">");
	
	redir_in = get_redir(args_v, "<");
	redir_out = get_redir(args_v, ">");

	if(tokIdx == 0) {
		if(redir_in > 0) {
			strcpy(stage -> input, args_v[redir_in + 1]);
		}
		else {
			strcpy(stage -> input, "original stdin");
		}
	}
	else {
		if(redir_in > 0) {
			fprintf(stderr, "")
		}
		else {
			strcpy(stage -> input, *(token + tokIdx - 1));
		}
	}

	if(strstr(arg, ">") && tokIdx == len - 1) {


	}

	/*check argv*/
	token = strtok(arg, " ");
	for (i = 0; i < CMAX; i++)
		tempArgv[i] = '\0';

	while(token != NULL) {
		if (strcmp(token, "<") != 0 &&
			strcmp(token, ">") != 0 &&
			strcmp(token, stage->input) != 0 &&
			strcmp(token, stage->output) != 0) {
			strcat(tempArgv, "\"");
			strcat(tempArgv, token);
			strcat(tempArgv, "\",");
			count ++;
		}
		token = strtok(NULL, " ");
	}
	/*remove last two characters */
	if (strlen(tempArgv) >= 1) {
		tempArgv[strlen(tempArgv) - 1] = '\0';
	}
	strcpy(stage->argv, arg);
	printf("%10s: %d\n", "argc", count);
	printf("%10s: %s\n", "argv", tempArgv);

	/*CHANGE THIS LATER*/
	stage->argc = 1;

	free(stage);

	return stage;
}

/*bool getCommand(char arg[])
{
	int i;
	char **commands;

	commands = splitStr(arg, ' ');

	fillCommand(arg);*/

	/*loop through commands*/
	/*i = 0;
	while(*(commands + i)) {*/
		/*if stdout is valid*/

		/*check if arg too big*/
		/*if (i > PMAX) {
			freeRemainingTokens(commands, i);
			exit(EXIT_FAILURE);
		}
		i++;
	}

	freeRemainingTokens(commands, i);
	return false;
}*/

bool getCommand(char arg[], char** tokens, int tokIdx)
{
	int i = 0;
	int len = 0;

	while(*(tokens + i)) {
		len ++;
		i++;
	}
	struct Stage *stage = fillCommand(arg, tokens, tokIdx, len);
	/*displayStage(stage);*/
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

	error = getCommand(arg, tokens, stageNum);

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

	return 0;
}
