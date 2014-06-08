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
#include "cache_ssd.h"


using namespace std;
extern int cnt;



void simulation(char *dat, char *file_name, SizeInfo *sizeArr)
    {
        printf("start running the simulation\n");
		char *output = strcat(file_name, ".csv");
        //algorithm1(dat, output);
		algorithm2(dat, output, sizeArr);

    }

void simulation(char *dat, char *file_name)
    {
        printf("start running the simulation\n");
		algorithm3();
		char *output = strcat(file_name, ".csv");
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
			splitInput(tempInput, &sizeArr[0]);
		}
		else if(i == 1)
		{
			printf("Enter block size: ");
			fgets(tempInput, sizeof(tempInput), stdin);
			lowerCaseString(tempInput);
			removeSpaces(tempInput);
			splitInput(tempInput, &sizeArr[1]);
		}
		else if(i == 2)
		{
			printf("Enter page size: ");
			fgets(tempInput, sizeof(tempInput), stdin);
			lowerCaseString(tempInput);
			removeSpaces(tempInput);
			splitInput(tempInput, &sizeArr[2]);
		}
		rewind(stdin);
	}
	return;
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

int main(int argc, _TCHAR* argv[])
{
	//get the list of the trace files
	system("dir /b .\\trace\\ >file.txt");
	FILE *file_list = fopen("file.txt", "r");
	SizeInfo sizeArr[3];

	// getting size info
	getSizeInput(sizeArr);

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
			simulation(trace_file, line);
			simulation(trace_file,line, sizeArr);
		}
	}
	return 0;
}

