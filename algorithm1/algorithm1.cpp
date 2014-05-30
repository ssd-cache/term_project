// algorithm1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algorithm1.h"
#include "cache_ssd.h"

using namespace std;

int cache_queue[4];
const int num_blocks = CACHE_SIZE / CACHE_LINE_SIZE;
const int num_pages = SIZE_OF_MEMORY / CACHE_LINE_SIZE;
int frame_idx = 0;
int page_idx = 0;
int page_ref[num_pages];
int cnt_transfer = 0;

struct trace {
	char* addr;
	int size;
	char* mode;
	float time_stamp;
} ;

int cnt = 0;
int q[4];
int mru_cnt[4];
int lru_cnt[4];
bool found = false;
int test[] = { 1, 2, 3, 3, 5, 5, 6 };

void call_lru()
{
	for (int j = 0; j < 7; j++)
	{
		lru(test[j]);
	}
	for (int k = 0; k < 4; k++)
	{
		printf("%d\n", q[k]);
		printf("%d\n", lru_cnt[k]);
		printf("%d\n", mru_cnt[k]);
	}

}
void lru(int reference)
{
	found = false;
	for (int i = 0; i < cnt; i++)
	{
		lru_cnt[i]++;
		if (q[i] == reference)
		{
			mru_cnt[i]++;
			//get next reference
			found = true;
			break;
		}		
	}
	if (!found)
	{		
		if (cnt < 4)
		{
			q[cnt] = reference;		
			cnt++;
		}
		//replacement is happening
		else
		{

		}
	}


}
//void lru(trace test_sample, int reference[])
//{
//	printf("%s\n", test_sample.addr);
//	printf("%d\n", test_sample.size);
//
//	//int q[num_blocks], p[num_pages], c = 0, c1, d, f, i, j, k = 0, n, r, t, b[num_blocks], c2[num_blocks];
//
//	//page_idx = atoi(test_sample.addr) / CACHE_LINE_SIZE;
//
//	//printf("Enter no of pages:");
//	////scanf("%d", &n);
//	//n = num_pages;
//
//	//printf("Enter the reference string:");
//	//for (i = 0; i < cnt_transfer; i++)
//	//	p[i] = page_ref[i];
//	//	//scanf("%d", &p[i]);
//	//printf("Enter no of frames:");
//	////scanf("%d", &f);
//	//f = num_blocks;
//	//q[k] = p[k];
//	//printf("\n\t%d\n", q[k]);
//	//c++;
//	//k++;
//	//for (i = 1; i<n; i++)
//	//{
//	//	c1 = 0;
//	//	for (j = 0; j<f; j++)
//	//	{
//	//		if (p[i] != q[j])
//	//			c1++;
//	//	}
//	//	if (c1 == f)
//	//	{
//	//		c++;
//	//		if (k<f)
//	//		{
//	//			q[k] = p[i];
//	//			k++;
//	//			for (j = 0; j<k; j++)
//	//				printf("\t%d", q[j]);
//	//			printf("\n");
//	//		}
//	//		else
//	//		{
//	//			for (r = 0; r<f; r++)
//	//			{
//	//				c2[r] = 0;
//	//				for (j = i - 1; j<n; j--)
//	//				{
//	//					if (q[r] != p[j])
//	//						c2[r]++;
//	//					else
//	//						break;
//	//				}
//	//			}
//	//			for (r = 0; r<f; r++)
//	//				b[r] = c2[r];
//	//			for (r = 0; r<f; r++)
//	//			{
//	//				for (j = r; j<f; j++)
//	//				{
//	//					if (b[r]<b[j])
//	//					{
//	//						t = b[r];
//	//						b[r] = b[j];
//	//						b[j] = t;
//	//					}
//	//				}
//	//			}
//	//			for (r = 0; r<f; r++)
//	//			{
//	//				if (c2[r] == b[0])
//	//					q[r] = p[i];
//	//				printf("\t%d", q[r]);
//	//			}
//	//			printf("\n");
//	//		}
//	//	}
//	//}
//	//printf("\nThe no of page faults is %d", c);
//	//output_entry new_entry;
//	//new_entry.page_fault = c;
//	//new_entry.time_stamp = test_sample.time_stamp;
//	//new_entry.hit_rate = 0.5;
//	//ouput_helper(new_entry);
//
//}

void algorithm1(char *sim_file, char *output)
    {

        printf("this is the 1th algorithm\n");
        printf("using the test file %s\n", sim_file);
		trace_parser("c:\\trace.txt");
        printf("output the result to %s\n", output);

    }

void trace_parser(char *trace_file)
{
	FILE * trace_f;
	int i;
	
	char *p;
	char *array[4];
	trace sample;
	
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
			page_ref[cnt_transfer] = atoi(sample.addr);
			cnt_transfer++;
			//lru(sample, page_ref);
			call_lru();
		}
		fclose(trace_f);
	}
	
	
}








