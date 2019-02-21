#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char *c = (char*) malloc(10 * sizeof(char));

	free(c);
	free(c);
	return 0;
}
