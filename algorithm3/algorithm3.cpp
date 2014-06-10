// algorithm3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cache_ssd.h"
#include "algorithm3.h"
#include "StringManip.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <time.h>

using namespace std;

struct page {
	int coldFlag;
	int id;
	bool clean;
};

//vector<page> cache;
vector<page> L1;
vector<page> L2;
vector<page> L3;
const int BOUNDARY = 4;
const int COOLMAX = 10;
int count;
int queue_optimized = 0;
static int page_fault = 0;
static int cache_hit = 0; // Anthony added as test to see if we could get code running
static int total_count = 0;
int adjustFrequency = 0;
const int max_pages = SIZE_OF_MEMORY / CACHE_LINE_SIZE;
int num_pages = max_pages/2;

//Time variables. not sure if this works!
double timestamp = 0;
const clock_t begin_time = clock();

void adjust_window(char* mode)
{
	timestamp += PAGE_READ;
	if(mode == "R" && L3.at(L3.size()-1).clean && L3.at(L3.size()-2).clean)
	{
		num_pages = num_pages*0.8;
		if(L1.size() > num_pages)
		{
			vector<page> tempL1;
			for(int i = L1.size()-num_pages; i < L1.size(); i++)
			{
				tempL1.push_back(L1.at(i));
			}
			L1 = tempL1;
		}
		if(L2.size() > num_pages)
		{
			vector<page> tempL2;
			for(int i = L2.size()-num_pages; i < L2.size(); i++)
			{
				tempL2.push_back(L2.at(i));
			}
			L2 = tempL2;
		}
	}
	else if(mode == "W" && !L3.at(L3.size()-1).clean && !L3.at(L3.size()-2).clean && num_pages*1.2 < max_pages)
		num_pages = num_pages*1.2;
}

void replace(int targetList, int targetIndex, page target)
{
	page replacePage;
	if(targetList == 1)
	{
		replacePage = L1.at(targetIndex);
		L1.at(targetIndex) = target;
	}
	else
	{
		replacePage = L2.at(targetIndex);
		L2.at(targetIndex) = target;
	}
	L3.push_back(replacePage);
	
	if(!replacePage.clean)
		timestamp += (DATA_TRANS + MEM_ACCESS_LATE);
	timestamp += BLOCK_ERASE;
}

void findPageToReplace(page target, char* mode)
{
	if(!L2.empty())
		replace(2,0, target);
	else
	{
		adjust_window(mode);
		bool found = false;
		page tempPage;
		for(int i = 0; i < L1.size(); i++)
		{
			tempPage = L1.at(i);
			if(tempPage.clean)
			{
				replace(1,i,target);
				found = true;
				break;
			}
			else
				tempPage.coldFlag ++;
		}

		if(!found)
		{
			for(int i = 0; i < L1.size(); i++)
			{
				tempPage = L1.at(i);
				if(tempPage.coldFlag >= BOUNDARY)
				{
					replace(1,i,target);
					found = true;
					break;
				}
			}
		}

		if(!found)
			replace(1,0,target);
	}
}

void agePages()
{
	for(int i = 0; i < L1.size(); i++)
	{
		page tempPage = L1.at(i);
		if(tempPage.coldFlag < COOLMAX)
		{
			tempPage.coldFlag++;
			if(tempPage.clean && tempPage.coldFlag >= BOUNDARY)
			{
				L2.push_back(tempPage);
				L1.erase(L1.begin()+i);			
			}
		}
	}
	for(int k = 0; k < L2.size(); k++)
	{
		page tempPage = L2.at(k);
		if(tempPage.coldFlag < COOLMAX)
			tempPage.coldFlag++;
	}
}

struct output_entry aml(int pid, char* mode)
{
	total_count++;
	output_entry stat;
	page newPage;
	newPage.coldFlag = 0;
	newPage.id = pid;
	bool found = false;

	if(mode == "R")
		newPage.clean = true;
	else
		newPage.clean = false;
	
	//Look for page hit
	for(int i = 0; i < L1.size(); i++)
	{
		page tempPage = L1.at(i);
		if (tempPage.id == pid)
		{
			tempPage.coldFlag = 0;
			L1.push_back(tempPage);
			L1.erase(L1.begin()+i);
			agePages();
			found = true;
			cache_hit++; // Anthony added as test to see if we could get code running
			timestamp += PAGE_READ;
			//stat.hit_rate = cache_hit; // Anthony added as test to see if we could get code running
		}
	}

	if(!found)
	{
		//if(cache.size() < max_pages)
		if(L1.size() < num_pages)
		{
			//cache.push_back(newPage);
			L1.push_back(newPage);
			timestamp += PAGE_WRITE;
		}
		else
			findPageToReplace(newPage,mode);
	
		agePages();
		page_fault++;
		stat.page_fault = page_fault; // Anthony added as test to see if we could get code running
	}
	stat.hit_rate = (float)cache_hit / total_count;
	return stat;
}

void trace_parse(char *trace_file, char* result)
{
	FILE * trace_f;
	int i;
	int page_idx;
	char *p;
	char *array[4];
	count = 0;
	trace sample;
	output_entry output;
	trace_f = fopen(trace_file, "r");
	if (trace_f != NULL)
	{
		char line[128];
		while (fgets(line, sizeof(line), trace_f) != NULL)
		{
			i = 0;

			//fputs(line, stdout);
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
			lowerCaseString(sample.mode);
			sample.time_stamp = atof(array[3]);
			//page_ref[cnt_transfer] = atoi(sample.addr);
			long value = strtol(sample.addr, NULL, 16);
			page_idx = value / CACHE_LINE_SIZE;
			//page_idx = atoi(sample.addr) / CACHE_LINE_SIZE;
			output.time_stamp = sample.time_stamp;
			//call the algorithm
			//output_entry temp = larc(page_idx);
			//output.page_fault = temp.page_fault;
			//output.hit_rate = temp.hit_rate;
			//output_helper(output, result);
			output_entry temp = aml(page_idx, sample.mode);
			output.page_fault = temp.page_fault;
			output.hit_rate = temp.hit_rate;
			output_helper(output, result);
		}
		fclose(trace_f);
	}
}

void algorithm3(char *sim_file, char *output)
{
	printf("this is the 3rd algorithm\n");
	trace_parse(sim_file, output);
    printf("output the result to %s\n", output);
}

void findPageToReplace3_5(page target, char* mode)
{
	if(!L2.empty())
		replace(2,0, target);
	else
	{
		int dirtyIndex = 0;
		if(adjustFrequency == 5)
			adjust_window(mode);
		else
			adjustFrequency++;
		bool found = false;
		page tempPage;
		for(int i = 0; i < L1.size(); i++)
		{
			tempPage = L1.at(i);
			if(tempPage.clean)
			{
				replace(1,i,target);
				found = true;
				break;
			}
			else if(tempPage.coldFlag >= BOUNDARY)
				dirtyIndex = i;
			else
				tempPage.coldFlag ++;
		}

		if(!found)
			replace(1,dirtyIndex,target);
	}
}

struct output_entry aml3_5(int pid, char* mode)
{
	total_count++;
	output_entry stat;
	page newPage;
	newPage.coldFlag = 0;
	newPage.id = pid;
	bool found = false;

	if(mode == "R")
		newPage.clean = true;
	else
		newPage.clean = false;
	
	//Look for page hit
	for(int i = 0; i < L1.size(); i++)
	{
		page tempPage = L1.at(i);
		if (tempPage.id == pid)
		{
			tempPage.coldFlag = 0;
			L1.push_back(tempPage);
			L1.erase(L1.begin()+i);
			agePages();
			found = true;
			cache_hit++; // Anthony added as test to see if we could get code running
			timestamp += PAGE_READ;
			//stat.hit_rate = cache_hit; // Anthony added as test to see if we could get code running
		}
	}

	if(!found)
	{
		//if(cache.size() < max_pages)
		if(L1.size() < num_pages)
		{
			//cache.push_back(newPage);
			L1.push_back(newPage);
			timestamp += PAGE_WRITE;
		}
		else
			findPageToReplace(newPage,mode);
	
		agePages();
		page_fault++;
		stat.page_fault = page_fault; // Anthony added as test to see if we could get code running
	}
	stat.hit_rate = (float)cache_hit / total_count;
	return stat;
}

void trace_parse3_5(char *trace_file, char* result)
{
	FILE * trace_f;
	int i;
	int page_idx;
	char *p;
	char *array[4];
	count = 0;
	trace sample;
	output_entry output;
	trace_f = fopen(trace_file, "r");
	if (trace_f != NULL)
	{
		char line[128];
		while (fgets(line, sizeof(line), trace_f) != NULL)
		{
			i = 0;

			//fputs(line, stdout);
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
			//output_entry temp = larc(page_idx);
			//output.page_fault = temp.page_fault;
			//output.hit_rate = temp.hit_rate;
			//output_helper(output, result);
			output_entry temp = aml3_5(page_idx, sample.mode);
			output.page_fault = temp.page_fault;
			output.hit_rate = temp.hit_rate;
			output_helper(output, result);
		}
		fclose(trace_f);
	}
}

void algorithm3_5(char *sim_file, char *output)
{
	printf("this is the 3rd algorithm\n");
	trace_parse3_5(sim_file, output);
    printf("output the result to %s\n", output);
}