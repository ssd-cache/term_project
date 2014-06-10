/* defentions */
#define BLOCK_ERASE 1500
#define PAGE_READ 25
#define PAGE_WRITE 200
#define DATA_TRANS 13.65
#define MEM_ACCESS_LATE 0.0025

////4M
//#define CACHE_SIZE 4194304
////1M
//#define CACHE_LINE_SIZE 409600
//#define CACHE_ACCESS_SPEED 2
//#define CACHE_REPLACE_SPEED 1
//#define HDD_ACCESS_SPEED 20
////1G
//#define SIZE_OF_MEMORY 1073741824

void algorithm3(char *sim_file, char *output);
struct output_entry aml(int pid, char* mode);