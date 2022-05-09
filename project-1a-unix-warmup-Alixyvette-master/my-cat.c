#include <stdlib.h>
#include <stdio.h>


int main (int argc, char ** argv) {
	FILE *fp;
	char theStr [100];
	for (int i = 1; i < argc; i++){
		fp = fopen(argv[i], "r");
		if (fp == NULL){
			printf("my-cat: cannot open file\n");
			exit(1);
		}
		while( fgets( theStr,100,fp ) != NULL){
			printf("%s",theStr);
		}
		fclose(fp);
	}
	return 0;
}
