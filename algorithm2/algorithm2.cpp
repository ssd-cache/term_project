// algorithm2.cpp : Defines the entry point for the console application.
//
// algorithm1.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "algorithm2.h"
#include "cache_ssd.h"
#include "StringManip.h"
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>


// FOR HARDCODED SIZES - STUPID!!!
void algorithm2(char *sim_file, char *output)
{
	// local variables
	InfoString *temp;
	LB_Clock* lb_clock = NULL;
	char tempInput[1024];

	// simple print information about the input/ output & algo
    printf("this is the Long Block Clock (LB-Clock) algorithm\n");
	// chi's test stuff
	printf("using the test file %s\n", sim_file);

	// initializing struct
	lb_clock = initClock();

	
	// setting sizes for struct of clock
	lb_clock->cacheSize.size = CACHE;
	if(!(lb_clock->cacheSize.type = (char*)malloc(4)))
	{
		printf("couldn't allocated memory for cache size type, exiting...");
		exit(-1);
	}
	strcpy(lb_clock->cacheSize.type, "mb");

	lb_clock->blockSize.size = BLOCK_SIZE;
	if(!(lb_clock->blockSize.type = (char*)malloc(4)))
	{
		printf("couldn't allocated memory for block size type, exiting...");
		exit(-1);
	}
	strcpy(lb_clock->blockSize.type, "kb");

	lb_clock->pageSize.size = PAGE_SIZE;
	if(!(lb_clock->pageSize.type = (char*)malloc(4)))
	{
		printf("couldn't allocated memory for page size type, exiting...");
		exit(-1);
	}
	strcpy(lb_clock->pageSize.type, "kb");


	// set number of blocks in cache and number of pages in blocks
	setBlockOfCache(lb_clock);
	setPageOfBlocks(lb_clock);

	// parse the file
	traceParser(sim_file, output, lb_clock);

	// output file
	printf("output the result to %s\n", output);

	// freeing memory
	free(lb_clock->blockSize.type);
	free(lb_clock->cacheSize.type);
	free(lb_clock->pageSize.type);
	free(lb_clock);
}

// FOR HARDCODED SIZES - STUPID!!!
void algorithm2_5(char *sim_file, char *output)
{
	// local variables
	InfoString *temp;
	LB_Clock* lb_write = NULL;
	LB_Clock* lb_read = NULL;
	char tempInput[1024];

	// simple print information about the input/ output & algo
    printf("this is the Long Block Clock (LB-Clock) algorithm Version 2\n");
	// chi's test stuff
	printf("using the test file %s\n", sim_file);

	// initializing struct
	lb_write = initClock();
	lb_read = initClock();

	
	// setting sizes for WRITE clock
	lb_write->cacheSize.size = CACHE / 2;
	if(!(lb_write->cacheSize.type = (char*)malloc(sizeof(4))))
	{
		printf("couldn't allocated memory for cache size type, exiting...");
		exit(-1);
	}
	strcpy(lb_write->cacheSize.type, "mb");

	lb_write->blockSize.size = BLOCK_SIZE;
	if(!(lb_write->blockSize.type = (char*)malloc(sizeof(4))))
	{
		printf("couldn't allocated memory for block size type, exiting...");
		exit(-1);
	}
	strcpy(lb_write->blockSize.type, "kb");

	lb_write->pageSize.size = PAGE_SIZE;
	if(!(lb_write->pageSize.type = (char*)malloc(sizeof(4))))
	{
		printf("couldn't allocated memory for page size type, exiting...");
		exit(-1);
	}
	strcpy(lb_write->pageSize.type, "kb");

	// setting sizes for READ clock
	lb_read->cacheSize.size = CACHE / 2;
	if(!(lb_read->cacheSize.type = (char*)malloc(sizeof(4))))
	{
		printf("couldn't allocated memory for cache size type, exiting...");
		exit(-1);
	}
	strcpy(lb_read->cacheSize.type, "mb");

	lb_read->blockSize.size = BLOCK_SIZE;
	if(!(lb_read->blockSize.type = (char*)malloc(sizeof(4))))
	{
		printf("couldn't allocated memory for block size type, exiting...");
		exit(-1);
	}
	strcpy(lb_read->blockSize.type, "lb");

	lb_read->pageSize.size = PAGE_SIZE;
	if(!(lb_read->pageSize.type = (char*)malloc(sizeof(4))))
	{
		printf("couldn't allocated memory for page size type, exiting...");
		exit(-1);
	}
	strcpy(lb_read->pageSize.type, "kb");


	// set number of blocks in cache and number of pages in blocks
	setBlockOfCache(lb_write);
	setPageOfBlocks(lb_write);
	setBlockOfCache(lb_read);
	setPageOfBlocks(lb_read);

	// parse the file
	traceParser(sim_file, output, lb_write);

	// output
	 printf("output the result to %s\n", output);

	// freeing write
	free(lb_write->blockSize.type);
	free(lb_write->cacheSize.type);
	free(lb_write->pageSize.type);
	free(lb_write);
	// read clock
	free(lb_read->blockSize.type);
	free(lb_read->cacheSize.type);
	free(lb_read->pageSize.type);
	free(lb_read);
}

void algorithm2(char *sim_file, char *output, void *arg)
{
	// local variables
	SizeInfo *size = (SizeInfo*)arg;
	InfoString *temp;
	LB_Clock* lb_clock = NULL;
	char tempInput[1024];

	// simple print information about the input/ output & algo
    printf("this is the Long Block Clock (LB-Clock) algorithm\n");
	// chi's test stuff
	printf("using the test file %s\n", sim_file);

	// initializing struct
	lb_clock = initClock();

	// copying to struct in my code base
	temp = (InfoString*)malloc(sizeof(InfoString));
	temp->size = size->size;
	temp->type = (char*)malloc(strlen(size->type)+1);
	strcpy(temp->type, size->type);
	
	// setting sizes for struct of clock
	lb_clock->cacheSize.size = size[0].size;
	if(!(lb_clock->cacheSize.type = (char*)malloc(strlen(size[0].type)+1)))
	{
		printf("couldn't allocated memory for cache size type, exiting...");
		exit(-1);
	}
	strcpy(lb_clock->cacheSize.type, size[0].type);

	lb_clock->blockSize.size = size[1].size;
	if(!(lb_clock->blockSize.type = (char*)malloc(strlen(size[1].type)+1)))
	{
		printf("couldn't allocated memory for block size type, exiting...");
		exit(-1);
	}
	strcpy(lb_clock->blockSize.type, size[1].type);

	lb_clock->pageSize.size = size[2].size;
	if(!(lb_clock->pageSize.type = (char*)malloc(strlen(size[2].type)+1)))
	{
		printf("couldn't allocated memory for page size type, exiting...");
		exit(-1);
	}
	strcpy(lb_clock->pageSize.type, size[2].type);


	// set number of blocks in cache and number of pages in blocks
	setBlockOfCache(lb_clock);
	setPageOfBlocks(lb_clock);

	// parse the file
	traceParser(sim_file, output, lb_clock);

	// output
	 printf("output the result to %s\n", output);

	// freeing memory
	free(temp->type);
	free(temp);
	free(lb_clock->blockSize.type);
	free(lb_clock->cacheSize.type);
	free(lb_clock->pageSize.type);
	free(lb_clock);
}

void algorithm2_5(char *sim_file, char *output, void *arg)
{
	// local variables
	SizeInfo *size = (SizeInfo*)arg;
	InfoString *temp;
	LB_Clock* lb_write = NULL;
	LB_Clock* lb_read = NULL;
	char tempInput[1024];

	// simple print information about the input/ output & algo
    printf("this is the Long Block Clock (LB-Clock) algorithm Version 2\n");
	// chi's test stuff
	printf("using the test file %s\n", sim_file);

	// initializing struct
	lb_write = initClock();
	lb_read = initClock();

	// copying to struct in my code base
	temp = (InfoString*)malloc(sizeof(InfoString));
	temp->size = size->size;
	temp->type = (char*)malloc(strlen(size->type)+1);
	strcpy(temp->type, size->type);
	
	// setting sizes for WRITE clock
	lb_write->cacheSize.size = size[0].size;
	if(!(lb_write->cacheSize.type = (char*)malloc(strlen(size[0].type)+1)))
	{
		printf("couldn't allocated memory for cache size type, exiting...");
		exit(-1);
	}
	strcpy(lb_write->cacheSize.type, size[0].type);

	lb_write->blockSize.size = size[1].size;
	if(!(lb_write->blockSize.type = (char*)malloc(strlen(size[1].type)+1)))
	{
		printf("couldn't allocated memory for block size type, exiting...");
		exit(-1);
	}
	strcpy(lb_write->blockSize.type, size[1].type);

	lb_write->pageSize.size = size[2].size;
	if(!(lb_write->pageSize.type = (char*)malloc(strlen(size[2].type)+1)))
	{
		printf("couldn't allocated memory for page size type, exiting...");
		exit(-1);
	}
	strcpy(lb_write->pageSize.type, size[2].type);

	// setting sizes for READ clock
	lb_read->cacheSize.size = size[0].size / 2;
	if(!(lb_read->cacheSize.type = (char*)malloc(strlen(size[0].type)+1)))
	{
		printf("couldn't allocated memory for cache size type, exiting...");
		exit(-1);
	}
	strcpy(lb_read->cacheSize.type, size[0].type);

	lb_read->blockSize.size = size[1].size;
	if(!(lb_read->blockSize.type = (char*)malloc(strlen(size[1].type)+1)))
	{
		printf("couldn't allocated memory for block size type, exiting...");
		exit(-1);
	}
	strcpy(lb_read->blockSize.type, size[1].type);

	lb_read->pageSize.size = size[2].size;
	if(!(lb_read->pageSize.type = (char*)malloc(strlen(size[2].type)+1)))
	{
		printf("couldn't allocated memory for page size type, exiting...");
		exit(-1);
	}
	strcpy(lb_read->pageSize.type, size[2].type);


	// set number of blocks in cache and number of pages in blocks
	setBlockOfCache(lb_write);
	setPageOfBlocks(lb_write);
	setBlockOfCache(lb_read);
	setPageOfBlocks(lb_read);

	// parse the file
	traceParser(sim_file, output, lb_write);

	// output
	 printf("output the result to %s\n", output);

	// freeing write
	free(temp->type);
	free(temp);
	free(lb_write->blockSize.type);
	free(lb_write->cacheSize.type);
	free(lb_write->pageSize.type);
	free(lb_write);
	// read clock
	free(lb_read->blockSize.type);
	free(lb_read->cacheSize.type);
	free(lb_read->pageSize.type);
	free(lb_read);
}

LB_Clock* initClock()
{
	// variables
	clock_t t = clock(); // move to a different spot so intial tick comparison isn't so large

	// inti struct
	LB_Clock *clock = (LB_Clock*)malloc(sizeof(LB_Clock));
	if(clock == NULL)
	{
		printf("Couldn't allocate memory for LB-Clock structure...\n");
		exit(-1);
	}
	else
	{
		// intializing circular linked list dummy node
		if((clock->clockStart = (LB_Node*)malloc(sizeof(LB_Node))) == NULL)
		{
			printf("couldn't allocate memory for clock list, exiting...");
			exit(-1);
		}
		clock->clockStart = NULL;

		// setting clock hand pointer to start of clock
		clock->clockHand = clock->clockStart;

		// setting intial values of clock struct
		clock->blockSize.size = 0;
		clock->cacheSize.size = 0;
		clock->pageSize.size = 0;
		clock->blockPerCache = 0;
		clock->pagePerBlock = 0;
		clock->currBlockCount = 0;
		clock->pageFaults = 0;
		clock->cacheHit = 0;
		clock->timeStamp = 0;
		clock->timer = t;
	}
}

//void setStructSizes(InfoString *size, LB_Clock *lb_clock)
//{
//
//}

void setBlockOfCache(LB_Clock *lb_clock)
{
	// variables
	unsigned long tempCache;
	unsigned long tempBlock;

	// check if sizes are the same type if so calculate, if not conversion time
	if(strcmp(lb_clock->cacheSize.type, lb_clock->blockSize.type) == 0)
	{
		lb_clock->blockPerCache = lb_clock->cacheSize.size / lb_clock->blockSize.size;
	}
	else
	{
		tempCache = getByteValue(&lb_clock->cacheSize);
		tempBlock = getByteValue(&lb_clock->blockSize);
		lb_clock->blockPerCache = tempCache/tempBlock;
	}
	return;
}

void setPageOfBlocks(LB_Clock *lb_clock)
{
	// variables
	int tempBlock;
	int tempPage;

	// check if sizes are the same type if so calculate, if not conversion time
	if(strcmp(lb_clock->blockSize.type, lb_clock->pageSize.type) == 0)
	{
		lb_clock->pagePerBlock = lb_clock->blockSize.size / lb_clock->pageSize.size;
	}
	else
	{
		tempBlock = (int)getByteValue(&lb_clock->blockSize);
		tempPage = (int)getByteValue(&lb_clock->pageSize);
		lb_clock->pagePerBlock = tempBlock/tempPage;
	}
	return;
}

// checking size of write or read in page granularity
int getNumOfPages(Trace *tp, LB_Clock *lb_clock)
{
	// variables
	SizeInfo temp;
	int tempPages;
	int tempBytes1 = 0;
	int tempBytes2 = 0;

	//split size of read or write into page granularity
	temp.size = tp->size;
	if(!(temp.type = (char*)malloc(strlen(lb_clock->pageSize.type)+1)))
	{
		printf("couldn't allocate string for temp SizeInfo, exiting...");
		exit(-1);
	}
	strcpy(temp.type, "b");
	tempBytes1 = (int)getByteValue(&temp);
	tempBytes2 = (int)getByteValue(&lb_clock->pageSize);

	// check for uneven divisions here
	tempPages = tempBytes1 / tempBytes2;
	if(tempBytes1 % tempBytes2 > 1)
	{
		tempPages += 1;
	}
	// free temp allocated string
	free(temp.type);

	return tempPages;
}

int checkPageSize(Trace *tp, LB_Clock *lb_clock, LB_Node *np)
{
	// variables
	int pagesNeeded = getNumOfPages(tp, lb_clock);
	int overage = 0;

	if(pagesNeeded > lb_clock->pagePerBlock - np->currNumOfPages)
	{
		overage = pagesNeeded;
	}
	return overage;
}

LB_Node* checkPagesInBlock(LB_Clock *lb_clock)
{
	// variables
	LB_Node *tempNode = NULL;

	if(lb_clock->clockStart != NULL)
	{
		tempNode = lb_clock->clockStart;

		while(tempNode->next != lb_clock->clockStart)
		{
			if(tempNode->currNumOfPages < lb_clock->pagePerBlock)
			{
				break;
			}
			tempNode = tempNode->next;
		}
	}
	return tempNode;
}

// think of changing function name to check page in cache
int checkBlockInCache(Trace *tp, LB_Clock *lb_clock)
{
	// variables
	int found = 0;
	LB_Node *currNode;

	currNode = lb_clock->clockStart;
	if(currNode != NULL)
	{
		while(currNode->next != lb_clock->clockStart)
		{
			// added this to loop through filled pages of cache block
			for(int i=0; i<currNode->currNumOfPages; i++) 
			{
				if(currNode->pageTable[i] == atoi(tp->addr)/lb_clock->pagePerBlock)
				{
					// could have issues with this if size overflows to second block
					found = 1;
				}
			}
			currNode = currNode->next;
		}
	}
	return found;
}

// writing to cache
//void writeToCache(Trace *tp, LB_Clock *lb_clock)// change to write cache block
void writeCacheBlock(Trace *tp, LB_Clock *lb_clock)
{
	// variables
	LB_Node *insertNode, *currNode;
	

	// safety check so we don't run out of memory
	if((insertNode = (LB_Node*)malloc(sizeof(LB_Node))) == NULL)
	{
		printf("can't malloc that many nodes, exiting...");
		exit(-1);
	}
	// allocating page table array
	if(!(insertNode->pageTable = (int*)malloc(lb_clock->pagePerBlock * sizeof(int))))
	{
		printf("couldn't allocate memory for page table array, exiting...");
		exit(-1);
	}
	insertNode->blockNum = lb_clock->currBlockCount;
	insertNode->currNumOfPages = 0;
	//test adding pages to cache
	addPagesToCache(tp, lb_clock, insertNode);
	insertNode->refBit = 1;

	//testing for where to insert new node
	if(lb_clock->currBlockCount < lb_clock->blockPerCache)
	{
		if(lb_clock->clockStart == NULL)
		{
			lb_clock->clockStart = insertNode;
			insertNode->next = lb_clock->clockStart;
			lb_clock->clockHand = lb_clock->clockStart;
			//keeping count of number of current blocks in list
			lb_clock->currBlockCount++;
		}
		else
		{
			currNode = lb_clock->clockStart;
			if (currNode == lb_clock->clockStart->next)
			{
				lb_clock->clockStart->next = insertNode;
				insertNode->next = lb_clock->clockStart; 
				//keeping count of number of current blocks in list
				lb_clock->currBlockCount++;
			}
			else 
			{
				while(currNode->next != lb_clock->clockStart)
				{
					currNode = currNode->next;
				}
				currNode->next = insertNode;
				insertNode->next = lb_clock->clockStart;
				//keeping count of number of current blocks in list
				lb_clock->currBlockCount++;
			}
		}
		printf("cache miss = %d, block number = %d\n", lb_clock->pageFaults, insertNode->blockNum);
	}
	else
	{
		// here is where you evict block
		evictBlockFromCache(tp, lb_clock, insertNode);
		// need to make currNode blockNum == evitBlock blockNum
	}
	return;
}

void updateBlockInCache(Trace *tp, LB_Clock *lb_clock)
{
	// variables
	LB_Node *currNode;
	int pageNumCheck;
	// need a data transfer time to add to timeStamp

	currNode = lb_clock->clockStart;
	if(currNode != NULL)
	{
		while(currNode->next != lb_clock->clockStart)
		{
			for(int i=0; i<currNode->currNumOfPages; i++)
			{
				if(currNode->pageTable[i] == atoi(tp->addr)/lb_clock->pagePerBlock)
				{
					pageNumCheck = getNumOfPages(tp, lb_clock);
					if(pageNumCheck == currNode->currNumOfPages)
					{
						currNode->refBit = 1;
						printf("cache hit = %d, page address = %d\n", lb_clock->cacheHit, currNode->pageTable[i]);
						break;
					}
					else
					{
						// dirty needs to be written back to disk
						printf("cache hit = %d, page address = %d\n",lb_clock->cacheHit, currNode->pageTable[i]);
						currNode->refBit = 1;
						pageNumCheck = checkPageSize(tp, lb_clock, currNode);
						if(currNode->currNumOfPages < lb_clock->pagePerBlock && pageNumCheck == 0)
						{
							addPagesToCache(tp, lb_clock, currNode);
							// adding to time stamp
							lb_clock->timeStamp += (DATA_TRANS + MEM_ACCESS_LATE);
							break;
							//goto End;
						}
						else
						{
							movePagesToNewBlock(tp, lb_clock, currNode);
							// adding to time stamp
							lb_clock->timeStamp += (DATA_TRANS + MEM_ACCESS_LATE);
							break;
							//goto End;
						}
						//lb_clock->timeStamp += dataTransTime;
					}
				}
			}
			currNode = currNode->next;
		}
	}
	//End:
	return;
}

void movePagesToNewBlock(Trace *tp, LB_Clock *lb_clock, LB_Node *currNode)
{
	// variables
	int pages;
	LB_Node *moveNode = lb_clock->clockStart;

	while(moveNode->next != lb_clock->clockStart)
	{
		pages = checkPageSize(tp, lb_clock, moveNode);
		if(moveNode->currNumOfPages < lb_clock->pagePerBlock && pages == 0)
		{
			break;
		}
		moveNode = moveNode->next;
	}

	
	if(lb_clock->currBlockCount == lb_clock->blockPerCache && moveNode->next == lb_clock->clockStart)
	{
		pages = checkPageSize(tp, lb_clock, moveNode);
		if(pages > 0)
		{
			writeCacheBlock(tp, lb_clock);
			moveNode = NULL;
		}
	}

	for(int i=0; i<currNode->currNumOfPages; i++)
	{
		if(currNode->pageTable[i] == atoi(tp->addr)/lb_clock->pagePerBlock)
		{
			currNode->pageTable[i] = -1;
			//currNode->currNumOfPages--;
		}
	}

	if(moveNode != NULL)
	{
		addPagesToCache(tp, lb_clock, moveNode);
	}
}

void addPagesToCache(Trace *tp, LB_Clock *lb_clock, LB_Node *np)
{
	// variables
	int numOfPagesNeeded;
	int cacheAddr;

	cacheAddr = atoi(tp->addr)/lb_clock->pagePerBlock;
	// checking for number of pages needed for the block
	numOfPagesNeeded = getNumOfPages(tp, lb_clock);

	// functions
	for(int i=np->currNumOfPages; i<numOfPagesNeeded+np->currNumOfPages; i++)
	{
		np->pageTable[i] = cacheAddr;
		// might want to do add MEM_ACCESS_LATE
	}
	np->currNumOfPages += numOfPagesNeeded;

	// updateing time stamp
	lb_clock->timeStamp += PAGE_WRITE;
	return;
}

void readFromCache(Trace *tp, LB_Clock *lb_clock)
{
	// variables
	LB_Node *currNode;
	// need a hit time to add to timer

	currNode = lb_clock->clockStart;
	if(currNode != NULL)
	{
		while(currNode->next != lb_clock->clockStart)
		{
			for(int i=0; i<lb_clock->currBlockCount; i++)
			{
				if(currNode->pageTable[i] == atoi(tp->addr)/lb_clock->pagePerBlock)
				{
					currNode->refBit = 1;
					//lb_clock->timeStamp += hitTime;
					printf("cache hit = %d, page address = %d\n", lb_clock->cacheHit, currNode->pageTable[i]);
					lb_clock->timeStamp += (PAGE_READ + MEM_ACCESS_LATE);
					break;
				}
			}
			currNode = currNode->next;
		}
	}
	return;
}

void evictBlockFromCache(Trace *tp, LB_Clock *lb_clock, LB_Node *newNode)
{
	// variables
	LB_Node *evictNode, *lastNode;

	// functions
	if(strcmp(tp->mode, "w") == 0)
	{
		evictNode = checkVictimSetWrite(lb_clock);
	}
	else
	{
		evictNode = checkVictimSetRead(lb_clock);
	}
	if(evictNode == lb_clock->clockStart)
	{
		lastNode = evictNode;
		while(lastNode->next != lb_clock->clockStart)
		{
			lastNode = lastNode->next;
		}
		newNode->blockNum = evictNode->blockNum;
		newNode->next = evictNode->next;
		lastNode->next = newNode;
		lb_clock->clockStart = newNode;
		if(evictNode == lb_clock->clockHand)
		{
			lb_clock->clockHand = newNode;
		}
	}
	else
	{
		lastNode = lb_clock->clockStart;
		while(lastNode->next != evictNode)
		{
			lastNode = lastNode->next;
		}
		newNode->blockNum = evictNode->blockNum;
		newNode->next = evictNode->next;
		lastNode->next = newNode;
		if(evictNode == lb_clock->clockHand)
		{
			lb_clock->clockHand = newNode;
		}
	}

	// freeing evicted block
	free(evictNode->pageTable);
    free(evictNode);

	// adding to time stamp
	lb_clock->timeStamp += BLOCK_ERASE;
	return;
}

void checkTraceMode(Trace *tp, LB_Clock *lb_clock)
{
	// variables
	LB_Node *tempNode;
	int testNum;

	if(strcmp(tp->mode, "w") == 0)
	{
		// check if in cache already
		// if not
		if(checkBlockInCache(tp, lb_clock) != 1)
		{
			tempNode = checkPagesInBlock(lb_clock);
			if(tempNode != NULL)
			{
				testNum = checkPageSize(tp, lb_clock, tempNode);
			}
			if(tempNode != NULL && tempNode->currNumOfPages < lb_clock->pagePerBlock && testNum == 0)
			{
				addPagesToCache(tp, lb_clock, tempNode);
				lb_clock->pageFaults++;
			}
			else
			{
				//OverageCheck:
				writeCacheBlock(tp, lb_clock);
				lb_clock->pageFaults++;
			}
		}
		else
		{
			updateBlockInCache(tp, lb_clock);
			lb_clock->cacheHit++;
		}
	}
	else if(strcmp(tp->mode, "r") == 0)
	{
		// check if in cache already
		//if is
		if(checkBlockInCache(tp, lb_clock) != 1)
		{
			tempNode = checkPagesInBlock(lb_clock);
			if(tempNode != NULL)
			{
				testNum = checkPageSize(tp, lb_clock, tempNode);
			}
			if(tempNode != NULL && tempNode->currNumOfPages < lb_clock->pagePerBlock && testNum == 0)
			{
			
				addPagesToCache(tp, lb_clock, tempNode);
				lb_clock->pageFaults++;
			}
			else
			{
				writeCacheBlock(tp, lb_clock);// change to write cache block
				lb_clock->pageFaults++;
			}
		}
		else
		{
			readFromCache(tp, lb_clock);
			lb_clock->cacheHit++;
		}
	}
	return;
}

LB_Node* checkVictimSetWrite(LB_Clock *lb_clock)
{
	// variables
	int evictBlock;
	LB_Node *evictNode, *currNode, *nextNode;
	int reset;

	// so there is always something to evict
	evictNode = lb_clock->clockHand;
	currNode = lb_clock->clockHand;
	nextNode = currNode->next;

	while(currNode->next != lb_clock->clockHand)
	{
		// think about logic here.....test
		if(nextNode != NULL && currNode->currNumOfPages >= nextNode->currNumOfPages)
		{
			if(currNode->refBit == 0 && currNode->currNumOfPages == lb_clock->pagePerBlock)
			{
				evictNode = currNode;
				break;
			}
		}
		currNode = currNode->next;
	}
	lb_clock->clockHand = lb_clock->clockHand->next;
	return evictNode;
}

LB_Node* checkVictimSetRead(LB_Clock *lb_clock)
{
	// variables
	int evictBlock;
	LB_Node *evictNode, *currNode, *nextNode;
	int reset;

	// so theres always someone to evict
	evictNode = lb_clock->clockHand;
	currNode = lb_clock->clockHand;
	nextNode = currNode->next;

	while(currNode->next != lb_clock->clockHand)
	{
		// think about logic here.....test
		if(nextNode != NULL && currNode->currNumOfPages <= nextNode->currNumOfPages)
		{
			if(currNode->refBit == 0 && currNode->currNumOfPages <= lb_clock->pagePerBlock)
			{
				evictNode = currNode;
				break;
			}
		}
		currNode = currNode->next;
	}
	lb_clock->clockHand = lb_clock->clockHand->next;
	return evictNode;
}

void checkTicks(LB_Clock *lb_clock)
{
	//variables
	clock_t t;
	int currTime;
	int ticks;

	t = clock();
	currTime = t;
	ticks = currTime - lb_clock->timer;
	if(ticks >= 3)
	{
		lb_clock->timer = currTime;
		// reset ref bits in  clock
		updateRefBit(lb_clock);
	}
	//printf("\ntesting clock tick as timer = %d\n\n", ticks);
	return;
}

int updateRefBit(LB_Clock *lb_clock)
{
	// variables
	LB_Node *currNode;
	int updated = 0;
	int i = 0;

	// functions
	currNode = lb_clock->clockHand;

	if(currNode != NULL)
	{
		while(currNode->next != lb_clock->clockStart)
		{
			if(currNode->refBit == 1)
			{
				currNode->refBit = 0;
				//lb_clock->victimCandidateSet[i++] = currNode->blockNum;
				updated = 1;
			}
			currNode = currNode->next;
		}
		currNode->refBit == 0;
	}
	return updated;
}

///////////////////////////////////////////
//	parser from chi's algorithm 1 code	//
/////////////////////////////////////////
void traceParser(char *trace_file, char* result, LB_Clock *lb)
{
	FILE * trace_f;
	int i;
	int page_idx;
	char *p;
	char *array[5];
	Trace sample;
	output_entry output;
	int cnt = 0;
	trace_f = fopen(trace_file, "r");
	if (trace_f != NULL)
	{
		char line[128];
		while (fgets(line, sizeof(line), trace_f) != NULL)
		{
			i = 0;
			
			//fputs(line, stdout);
			//split the line into the struct
			p = strtok(line, ",");
			while (p != NULL)
			{
				array[i++] = p;
				p = strtok(NULL, ",");
			}			
			sample.addr = array[0];
			sample.size = atoi(array[1]);
			sample.mode = array[2];
			lowerCaseString(sample.mode);
			sample.time_stamp = atof(array[3]);
			checkTraceMode(&sample, lb);
			checkTicks(lb);
			output.time_stamp = sample.time_stamp;
			output.page_fault = lb->pageFaults;
			output.hit_rate = lb->cacheHit;
			output_helper(output, result);
		}
		fclose(trace_f);
		// write overall time stamp to output file
		output.hit_rate = 0;
		output.page_fault = 0;
		output.time_stamp = lb->timeStamp;
		output_helper(output, result);
	}
	return;
}

void traceParser2_5(char *trace_file, char* result, LB_Clock *lb_r, LB_Clock* lb_w)
{
	FILE * trace_f;
	int i;
	int page_idx;
	char *p;
	char *array[5];
	Trace sample;
	output_entry output;
	int cnt = 0;
	trace_f = fopen(trace_file, "r");
	if (trace_f != NULL)
	{
		char line[128];
		while (fgets(line, sizeof(line), trace_f) != NULL)
		{
			i = 0;
			
			//fputs(line, stdout);
			//split the line into the struct
			p = strtok(line, ",");
			while (p != NULL)
			{
				array[i++] = p;
				p = strtok(NULL, ",");
			}			
			sample.addr = array[0];
			sample.size = atoi(array[1]);
			sample.mode = array[2];
			sample.time_stamp = atof(array[3]);
			if(strcmp(sample.mode, "w") == 0)
			{
				checkTraceMode(&sample, lb_w);
				checkTicks(lb_w);
				output.page_fault = lb_w->pageFaults;
				output.hit_rate = lb_w->cacheHit;
				output_helper(output, result);
			}
			else
			{
				checkTraceMode(&sample, lb_r);
				checkTicks(lb_r);
				output.page_fault = lb_r->pageFaults;
				output.hit_rate = lb_r->cacheHit;
				output_helper(output, result);
			}
			output.time_stamp = sample.time_stamp;

		}
		fclose(trace_f);
				// write overall time stamp to output file
		output.hit_rate = 0;
		output.page_fault = 0;
		output.time_stamp = lb_w->timeStamp;
		output_helper(output, result);
		output.time_stamp = lb_r->timeStamp;
		output_helper(output, result);
	}
	return;
}





