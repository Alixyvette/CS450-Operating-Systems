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

	int curFile;
	int counter;
} arg_t;

struct {
        size_t *filesize; 
        void **data; 
	int filenum;
} ftable;


char * testing;
int * testingInts;


int fixInter(arg_t *arguments, int idx_i, int idx_f){
	//A end:
	if (arguments[idx_i].compressedSize ==0){
		return -1;
	}
	char a = arguments[idx_i].compressed[arguments[idx_i].compressedSize - 1];	
	//B head:
	char b = arguments[idx_f].compressed[0];
	if (a == b){
		//combine A and B intersection to A
		arguments[idx_i].compressedNums[arguments[idx_i].compressedSize - 1] += arguments[idx_f].compressedNums[0];
		//delete B head
		arguments[idx_f].compressed++;
		arguments[idx_f].compressedNums++;
		arguments[idx_f].compressedSize--;
		//return 0;
	}/* else if (a == '\0'){
		return -1;
	}*/
	return 0;
}


void printEverything(pthread_t *ps, arg_t *arguments){
//join first thread
		int nullFlag = -1;
		int correctA = 0;
		pthread_join(ps[0], NULL);
//k starts from 1 instead
		for (int k = 1; k < NUMBER_OF_THREADS; k++){			
//TODO maybe put a new array for intersaction and write big here first.

			pthread_join(ps[k], NULL);
//fix A and B		
			nullFlag = -1;
			correctA = k-1;
			while (nullFlag < 0){
				nullFlag = fixInter(arguments, correctA, k);
				if (nullFlag != 0){
					correctA--;
				}
			}
		}
		for (int k = 0; k < NUMBER_OF_THREADS; k++){			

			//if (nullFlag == 1){
			testing = arguments[k].compressed;
                        testingInts = arguments[k].compressedNums;
			while (*testing != '\0'){
//				printf("%d",*testingInts);
//				printf("%c",*testing);
				fwrite(testingInts, sizeof(int), 1, stdout);
				fwrite(testing, sizeof(char), 1, stdout);


                                testing++;
                                testingInts++;
               		}//}
		}
//write Last thread
/*		testing = arguments[NUMBER_OF_THREADS-1].compressed;
              	testingInts = arguments[NUMBER_OF_THREADS-1].compressedNums;
               	while (*testing != '\0'){
			printf("%d",*testingInts);
			printf("%c",*testing);
//                              fwrite(testingInts, sizeof(int), 1, stdout);
//                            fwrite(testing, sizeof(char), 1, stdout);

               	testing++;
                testingInts++;
               	}   
*/
}

int checkoffset(int offset_i, arg_t *argument, int bOrA){
//    for(int i = 0; i < ftable.filenum; i++){
	void *boundA = (ftable.data)[argument->curFile];
	void *boundB = (ftable.data)[argument->curFile] + ftable.filesize[argument->curFile];
	if (offset_i + (char *)(ftable.data[argument->curFile]) >= (char *)boundA && offset_i + (char *)((ftable.data[argument->curFile])) < (char *)boundB){
		return offset_i;
	}else if (argument->curFile != ftable.filenum){
//printf("WRONGGG\n");
		//b comes first, bOrA is 0, !0 is 1, curFile++.
		if (!bOrA){

			argument->curFile++;
		}
		else{
		//a:
			argument->counter = 0;

		}
		boundA = (ftable.data)[argument->curFile];
//		boundB = (ftable.data)[i+1] + ftable.filesize[i+1];
//		return ((char *)boundA + offset_i - ((char *)(argument->head)));
		return (char *)boundA - (char *)(argument->head);
	}

//    }
    return -1;
}


/*
int checkoffset(int offset_i, char *head){
return offset_i;
}
*/

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

	a = *(((char *)(argument->head)) + checkoffset(argument->counter, argument, 1));
//printf("%c", a);
		// end case:
		if (offset != (argument->blocksize -1)){
			b = *(((char *)(argument->head)) + checkoffset(argument->counter + 1, argument, 0));
/*			if ((((char *)(argument->head)) + checkoffset(argument->counter, argument, argument->curFile)) == (((char *)(argument->head)) + checkoffset(argument->counter + 1, (char*)(argument->head), argument->curFile))){
				b = *(((char *)(argument->head)) + checkoffset(argument->counter + 1, (char*)(argument->head), argument->curFile)+1);
			}
*/
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
/*
if (offset==4){
printf("...");
}
*/
	    argument->counter++;
//	    if (argument->counter != argument->blocksize){
//		offset = checkoffset(offset, (char *)(argument->head), argument->curFile);
//	    }
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


//file array

	ftable.filenum = argc - 1;


	int fd[argc - 1];
	size_t filesize[argc - 1];
	void *data[argc - 1];

	ftable.data = data;
	ftable.filesize = filesize;
 
	size_t totalfilesize = 0;
	for (int i = 1; i < argc; i++){

		//file discriptor:
		fd[i - 1] = open(argv[i], O_RDONLY);
	    	filesize[i - 1] = getfilesize(argv[i]);

		totalfilesize += filesize[i - 1];
		//0 is offset:
		data[i - 1] = mmap(NULL, filesize[i - 1], PROT_READ, MAP_SHARED | MAP_POPULATE, fd[i - 1], 0);

		if (data[i - 1] == MAP_FAILED){
			printf("my-zip: cannot open file\n");
			exit(1);
		}
//printf("data is %ld\n", (long int)data);
		close(fd[i - 1]);
	}


	
	int blockcalc = totalfilesize/NUMBER_OF_THREADS;
	int remainderBits = totalfilesize%NUMBER_OF_THREADS;



//1 is stdoutput
//		int trash = write(1, data, filesize);
//		printf("[trash is %d]\n", trash);
		
		//create threads:
		pthread_t ps[NUMBER_OF_THREADS];
		arg_t arguments[NUMBER_OF_THREADS];

//		int c = 0;
		int d = 0;
		size_t temp = 0;
		for(int j = 0; j < NUMBER_OF_THREADS; j++){

//printf("[%d]", j);
			arguments[j].blocksize = blockcalc;
			
			if (remainderBits != 0 && j == (NUMBER_OF_THREADS -1)){
				arguments[j].blocksize += remainderBits;
			}
//wrong for multi files!			
//			arguments[j].head = data + (j * (blockcalc));

//fix: find head:
			if (j == 0){		
			arguments[j].head = data[d];// + c * blockcalc;
			arguments[j].curFile = d;
			}
			else{
				arguments[j].head = arguments[j - 1].head + /*c **/ blockcalc;
				arguments[j].curFile = d;
				if (arguments[j].head >= (data[d] + filesize[d])){
//printf("WRONG!");
					temp = data[d] + filesize[d] - (arguments[j - 1].head);
					//data++;
					//filesize=filesize[j];
					d++;
					arguments[j].head = data[d] + (blockcalc - temp);
//					c = 1;
					arguments[j].curFile = d;
				}
			}
//			c++;
//head is found
			arguments[j].compressedSize = 0;
			arguments[j].idx = j;
			arguments[j].compressed = (char*)malloc(arguments[j].blocksize*sizeof(char)+1);
			arguments[j].compressedNums = (int*)malloc(arguments[j].blocksize*sizeof(int));
			arguments[j].counter = 0;

			pthread_create(&(ps[j]), NULL, compression, &(arguments[j]));
		}
		printEverything(ps, arguments);

		for (int i = 1; i < argc; i++){
			munmap(data[i - 1], filesize[i - 1]);

		}

//	}
	return 0;
}
