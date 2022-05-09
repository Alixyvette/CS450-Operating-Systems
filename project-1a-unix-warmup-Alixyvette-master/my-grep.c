#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main (int argc, char ** argv) {
	FILE *fp;
	char *line = 0;
	size_t lineLen;
	char *found = 0;
	char theStr [100];
	if( argc < 2){
		printf("my-grep: searchterm [file ...]\n");
		exit(1);
	}
	if( argc == 2){
		while( fgets(theStr, 100, stdin ) != NULL){
			found = strstr(theStr,argv[1]);
			if(found) printf("%s", theStr);
		}
		return 0;
	}
	for(int i = 2;  i < argc; i++){
		fp = fopen(argv[i], "r");
		if (fp == NULL){
			printf("my-grep: cannot open file\n");
			exit(1);
		}
		while( getline(&line, &lineLen, fp) > -1){
			found = strstr(line, argv[1]);
			if(found) printf("%s",line);
		}
		fclose(fp);
	}
	return 0;
}
