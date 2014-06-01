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
/*
struct trace {
	char* addr;
	int size;
	char* mode;
	float time_stamp;
} ;
*/

int q[4];
int cnt = 0;
int mru_cnt[4];
int lru_cnt[4];
int page_fault = 0;
int reference_cnt = 0;
int hit_cnt = 0;


bool found = false;
struct output_entry lru(int reference)
{	
	output_entry stat;
	reference_cnt++;
	found = false;
	for (int i = 0; i < cnt; i++)
	{
		lru_cnt[i]++;
		if (q[i] == reference)
		{
			hit_cnt++;
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
			printf("The queue is full, will replace the old block\n");
			page_fault++;
			//compare the mru cnt of the each block
			for (int j = 0; j < 3; j++)
			{
				if (mru_cnt[j] < mru_cnt[j + 1])
				{
					//swap the value
					int temp = q[j];
					q[j] = q[j + 1];
					q[j + 1] = temp;
					//swap the mru cnt
					int temp_mru_cnt = mru_cnt[j];
					mru_cnt[j] = mru_cnt[j + 1];
					mru_cnt[j + 1] = temp_mru_cnt;
					//swap the lru cnt
					int temp_lru_cnt = lru_cnt[j];
					lru_cnt[j] = lru_cnt[j + 1];
					lru_cnt[j + 1] = temp_lru_cnt;

				}
				else if (mru_cnt[j] == mru_cnt[j+1])
				{
					if (lru_cnt[j] > lru_cnt[j + 1])
					{
						int temp2 = q[j];
						q[j] = q[j + 1];
						q[j + 1] = temp2;
						//swap the cnt
						int temp_cnt = mru_cnt[j];
						mru_cnt[j] = mru_cnt[j + 1];
						mru_cnt[j + 1] = temp_cnt;
						//swap the lru cnt
						int temp_lru_cnt = lru_cnt[j];
						lru_cnt[j] = lru_cnt[j + 1];
						lru_cnt[j + 1] = temp_lru_cnt;
					}
				}
			}
			//get the relaced block
			printf("the block %d is replaced\n", q[3]);
			q[3] = reference;
			//set the mru cnt for the new block
			mru_cnt[3] = 0;
			lru_cnt[3] = 0;
		}
	}
	//count the total of page fault
	stat.page_fault = page_fault;
	stat.hit_rate = (float)hit_cnt / reference_cnt;
	return stat;

}
/*temp comment out
struct output_entry larc(int reference)
{
	int ghost_q[3] = { 0 };
	output_entry stat;
	reference_cnt++;
	found = false;
	for (int i = 0; i < cnt; i++)
	{
		lru_cnt[i]++;
		if (q[i] == reference)
		{
			hit_cnt++;
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
*/
void algorithm1(char *sim_file, char *output)
    {

        printf("this is the 1th algorithm\n");
		int q[4] = { 0 };
		int mru_cnt[4] = { 0 };
		int lru_cnt[4] = { 0 };
		int cnt = 0;
		page_fault = 0;
		hit_cnt = 0;
		reference_cnt = 0;
		trace_parser(sim_file, output);
        printf("output the result to %s\n", output);
    }








