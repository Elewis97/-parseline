#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMAX 512 /*Command line length max*/
#define PMAX 10  /*Pipeline command max*/

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
			fprintf(stderr,"Too much input");
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
