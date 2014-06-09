/* defentions */
#define	BLOCK_ERASE 1500
#define PAGE_READ 25
#define PAGE_WRITE 200
#define DATA_TRANS 13.65
#define MEM_ACCESS_LATE 0.0025

// these shouldn't be hardcoded!!!!
#define CACHE_SIZE 4
#define BLOCK_SIZE 128
#define PAGE_SIZE 2
// sizes
#define KB "kb"
#define MB "mb"
#define GB "gb"

/* structures */
struct Trace 
{
	char* addr;
	int size;
	char* mode;
	float time_stamp;
};

typedef struct InfoString
{
	char* type;
	int size;
}InfoString;

typedef struct LB_Node
{
	int blockNum;
	int *pageTable;
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
	double timeStamp;
	int timer;
	int pageFaults;
	int cacheHit;
}LB_Clock;

/* functions */
void algorithm2(char *sim_file, char *output);
void algorithm2(char *sim_file, char *output, void *arg);
void algorithm2_5(char *sim_file, char *output, void *arg);

LB_Clock* initClock();
void setStructSizes(InfoString *size, LB_Clock *lb_clock);
void setBlockOfCache(LB_Clock *lb_clock);
void setPageOfBlocks(LB_Clock *lb_clock);
int getNumOfPages(Trace *tp, LB_Clock *lb_clock);
int checkPageSize(Trace *tp, LB_Clock *lb_clock, LB_Node *np);
int checkBlockInCache(Trace *tp, LB_Clock *lb_clock);
LB_Node* checkPagesInBlock(LB_Clock *lb_clock);
void writeToCache(Trace *tp, LB_Clock *lb_clock); 
void writeCacheBlock(Trace *tp, LB_Clock *lb_clock);
void readFromCache(Trace *tp, LB_Clock *lb_clock);
void evictBlockFromCache(Trace *tp, LB_Clock *lb_clock, LB_Node* newNode);
void replaceBlock(LB_Clock *lb_clock);
void addPagesToCache(Trace *tp, LB_Clock *lb_clock, LB_Node *np);
void updateBlockInCache(Trace *tp, LB_Clock *lb_clock);
void updateClockHand(LB_Clock *lb_clock);
int updateRefBit(LB_Clock *lb_clock);
LB_Node* checkVictimSetWrite(LB_Clock *lb_clock);
LB_Node* checkVictimSetRead(LB_Clock *lb_clock);
void checkTicks(LB_Clock *lb_clock);
void movePagesToNewBlock(Trace *tp, LB_Clock *lb_clock, LB_Node *currNode);

void checkTraceMode(Trace *tp, LB_Clock *lb_clock);

// from chi's code
void traceParser(char *trace_file, char* result, LB_Clock *lb_clock);
void traceParser2_5(char *trace_file, char* result, LB_Clock *lb_r, LB_Clock* lb_w);
