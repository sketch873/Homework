#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
	FILE* fd = fopen("input", "r");
	FILE* fo = fopen("output", "w");
	char *x = (char*) malloc(11 * sizeof(char));

	int fdd = open("input", O_RDONLY);
	int len = lseek(fdd, 0, SEEK_END);
	printf("Lungimea este %d !\n", len);

		/*
	if(fd == NULL || fo == NULL) {
		printf("EROARE\n");
		return 1;
	}
	
	fseek(fd, 0, SEEK_END);
	int len = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	while( len >= 10) {
		fread(x, sizeof(char), 10, fd);
		fwrite(x, sizeof(char), 10, fo);

		len -= 10;
	}

	fread(x, sizeof(char), len, fd);
	fwrite(x, sizeof(char), len, fo);
*/
	return 0;
}
