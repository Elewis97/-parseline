#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

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

int get_redir(char **args_v, char *redir) {

	int i = 0;
	while(i < 10 && args_v[i] != NULL) {
		if(strcmp(args_v[i], redir) == 0) {
			return i;
		}
		i++;
	}
	return -1;
}

void check_double_redirect(char arg[], char **args_v, char *sym) {

	char *redir = NULL;

	redir = strstr(arg, sym);
	/*printf("readir1: %s", redir);*/
	if(redir != NULL) {
		redir = strstr(redir + 1, sym);
		/*printf("readir2: %s", redir);*/
		if(redir != NULL) {
			fprintf(stderr,
				"%s: bad input redirection", args_v[0]);
			exit(0);
		}
	}
}

void get_prev_cmd(char **tokens, int tokIdx, char buffer[]) {

	int i = 0;
	char *arg;

	/*printf("tokens: %s", *(tokens));*/
	arg = *(tokens + tokIdx - 1);
	/*printf("getPrevArg: %s, (%d)\n", arg, tokIdx);*/

	while(!isspace(arg[i])) {
		buffer[i] = arg[i];
		i++;
	}
}


void get_next_cmd(char **tokens, int tokIdx, char buffer[]) {

	int i = 0;
	char *arg = *(tokens + tokIdx + 1);

	while(!isspace(arg[i])) {
		buffer[i] = arg[i];
		i++;
	}
}


struct Stage *fillCommand(char arg[], char **tokens, int tokIdx, int len)
{
	struct Stage *stage = initStage();
	int count = 0;
	char *token;
	char tempArgv[CMAX];
	int i;
	char arg_cpy[512] = {0};
	char *args_v[10] = {NULL};
	int redir_in = -1;
	int redir_out = -1;
	char prev_cmd[512] = {0};
	char next_cmd[512] = {0};
	char pipe_num_in = 0;
	char pipe_num_out = 0;

	/*printf("arg: %s\n", arg);*/
	/*printf("tokens: %s\n", *(tokens));*/

	/*check if argument is all spaces*/
	if (strcmp(arg, " ") == 0) {
		fprintf(stderr, "invalid null command\n");
		free(stage);
		freeRemainingTokens(tokens, tokIdx);
		exit(EXIT_FAILURE);
	}

	i = 0;

	for (i = 0; i < CMAX; i++)
		arg_cpy[i] = '\0';

	strcpy(arg_cpy, arg);
	token = strtok(arg_cpy, " ");
	for (i = 0; i < CMAX; i++)
		tempArgv[i] = '\0';

	i = 0;
	while(token != NULL) {
		args_v[i] = token;
		token = strtok(NULL, " ");
		i++;
	}

	i = 0;

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
		/*printf("prevCmd: %s\n", prev_cmd);*/
		get_prev_cmd(tokens, tokIdx, prev_cmd);
		/*printf("prevCmd: %s\n", prev_cmd);*/
		if(redir_in > 0) {
			fprintf(stderr, "%s: ambiguous input\n", prev_cmd);
		}
		else {
			snprintf(prev_cmd, 20,"pipe from stage %d",tokIdx - 1);
			/*pipe_num_in = (char) tokIdx - '0';
			prev_cmd[14] = pipe_num_in;*/
			strcpy(stage -> input, prev_cmd);
		}
	}

	/*check for output*/
	if (tokIdx == len - 1) {
		if(redir_out < 0) {
			strcpy(stage->output, "original stdout");
		}
		else {
			strcpy(stage->output, args_v[redir_out + 1]);
		}
	}
	else {
		get_next_cmd(tokens, tokIdx, next_cmd);
	/*	printf("nextCmd: %s\n", next_cmd);*/
		if(redir_out > 0) {
			fprintf(stderr, "%s: ambiguous output\n", next_cmd);
		}
		else {
			snprintf(next_cmd, 17, "pipe to stage %d", tokIdx + 1);
		//	pipe_num_out = (char) tokIdx - '0';
		//	prev_cmd[14] = pipe_num_out;
			strcpy(stage->output, next_cmd);
		}
	}

	printf("%10s: %s\n", "input", stage->input);
	printf("%10s: %s\n", "output", stage->output);
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
	/*remove last comma from argv*/
	if (strlen(tempArgv) >= 1) {
		tempArgv[strlen(tempArgv) - 1] = '\0';
	}
	strcpy(stage->argv, tempArgv);
	printf("%10s: %d\n", "argc", count);
	printf("%10s: %s\n", "argv", tempArgv);

	stage->argc = count;

	/*CHANGE THIS LATER*/

	free(stage);

	return stage;
}


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
		/*free(*(tokens + i));*/
		i++;
	}

	i = 0;
	while (*(tokens + i)) {
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
