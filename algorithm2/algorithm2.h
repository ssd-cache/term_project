
/* structures */
typedef struct InfoString
{
	char* type;
	int size;
}InfoString;

typedef struct LB_Node
{
	int blockNum;
	int currNumOfPages;
	int refBit;
	LB_Node* next;
}LB_Node;

typedef struct LB_Clock
{
	LB_Node* clockStart;
	LB_Node* clockHand;
	InfoString cacheSize;
	InfoString blockSize;
	InfoString pageSize;
	unsigned long blockPerCache;
	int pagePerBlock;
	int currBlockCount;
}LB_Clock;

/* functions */
void algorithm2(char *sim_file, char *output, void *arg);

LB_Clock* initClock();
void setBlockOfCache(LB_Clock *lb_clock);
void setPageOfBlocks(LB_Clock *lb_clock);
void createCacheOfBlocks(LB_Clock *lb_clock);

