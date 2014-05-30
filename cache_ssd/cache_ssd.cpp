// cache_ssd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "algorithm1.h"
#include <iostream>
#include <string>
#include <sstream>
#include "cache_ssd.h"
using namespace std;
const char *output_file = "output.csv";


void simulation(char *dat)
    {
        char *output = "output.csv";
        printf("start running the simulation\n");
        algorithm1(dat, output);
		//hash_lookup();
    }
void ouput_helper(struct output_entry input)
    {
        printf("formatting the output into csv\n");
		FILE *f = fopen(output_file, "a");
		if (f == NULL)
		{
			printf("Error opening file!\n");
			exit(1);
		}
		fprintf(f, "%f,%d,%f\n", input.time_stamp, input.page_fault, input.hit_rate);
		
    }
void result_parser(char *result_file)
    {
    //Adding the code to parse the result.

    }

int main(int argc, _TCHAR* argv[])
{

    char *trace_files[] = {"trace1.dat", "trace2.dat"};
    for (int i=0; i<sizeof(trace_files)/sizeof(trace_files[0]); i++)
        {
            printf("the test dat is %s\n", trace_files[i]);
            simulation(trace_files[i]);
        }
	return 0;
}

