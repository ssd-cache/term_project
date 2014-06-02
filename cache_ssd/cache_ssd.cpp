// cache_ssd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "algorithm1.h"
#include "algorithm2.h"
#include "algorithm3.h"
#include "StringManip.h"
#include <iostream>
#include <string>
#include <sstream>
#include <direct.h>
#include "cache_ssd.h"
using namespace std;
//const char *output_file = "output.csv";



void simulation(char *dat, int cnt_trace, SizeInfo *sizeArr)
    {
        printf("start running the simulation\n");
		algorithm3();
		char *cur_dir;
		char cnt[128];
		itoa(cnt_trace, cnt, 10);
		cur_dir = (char *)malloc(100 * sizeof(char));
		getcwd(cur_dir, 100);
		char *output = strcat(cur_dir, "\\");
		output = strcat(output, cnt);
		output = strcat(output, "_output.csv");
		// chi added the size array of the users input sizes
        algorithm1(dat, output, sizeArr);
		algorithm2(dat, output, sizeArr);
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

void getSizeInput(SizeInfo *sizeArr)
{
	// local varaibles
	char tempInput[1024];

	// get sizes needed for project
	for(int i=0; i<3; i++)
	{
		if(i == 0)
		{
			printf("Enter cache size: ");
			fgets(tempInput, sizeof(tempInput), stdin);
			lowerCaseString(tempInput);
			removeSpaces(tempInput);
			splitInput(tempInput, &sizeArr);
		}
		else if(i == 1)
		{
			printf("Enter block size: ");
			fgets(tempInput, sizeof(tempInput), stdin);
			lowerCaseString(tempInput);
			removeSpaces(tempInput);
			splitInput(tempInput, &sizeArr);
		}
		else if(i == 2)
		{
			printf("Enter page size: ");
			fgets(tempInput, sizeof(tempInput), stdin);
			lowerCaseString(tempInput);
			removeSpaces(tempInput);
			splitInput(tempInput, &sizeArr);
		}
		rewind(stdin);
	}
	return;
}


int main(int argc, _TCHAR* argv[])
{

    char *trace_files[] = {"c:\\trace1.txt", "c:\\trace2.txt"};
	SizeInfo sizeArr[3];

    for (int i=0; i<sizeof(trace_files)/sizeof(trace_files[0]); i++)
        {
            printf("the test dat is %s\n", trace_files[i]);
            simulation(trace_files[i], i, sizeArr);
        }
	return 0;
}

