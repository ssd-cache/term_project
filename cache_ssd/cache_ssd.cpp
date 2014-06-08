// cache_ssd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "algorithm1.h"
#include <iostream>
//#include "algorithm2.h"
#include "algorithm3.h"
#include <iostream>
#include <string>
#include <sstream>
#include "cache_ssd.h"


using namespace std;
extern int cnt;



void simulation(char *dat, char *file_name)
    {
        printf("start running the simulation\n");
		//algorithm2();
		//algorithm3();
		char *output = strcat(file_name, ".csv");
        algorithm1(dat, output);
    }

void simulation(char*dat, char *file_name, int input_size)
{
	printf("use the overload simulation for interactive mode\n");
	char *output = strcat(file_name, ".csv");
	//algorithm2(dat, output, input.size);
}
void output_helper(struct output_entry input, char* output_file)
    {
        printf("formatting the output into csv\n");
		FILE *f = fopen(output_file, "a");
		if (f == NULL)
		{
			printf("Error opening file!\n");
			exit(1);
		}
		fprintf(f, "%f,%d,%f\n", input.time_stamp, input.page_fault, input.hit_rate);
		fclose(f);
		
    }

void trace_parser(char *trace_file, char* result)
{
	FILE * trace_f;
	int i;
	int page_idx;
	char *p;
	char *array[4];
	cnt = 0;
	trace sample;
	output_entry output;
	trace_f = fopen(trace_file, "r");
	if (trace_f != NULL)
	{
		char line[128];
		while (fgets(line, sizeof(line), trace_f) != NULL)
		{
			i = 0;

			fputs(line, stdout);
			//split the line into the struct
			p = strtok(line, ",");
			while (p != NULL)
			{
				array[i++] = p;
				p = strtok(NULL, ",");
			}
			sample.addr = array[0];
			sample.size = atoi(array[1]);
			sample.mode = array[2];
			sample.time_stamp = atof(array[3]);
			//page_ref[cnt_transfer] = atoi(sample.addr);
			long value = strtol(sample.addr, NULL, 16);
			page_idx = value / CACHE_LINE_SIZE;
			//page_idx = atoi(sample.addr) / CACHE_LINE_SIZE;
			output.time_stamp = sample.time_stamp;
			//call the algorithm
			output_entry temp = larc(page_idx);
			output.page_fault = temp.page_fault;
			output.hit_rate = temp.hit_rate;
			output_helper(output, result);
		}
		fclose(trace_f);
	}
}

int main(int argc, char* argv[])
{
	//get the list of the trace files
	system("dir /b .\\trace\\ >file.txt");
	FILE *file_list = fopen("file.txt", "r");
	if (file_list != NULL)
	{
		char line[128];
		while (fgets(line, sizeof(line), file_list) != NULL)
		{
			fputs(line, stdout);
			char trace_dir[128];
			char trace_file[128];
			strcpy(trace_dir, ".\\trace\\");
			line[strlen(line) - 1] = '\0';
			strcat(trace_dir, line);
			strcpy(trace_file, trace_dir);
			printf("the test dat is %s\n", trace_file);
			sizeinfo input;
			//non-input mode
			if (argc == 1)
			{
				simulation(trace_file, line);
			}
			else
			{
				
				input.size = atoi(argv[1]);
				simulation(trace_file, line, input.size);
				
			}
		}
	}
	return 0;
}

