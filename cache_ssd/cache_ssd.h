struct output_entry
{
	float time_stamp;
	int page_fault;
	float hit_rate;
};

void ouput_helper(struct output_entry input);
void simulation(char *test_file);
void result_parser(char *result_file);