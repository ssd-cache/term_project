struct output_entry
{
	float time_stamp;
	int page_fault;
	float hit_rate;
};

void output_helper(struct output_entry input, char *output_file);
void simulation(char *test_file, int cnt_trace);
void result_parser(char *result_file);
