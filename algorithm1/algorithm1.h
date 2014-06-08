//4M
#define CACHE_SIZE 4194304
//1M
#define CACHE_LINE_SIZE 409600
#define CACHE_ACCESS_SPEED 2
#define CACHE_REPLACE_SPEED 1
#define HDD_ACCESS_SPEED 20
//1G
#define SIZE_OF_MEMORY 1073741824
#define HASH_SEARCH 0
#define LRU 1
#define LARC 1




//void trace_parser(char *trace_file, char *result);
struct output_entry lru(int reference);
struct output_entry larc(int reference);
void algorithm1(char *sim_file, char*output);
