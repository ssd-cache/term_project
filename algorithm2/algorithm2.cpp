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


void algorithm2(char *sim_file, char *output, void *arg)
{
	// local variables
	SizeInfo *size = (SizeInfo*)arg;
	InfoString *temp;
	LB_Clock* lb_clock = NULL;
	FILE *traceFile;
	char tempInput[1024];

	// simple print information about the input/ output & algo
    printf("this is the Long Block Clock (LB-Clock) algorithm\n");
	// chi's test stuff
	printf("using the test file %s\n", sim_file);
    printf("output the result to %s\n", output);

	// initializing struct
	lb_clock = initClock();

	// copying to struct in my code base
	temp = (InfoString*)malloc(sizeof(InfoString));
	temp->size = size->size;
	temp->type = (char*)malloc(strlen(size->type)+1);
	strcpy(temp->type, size->type);
	
	// setting sizes for struct of clock
	// setting sizes (MOVE THIS TO ITS OWN FUNCTION)
	lb_clock->cacheSize.size = size[0].size;
	lb_clock->cacheSize.type = (char*)malloc(strlen(size[0].type)+1);
	// add safety checks here
	strcpy(lb_clock->cacheSize.type, size[0].type);

	lb_clock->blockSize.size = size[1].size;
	lb_clock->blockSize.type = (char*)malloc(strlen(size[1].type)+1);
	// add safety checks here
	strcpy(lb_clock->blockSize.type, size[1].type);

	lb_clock->pageSize.size = size[2].size;
	lb_clock->pageSize.type = (char*)malloc(strlen(size[2].type)+1);
	// add safety checks here
	strcpy(lb_clock->pageSize.type, size[2].type);


	// set number of blocks in cache and number of pages in blocks
	setBlockOfCache(lb_clock);
	setPageOfBlocks(lb_clock);

	// parse the file
	traceParser(sim_file, output, lb_clock);

	// freeing memory
	free(temp->type);
	free(temp);
	free(lb_clock->blockSize.type);
	free(lb_clock->cacheSize.type);
	free(lb_clock->pageSize.type);
	free(lb_clock->victimCandidateSet);
	free(lb_clock);
}

LB_Clock* initClock()
{
	// variables
	clock_t t = clock();

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
		// allocating victim set to size of cache blocks
		if(!(lb_clock->victimCandidateSet = (int*)malloc(lb_clock->blockPerCache * sizeof(int))))
		{
			printf("couldn't allocate memory for victim set array, exiting...");
			exit(-1);
		}
	}
	else
	{
		tempCache = getByteValue(&lb_clock->cacheSize);
		tempBlock = getByteValue(&lb_clock->blockSize);
		lb_clock->blockPerCache = tempCache/tempBlock;
		// allocating victim set to size of cache blocks
		if(!(lb_clock->victimCandidateSet = (int*)malloc(lb_clock->blockPerCache * sizeof(int))))
		{
			printf("couldn't allocate memory for victim set array, exiting...");
			exit(-1);
		}
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

	tempPages = tempBytes1 / tempBytes2;

	tempPages = checkPageInBlock(tempPages, tp, lb_clock);

	// free temp allocated string
	free(temp.type);

	return tempPages;
}

int checkPageInBlock(int tempPage, Trace *tp, LB_Clock *lb_clock)
{
	if(tempPage > lb_clock->pagePerBlock)
	{
		tp->pageOverage = lb_clock->pagePerBlock - tempPage;
		tempPage = lb_clock->pagePerBlock;
	}
	return tempPage;
}

// check if 
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
			if(currNode->cacheAddr == atoi(tp->addr))
			{
				found = 1;
			}
			currNode = currNode->next;
		}
	}
	return found;
}

// writing to cache
void writeToCache(Trace *tp, LB_Clock *lb_clock)
{
	// variables
	LB_Node *insertNode, *currNode;
	int numOfPagesNeeded;

	// safety check so we don't run out of memory
	if((insertNode = (LB_Node*)malloc(sizeof(LB_Node))) == NULL)
	{
		printf("Can malloc that many nodes, exiting...");
		exit(-1);
	}
	insertNode->blockNum = lb_clock->currBlockCount;
	insertNode->cacheAddr = atoi(tp->addr);
	// checking for number of pages needed for the block
	numOfPagesNeeded = getNumOfPages(tp, lb_clock);
	insertNode->currNumOfPages = numOfPagesNeeded;
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
		printf("cache miss, writing to cache, block address = %s, size = %d bytes\n", tp->addr, tp->size);
	}
	else
	{
		// here is where you evict block
		evictBlockFromCache(tp, lb_clock);
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
			if(currNode->cacheAddr == atoi(tp->addr))
			{
				// set hit in lb_clock
				pageNumCheck = getNumOfPages(tp, lb_clock);
				if(pageNumCheck == currNode->currNumOfPages)
				{
					currNode->refBit = 1;
					printf("cache hit, already in cache, block address = %s, size = %d bytes\n", currNode->cacheAddr, tp->size);
				}
				else
				{
					currNode->refBit = 0;
					//lb_clock->timeStamp += dataTransTime;
					// dirty needs to be written back to disk
					// need to test if new size will fit in block
					// decided what to do if not...
					printf("cache hit, writting to block address = %s, size = %d bytes\n", currNode->cacheAddr, tp->size);
				}
			}
			currNode = currNode->next;
		}
	}
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
			if(currNode->cacheAddr == atoi(tp->addr))
			{
				// set hit in lb_clock
				//lb_clock;
				//lb_clock->timeStamp += hitTime;
				printf("cache hit,reading from block address = %s, size = %d bytes\n", currNode->cacheAddr, tp->size);
			}
			currNode = currNode->next;
		}
	}
	return;
}

void evictBlockFromCache(Trace *tp, LB_Clock *lb_clock)
{
	// variables
	LB_Node *currNode, *PrevNode, *evictNode;

	// functions
	if(strcmp(tp->mode, "W") == 0)
	{
		// have to go with most pages in block and ref bit zero
		evictNode = checkVictimSetWrite(lb_clock);
	}
	else
	{
		// have to go with middle pages in block and ref bit zero
		//  !!! think on this !!!
		evictNode = checkVictimSetRead(lb_clock);

	}

	return;
}

void checkTraceMode(Trace *tp, LB_Clock *lb_clock)
{
	if(strcmp(tp->mode, "W") == 0)
	{
		// check if in cache already
		// if not
		if(checkBlockInCache(tp, lb_clock) != 1)
		{
			writeToCache(tp, lb_clock);
			lb_clock->pageFaults++;
		}
		else
		{
			updateBlockInCache(tp, lb_clock);
		}
	}
	else if(strcmp(tp->mode, "R") == 0)
	{
		// check if in cache already
		//if is
		if(checkBlockInCache(tp, lb_clock) != 1)
		{
			writeToCache(tp, lb_clock);
			lb_clock->pageFaults++;
		}
		else
		{
			readFromCache(tp, lb_clock);
		}
	}
	return;
}

LB_Node* checkVictimSetWrite(LB_Clock *lb_clock)
{
	// variables
	int evictBlock;
	LB_Node *evictNodeA, *evictNodeB, *finalEvictNode, *currNode;
	int reset;

	currNode = lb_clock->clockHand;
	evictNodeA = NULL;
	evictNodeB = NULL;

	while(currNode->next != lb_clock->clockHand)
	{
		// think about logic here.....test
		if(currNode->refBit == 0)
		{
			if(evictNodeB == NULL)
			{
				evictNodeB = evictNodeA;
			}
			evictNodeA = currNode;
		}
		if(evictNodeA != NULL && evictNodeB != NULL)
		{
			if(evictNodeB->currNumOfPages < evictNodeA->currNumOfPages)
			{
				finalEvictNode = evictNodeA;
				// testing to see if this holds for reseting victim set
				// that was evicted
				//reset = j-1;
			}
		}
		currNode = currNode->next;
	}


	//evictNodeFromCache
	return finalEvictNode;
}

LB_Node* checkVictimSetRead(LB_Clock *lb_clock)
{
	// variables
	int evictBlock;
	LB_Node *evictNodeA, *evictNodeB, *finalEvictNode, *currNode;
	int reset;

	currNode = lb_clock->clockStart;

	while(currNode->next != lb_clock->clockStart)
	{
		for(int j=1; j < lb_clock->blockPerCache; j++)
		{
			if(currNode->blockNum == lb_clock->victimCandidateSet[j-1])
			{
				evictNodeA = currNode;
			}
			if(currNode->blockNum == lb_clock->victimCandidateSet[j])
			{
				evictNodeB = currNode;
			}
			if(evictNodeA != NULL && evictNodeB != NULL)
			{
				if(evictNodeB->currNumOfPages > evictNodeA->currNumOfPages)
				{
					finalEvictNode = evictNodeA;
					// testing to see if this holds for reseting victim set
					// that was evicted
					reset = j-1;
				}
			}
		}
		currNode = currNode->next;
	}

	//evictNodeFromCache
	return finalEvictNode;
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
	printf("\ntesting clock tick as timer = %d\n\n", ticks);
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
	}
	return updated;
}

///////////////////////////////////////////
//	parser from chi's algorithm 1 code	//
/////////////////////////////////////////
Trace* traceParser(char *trace_file, char* result, LB_Clock *lb)
{
	FILE * trace_f;
	int i;
	int page_idx;
	char *p;
	char *array[10];
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
			
			fputs(line, stdout);
			//split the line into the struct
			p = strtok(line, ",");
			while (p != NULL)
			{
				array[i++] = p;
				p = strtok(NULL, ",");
			}			
			sample.addr = array[1];
			sample.size = atoi(array[2]);
			sample.mode = array[3];
			sample.time_stamp = atof(array[4]);
			checkTraceMode(&sample, lb);
			checkTicks(lb);

			//page_ref[cnt_transfer] = atoi(sample.addr);
			//page_idx = atoi(sample.addr) / CACHE_LINE_SIZE;	
			//output.time_stamp = sample.time_stamp;
			//output.page_fault = lru(page_idx).page_fault;
			//output.hit_rate = lru(page_idx).hit_rate;
			//output_helper(output, result);
		}
		fclose(trace_f);
	}
	return &sample;
}





