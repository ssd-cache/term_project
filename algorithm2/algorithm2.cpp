// algorithm2.cpp : Defines the entry point for the console application.
//
// algorithm1.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "algorithm2.h"
#include "cache_ssd.h"
#include "StringManip.h"


void algorithm2(char *sim_file, char *output, void *arg)
{
	// local variables
	SizeInfo *size = (SizeInfo*)arg;
	LB_Clock* lb_clock = NULL;
	FILE *traceFile;
	char tempInput[1024];

	// simple print information about the input/ output & algo
    printf("this is the Long Block Clock (LB-Clock) algorithm\n");
	
	if(!(traceFile = fopen(sim_file, "r")))
	{
		printf("file didn't open, exiting...");
		exit(-2);
	}

	// initializing struct
	lb_clock = initClock();

	while(fgets(tempInput, sizeof(tempInput), traceFile) != NULL)
	{
		printf("%s\t", tempInput);
	}
	// set number of blocks in cache and number of pages in blocks
	setBlockOfCache(lb_clock);
	setPageOfBlocks(lb_clock);

	createCacheOfBlocks(lb_clock);


	// chi's test stuff
	printf("using the test file %s\n", sim_file);
    printf("output the result to %s\n", output);
	// freeing struct
	free(lb_clock);
}

LB_Clock* initClock()
{
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
	}
}

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

// this is going to be named something else
// using to calculate pages of a write or read
// from cache
void createBlocksOfPages(LB_Clock *lb_clock)
{

}


// test creating the whole cache size to see if
// we got crashes in allocating large amounts
// will change to be an insertion algo
void createCacheOfBlocks(LB_Clock *lb_clock)
{
	// variables
	LB_Node *insertNode, *currNode;

	// functions
	for(int i=0; i < lb_clock->blockPerCache; i++)
	{
		// safety check so we don't run out of memory
		if((insertNode = (LB_Node*)malloc(sizeof(LB_Node))) == NULL)
		{
			printf("Can malloc that many nodes, exiting...");
			exit(-1);
		}
		insertNode->blockNum = i;
		insertNode->currNumOfPages = 0;
		insertNode->refBit = 1;

		//testing for where to insert new node
		if(lb_clock->clockStart == NULL)
		{
			lb_clock->clockStart = insertNode;
			insertNode->next = lb_clock->clockStart;
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
	}
	return;
}





