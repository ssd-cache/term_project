// cache_ssd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "algorithm1.h"
#include "algorithm2.h"
#include "algorithm3.h"
#include <iostream>
#include <string>
#include <sstream>
#include <direct.h>
#include "cache_ssd.h"
using namespace std;
//const char *output_file = "output.csv";



void simulation(char *dat, int cnt_trace)
    {
        printf("start running the simulation\n");
		algorithm2();
		algorithm3();
		char *cur_dir;
		char cnt[128];
		itoa(cnt_trace, cnt, 10);
		cur_dir = (char *)malloc(100 * sizeof(char));
		getcwd(cur_dir, 100);
		char *output = strcat(cur_dir, "\\");
		output = strcat(output, cnt);
		output = strcat(output, "_output.csv");
        algorithm1(dat, output);
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


int main(int argc, _TCHAR* argv[])
{

    char *trace_files[] = {"c:\\trace1.txt", "c:\\trace2.txt"};
    for (int i=0; i<sizeof(trace_files)/sizeof(trace_files[0]); i++)
        {
            printf("the test dat is %s\n", trace_files[i]);
            simulation(trace_files[i], i);
        }
	return 0;
}

