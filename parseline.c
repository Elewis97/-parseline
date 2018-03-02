#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMAX 512 /*Command line length max*/
#define PMAX 10  /*Pipeline command max*/

void errorHandling(int argc, char *argv[])
{
	int i;
	int cLen = 0;
	/*check if commandline exceeds limits*/
	for (i = 0; i < argc; i++) {
		cLen += strlen(argv[i]);
		cLen += 1; /*to handle spaces in between*/
	}
	cLen -= 1; /*go get rid of last space*/

	if (cLen > CMAX) {
		fprintf(stderr, "Command line length too long\n");
		exit(EXIT_FAILURE);
	}
	/*test for malformed redirects*/

	/*test for ambiguous inputs or outputs*/

}

int main(int argc, char *argv[])
{
	/*handle errors*/
	errorHandling(argc, argv);

	printf("Hello, World!\n");
	return 0;
}
