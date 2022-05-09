#include "stdlib.h"
#include "stdio.h"
#include "mapreduce.h"
#include "string.h"
#include "pthread.h"
/*
typedef struct lock_t{
	int flag;
} lock_t;
*/


typedef struct node{
	char* key;
	char* val;	
	struct node* next;
} node;

typedef struct partition{
	node* head;
	node* current;
	pthread_mutex_t mutex;
	//int filesMapped;
} partition;




// GLOBAL VARIABLES

// previously filer
	char* allFiles;
	char* nextFile;
	pthread_mutex_t fileLock;

// previously prtTable
	pthread_t* myThreads;
	//int* threadFiles;
	partition* prt;
	int filesMapped = 0;
	int totFiles;
	Mapper theMap;
	Reducer theReduce;
	Partitioner thePartition;
	int theNumReducers;

void
MR_Emit (char * key, char * value)
{
	unsigned long prtNum;
	node* theNode = (node*)malloc(sizeof(node));
	node* searcher = NULL;
	theNode.key = key;
	theNode.val = val;
	int temp = 0;
    // FILL ME IN
	if( thePartition == NULL){
		// use default
		prtNum = MR_DefaultHashPartition(key, theNumReducers);
	} else {
		// use given partitioner function
		prtNum = thePartition(key, theNumReducers);
	}
	
	if(prt[prtNum].head == NULL){
		// if there is nothing else in the partition yet
		prt[prtNum].head = theNode;
		prt[prtNum].head.next = NULL;
	} else {
		// compare to all other nodes an put into correct location
		searcher = prt[prtNum].head;
		while (searcher != NULL){
			temp = strcmp(theNode.key, searcher.key)
			if (temp == 0){
				
			} else if (temp < 0){ // first one is a second is b, place node before
				// need a previous to get this stuff working

			} else {
				
			}
		}
	}
}


unsigned long
MR_DefaultHashPartition (char * key, int num_partitions)
{
    // FILL ME IN 
    // return -1;
	unsigned long hash = 5381;
	int c;
	while ((c = *key++) != '\0'){
		hash = hash * 33 + c;
	}
	return hash % num_partitions;
}


void
table_init(int num_reducers)
{
	// malloc our large table container
	//table = (prtTable*)malloc(sizeof(prtTable));
	// for the number of files each mapper thread should get
	/*
	table->threadFiles = (int*)malloc(sizeof(int)*num_mappers);
	for (int i = 0; i < num_mappers; i++){
		table->threadFiles[i] = numFiles/num_mappers;
	}
	if ( (numFiles%num_mappers) != 0){
		int extra = numFiles%num_mappers;
		for (int i = 0; i < extra; i++){
			table->threadFiles[i]++;
		}
	}
	*/
	// malloc each partition in the array
	prt = (partition*)malloc(num_reducers*sizeof(partition));

	// initialize head and current nodes and a lock for each partition
	for (int i = 0; i < num_reducers; i++){
		prt[i].head = NULL;
		prt[i].current = NULL;
		// initialize the lock
		//table->prt[i].lock_t = 0;
		
		int tester = pthread_mutex_init(&(prt[i].mutex), NULL);
		if (tester != 0){
			printf("didn't work \n");
		}
		printf("prt lock init\n");
	}
	
	//table->filesMapped = 0;
}

void
multiMapper()
{
	// for the number of files assigned to this thread, run map on the current file pointed to. lock the filer when picking the file and moving current.
	// increment the number of filesMapped and actually just whiteboard this so you know what you're doing
	//int potatoTest = 5;
	pthread_mutex_lock(&fileLock);
	int fileNum = filesMapped;
	filesMapped++;
	char* theFile = nextFile;
	nextFile++;
	pthread_mutex_unlock(&fileLock);
	
	//pthread_mutex_lock(
	while (filesMapped < totFiles){
		printf("%s",theFile);
		theMap(theFile);
		
		pthread_mutex_lock(&fileLock);
		fileNum = filesMapped;
		filesMapped++;
		theFile = nextFile;
		nextFile++;
		pthread_mutex_unlock(&fileLock);
	}
}

void
MR_Run (int argc, char * argv[], Mapper map, int num_mappers, Reducer reduce, int num_reducers, Partitioner partition)
{
    // FILL ME IN
	
	// map_init should initialize num_reducers number of partitions in an array
	
	//prtTable* myTable;
	totFiles = argc - 1;
	theNumReducers = num_reducers;
	table_init(num_reducers);
	printf("table initialized \n");
	theMap = map;
	theReduce = reduce;
	thePartition = partition;
	// bad practice but part of the table (myThreads) is initialized later


	// save the files and initialize the filer struct
	//int fd[argc - 1];

	//struct filer* myFiles;
	//myFiles = (struct filer*)malloc(sizeof(filer));
	allFiles = argv[1];
	nextFile = allFiles;
	// fileLock = malloc(sizeof(fileLock));
	printf("%s\n", allFiles);
	printf("initing filelock\n");

	int tester = pthread_mutex_init(&fileLock, NULL);
	if (tester != 0){
		printf("didn't work \n");
	}
	printf("done\n");
	
/*	
	for (int i = 1; i < argc; i++){
		// permission to read or write because idk what the user's map and reduce will do?
		fd[i - 1] = open(argv[i], O_RDWR);
		// a file fails to open
		if (fd[i - 1] == -1){
			printf("mapreduce: cannot open file\n");
			return -1;
		}
	}
*/	
	//myTable->theMap = &map;
	//myTable->theReduce = &reduce;
	// creating threads
	pthread_t ps[num_mappers];
	myThreads = ps;
	
	// if the number of files is equal to the number of mappers
	if (num_mappers == (argc-1)){
		for (int j = 0; j < num_mappers; j++){
			tester = pthread_create(&(ps[j]), NULL, (void*)multiMapper, NULL);
			if (tester != 0){
				printf("didn't work \n");
			}
			printf("mapping case 1\n");
		}
		for (int k = 0; k < num_mappers; k++){
			pthread_join(ps[k], NULL);
		}
	} else if (num_mappers > totFiles){
	// if the number of files is less than the number of mappers
		for (int j = 0; j < totFiles; j++ ){
			tester = pthread_create(&(ps[j]), NULL, (void*)multiMapper, NULL);
			if (tester != 0){
				printf("didn't work \n");
			}
			printf("mapping case 2\n");
		}
		for (int k = 0; k < totFiles; k++){
			pthread_join(ps[k], NULL);
		}

	} else {
	// if thw numbwe of files is greater than the number of mappers
		//int j = 0;
		for (int j = 0; j < num_mappers; j++){
			tester = pthread_create(&(ps[j]), NULL, (void*)multiMapper, NULL);
			if (tester != 0){
				printf("didn't work \n");
			}
			printf("mapping case 3\n");
		}
		for (int k = 0; k < num_mappers; k++){
			pthread_join(ps[k], NULL);
		}

		/*
		for (;j < (argc - 1); j++){
			// make the other threads map more than one file
		}*/
	}
}
