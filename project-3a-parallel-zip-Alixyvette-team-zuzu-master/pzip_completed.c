#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>


//#define NUMBER_OF_THREADS 1 
#define NUMBER_OF_THREADS 4

/*	initizlizing thread:
thread:		a pointer to pthread_t structure.
attr:		(we use null?) any atributes might need.
start_routine:	a function.
arg:		argument passed to the function "start_routine".
*/
//int pthread_create(pthread_t * thread, const pthread_attr_t * attr, void * (*start_routine)(void *), void * arg);

size_t getfilesize(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

typedef struct __arg_t{
	void *head;
	size_t blocksize;
	int idx;
	int compressedSize;

	char *compressed;
	int* compressedNums;
} arg_t;


char * testing;
int * testingInts;


void fixInter(arg_t *arguments, int idx_i){
	//A end:
	char a = arguments[idx_i].compressed[arguments[idx_i].compressedSize - 1];	
	//B head:
	char b = arguments[idx_i + 1].compressed[0];
	if (a == b){
		//combine A and B intersection to A
		arguments[idx_i].compressedNums[arguments[idx_i].compressedSize - 1] += arguments[idx_i + 1].compressedNums[0];
		//delete B head
		arguments[idx_i + 1].compressed++;
		arguments[idx_i + 1].compressedNums++;
		arguments[idx_i + 1].compressedSize--;
	}

}


void printEverything(pthread_t *ps, arg_t *arguments){
//join first thread

		pthread_join(ps[0], NULL);
//k starts from 1 instead
		for (int k = 1; k < NUMBER_OF_THREADS; k++){			
//TODO maybe put a new array for intersaction and write big here first.

			pthread_join(ps[k], NULL);
//fix A and B
			fixInter(arguments, k-1);
			testing = arguments[k-1].compressed;
                        testingInts = arguments[k-1].compressedNums;
			while (*testing != '\0'){
				printf("%d",*testingInts);
				printf("%c",*testing);
//				fwrite(testingInts, sizeof(int), 1, stdout);
//				fwrite(testing, sizeof(char), 1, stdout);


                                testing++;
                                testingInts++;
               		}
		}
//write Last thread
		testing = arguments[NUMBER_OF_THREADS-1].compressed;
              	testingInts = arguments[NUMBER_OF_THREADS-1].compressedNums;
               	while (*testing != '\0'){
			printf("%d",*testingInts);
			printf("%c",*testing);
//                              fwrite(testingInts, sizeof(int), 1, stdout);
//                            fwrite(testing, sizeof(char), 1, stdout);

               	testing++;
                testingInts++;
               	}   
}


void writeEverything(pthread_t *ps, arg_t *arguments){
//join first thread

		pthread_join(ps[0], NULL);
//k starts from 1 instead
		for (int k = 1; k < NUMBER_OF_THREADS; k++){			
//TODO maybe put a new array for intersaction and write big here first.

			pthread_join(ps[k], NULL);
//fix A and B
			fixInter(arguments, k-1);
			testing = arguments[k-1].compressed;
                        testingInts = arguments[k-1].compressedNums;
			while (*testing != '\0'){
//				printf("%d",*testingInts);
//				printf("%c",*testing);
				fwrite(testingInts, sizeof(int), 1, stdout);
				fwrite(testing, sizeof(char), 1, stdout);


                                testing++;
                                testingInts++;
               		}
		}
//write Last thread
		testing = arguments[NUMBER_OF_THREADS-1].compressed;
              	testingInts = arguments[NUMBER_OF_THREADS-1].compressedNums;
               	while (*testing != '\0'){
//			printf("%d",*testingInts);
//			printf("%c",*testing);
                              fwrite(testingInts, sizeof(int), 1, stdout);
                              fwrite(testing, sizeof(char), 1, stdout);

               	testing++;
                testingInts++;
               	}   
}

void *compression(void *arguments){
//void compression(void *arguments){
	arg_t *argument = (arg_t *)arguments;

//printf("head is %ld\n", (long int)argument->head);
	int offset = 0;
	char a = '\0';
	char b = '\0';
	int counter = 1;
	char* alphaP = argument->compressed;
	int* intP = argument->compressedNums;
//printf("blocksize is %ld\n", argument->blocksize);
	while (offset != argument->blocksize){

	a = *(((char *)(argument->head)) + offset);
//printf("%c", a);
		// end case:
		if (offset != (argument->blocksize -1)){
			b = *(((char *)(argument->head)) + offset + 1);
			// if equal
			if (a == b){
				counter++;
			} else {
				*intP = counter;
//printf("%d: %d",argument->idx, counter);
				intP++;
				*alphaP = a;
//printf("%d: %c\n",argument->idx, a);
				alphaP++;
				// at the end we should append \0 but it's okay if it is overwritten
				*alphaP = '\0';
				// restart count
				counter = 1;
				argument->compressedSize++;
			}
		} else {
			
			*intP = counter;
//pintf("%d: %d",argument->idx, counter);
			*alphaP = a;
//printf("%d: %c\n",argument->idx, a);
			alphaP++;
			// at the end we should append \0 but it's okay if it is overwritten
			*alphaP = '\0';
			// restart count
			argument->compressedSize++;
		}

	offset++;
//break;
	}
//printf("offse is %d\n", offset);


	return NULL;
}





int main (int argc, char ** argv) {

//try getfilesize:
//printf("%ld\n", getfilesize(argv[1]));


	if (argc == 1){
		printf("my-zip: file1 [file2 ...]\n");
		return 1;
	}

	for (int i = 1; i < argc; i++){

		//file discriptor:
		int fd = open(argv[i], O_RDONLY);
	    	size_t filesize = getfilesize(argv[i]);
		int blockcalc = filesize/NUMBER_OF_THREADS;
		int remainderBits = filesize%NUMBER_OF_THREADS;

		//0 is offset:
		void *data = mmap(NULL, filesize, PROT_READ, MAP_SHARED | MAP_POPULATE, fd, 0);
//printf("data is %ld\n", (long int)data);
		close(fd);

//1 is stdoutput TODO
//		int trash = write(1, data, filesize);
//		printf("[trash is %d]\n", trash);
		
		//create threads:
		pthread_t ps[NUMBER_OF_THREADS];
		arg_t arguments[NUMBER_OF_THREADS];
		for(int j = 0; j < NUMBER_OF_THREADS; j++){

//printf("[%d]", j);
			arguments[j].blocksize = blockcalc;
			
			if (remainderBits != 0 && j == (NUMBER_OF_THREADS -1)){
				arguments[j].blocksize += remainderBits;
			}
			
			arguments[j].head = data + (j * (blockcalc));
			arguments[j].compressedSize = 0;
			arguments[j].idx = j;
			arguments[j].compressed = (char*)malloc(arguments[j].blocksize*sizeof(char)+1);
			arguments[j].compressedNums = (int*)malloc(arguments[j].blocksize*sizeof(int));
			pthread_create(&(ps[j]), NULL, compression, &(arguments[j]));
//compression(&arguments[j]);			
		//	testing = arguments[j].compressed;
		//	testingInts = arguments[j].compressedNums;
			/*while (*testing == '\0'){
				printf("pointer is null\n");//printf("%s", testing);
				//testing++;
				printf("%s", testing);
                                testing++;
				printf("%s", testing);
                                testing++;
			}*/
			//printf("is this printing???\n"); 
		//	pthread_join(ps[j], NULL);// do this in separate loop after this one

			/*while (*testing != '\0'){
				printf("%d",*testingInts);
				printf("%c",*testing);
				testing++;
				testingInts++;
			}*/
			//pthread_join(ps[j], NULL);
		}

//		for (int l = 0; l < NUMBER_OF_THREADS; l++){
//			pthread_join(ps[l], NULL);
//		}
		
//printf("\n[%d done.]\n", k);
//printf("%ld\n", (unsigned long int)arguments[k].head);

		
//printf("%ld\n",(unsigned long int)data);
		
//shabi
		writeEverything(ps, arguments);
//		printEverything(ps, arguments);
		munmap(data, filesize);

	}
	return 0;
}
