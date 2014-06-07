
/* structures */
struct Trace 
{
	char* addr;
	int size;
	char* mode;
	float time_stamp;
	int pageOverage;
};

typedef struct InfoString
{
	char* type;
	int size;
}InfoString;

typedef struct LB_Node
{
	int blockNum;
	int cacheAddr;
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
	int *victimCandidateSet;
	double timeStamp;
	int timer;
	int pageFaults;
	int cacheHit;
}LB_Clock;

/* functions */
void algorithm2(char *sim_file, char *output, void *arg);

LB_Clock* initClock();
void setStructSizes(InfoString *size, LB_Clock *lb_clock);
void setBlockOfCache(LB_Clock *lb_clock);
void setPageOfBlocks(LB_Clock *lb_clock);
int getNumOfPages(Trace *tp, LB_Clock *lb_clock);
int checkPageInBlock(int tempPage, Trace *tp, LB_Clock *lb_clock);
int checkBlockInCache(Trace *tp, LB_Clock *lb_clock);
void writeToCache(Trace *tp, LB_Clock *lb_clock);
void readFromCache(Trace *tp, LB_Clock *lb_clock);
void evictBlockFromCache(Trace *tp, LB_Clock *lb_clock);
void replaceBlock(LB_Clock *lb_clock);
void updateBlockInCache(Trace *tp, LB_Clock *lb_clock);
void updateClockHand(LB_Clock *lb_clock);
int updateRefBit(LB_Clock *lb_clock);
LB_Node* checkVictimSetWrite(LB_Clock *lb_clock);
LB_Node* checkVictimSetRead(LB_Clock *lb_clock);
void checkTicks(LB_Clock *lb_clock);

void checkTraceMode(Trace *tp, LB_Clock *lb_clock);

void testAlloc(LB_Clock *lb_clock);

// from chi's code
Trace* traceParser(char *trace_file, char* result, LB_Clock *lb_clock);
