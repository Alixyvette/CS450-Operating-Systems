#include <stdlib.h>
#include <stdio.h>


int main (int argc, char ** argv) {
	FILE *fp;
	char theChar;
	int numChars;
	if( argc < 2){
		printf("my-unzip: file1 [file2 ...]\n");
		exit(1);
	}

	for(int i = 1; i < argc; i++){
		fp = fopen(argv[i], "r");
		if (fp == NULL){
			printf("my-unzip: cannot open file\n");
			exit(1);
		}
		do{
		if (fread(&numChars,4,1,fp)!=1){
			break;
		}
	
		theChar = getc(fp);
		for (int j = 0; j < numChars; j++){
			printf("%c", theChar);
		}

		} while (theChar != EOF);
		fclose(fp);
	}
    return 0;
}
