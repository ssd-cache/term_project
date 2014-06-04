//4M
#define CACHE_SIZE 4194304
//1M
#define CACHE_LINE_SIZE 1048576
#define CACHE_ACCESS_SPEED 2
#define CACHE_REPLACE_SPEED 1
#define HDD_ACCESS_SPEED 20
//1G
#define SIZE_OF_MEMORY 1073741824




void trace_parser(char *trace_file, char *result);
//void lru(struct Trace sample, int reference[]);
struct output_entry lru(int reference);
struct output_entry larc(int reference);

// added a void pointer to pass the size array of user input sizes
void algorithm1(char *sim_file, char*output, void *size);	// added pointer of the sizes array