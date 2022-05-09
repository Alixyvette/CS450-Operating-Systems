#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main (int argc, char ** argv) {
	FILE *fp;
	int theASCII;
	int prev;
	int count = 0;

	if( argc < 2){
		printf("my-zip: file1 [file2 ...]\n");
		exit(1);
	}
	for(int i = 1; i < argc; i++){ //for each command
		fp = fopen(argv[i], "r");
		if (fp == NULL){
			printf("my-zip: cannot open file\n");
			exit(1);
		}
		prev = getc(fp);
		count++;	// first instance of a letter
		theASCII = getc(fp); // maybe second
		while( theASCII != EOF){
			if(theASCII == prev){
				count++;
				prev = theASCII;
				theASCII = getc(fp);
				if(theASCII == EOF && i+1 == argc){
				fwrite(&count,sizeof(int),1,stdout);
				printf("%c",prev);
				}
			}
			else{
				//you have the number and the ASCII
				fwrite(&count,sizeof(int),1,stdout);
				printf("%c",prev);
				prev = theASCII;
				count = 1;
				theASCII = getc(fp);
				if(theASCII == EOF && i+1 == argc){
					fwrite(&count,sizeof(int),1,stdout);
					printf("%c",prev);
				}
			}
		}
		fclose(fp);
	}
	return(0);
}
