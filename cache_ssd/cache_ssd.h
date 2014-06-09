//#define TRACE_DIR ".//trace//"
//4M
#define CACHE_SIZE 4194304
//1M
#define CACHE_LINE_SIZE 409600
#define CACHE_ACCESS_SPEED 2
#define CACHE_REPLACE_SPEED 1
#define HDD_ACCESS_SPEED 20
//1G
#define SIZE_OF_MEMORY 1073741824
struct output_entry
{
	float time_stamp;
	int page_fault;
	float hit_rate;
};
struct trace {
	char* addr;
	int size;
	char* mode;
	float time_stamp;
};

struct SizeInfo
{
	char* type;
	int size;
};

void trace_parser(char *trace_file, char *result);
void output_helper(struct output_entry input, char *output_file);
void simulation(char *test_file, char *file_name);
void simulation(char *test_file, char *file_name, SizeInfo *size);
void result_parser(char *result_file);
void getSizeInput(SizeInfo *sizeArr);
