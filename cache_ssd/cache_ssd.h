//#define TRACE_DIR ".//trace//"
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
struct sizeinfo{
	int size;
	char *attr;
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
