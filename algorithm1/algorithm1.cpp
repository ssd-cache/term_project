// algorithm1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "algorithm1.h"
#include "cache_ssd.h"
#include "hash.h"


using namespace std;

int cache_queue[4];
const int num_blocks = CACHE_SIZE / CACHE_LINE_SIZE;
const int num_pages = SIZE_OF_MEMORY / CACHE_LINE_SIZE;
int frame_idx = 0;
int page_idx = 0;
int page_ref[num_pages];
int cnt_transfer = 0;

//counters of queue of cache
//int q[4];
int q[num_blocks];
int ghost_q[3];
int cnt = 0;
//int mru_cnt[4];
//int lru_cnt[4];
int mru_cnt[num_blocks];
int lru_cnt[num_blocks];
//counters of ghost queue
int g_cnt = 0;
int g_mru_cnt[3];
int g_lru_cnt[3];
bool g_found = false;

int page_fault = 0;
int reference_cnt = 0;
int hit_cnt = 0;
bool found = false;



//test code of hash
char *test[5] = { "abc", "def", "efg", "test", "fast" };
int test_array[13] = { 1, 2, 3, 3, 4, 3, 6, 9, 10,12,10,7,9 };
void test_func()
{
	for (int m = 0; m < 13; m++)
	{
		larc(test_array[m]);
	}
}
void foo()
{
	for (int t = 0; t < 5; t++)
	{
		printf("the string is %s\n", test[t]);
		//unsigned int hval = hash(test[t]);
		//printf("the hash value is %u\n", hval);
		char key[3];
		itoa(t, key, 10);
		addhash(key, test[t]);
	}
	if (findhash("3") != NULL)
	{
		printf("The reference is found\n");
	}
	
}

struct output_entry lru(int reference)
{	
	output_entry stat;
	reference_cnt++;
	found = false;
	//loop search
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
		//if (cnt < 4)
		if (cnt < num_blocks)
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
			//for (int j = 0; j < 3; j++)
			for (int j = 0; j < num_blocks-1; j++)
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
			printf("the block %d is replaced\n", q[num_blocks-1]);
			//q[3] = reference;
			q[num_blocks - 1] = reference;
			//set the mru cnt for the new block
			//mru_cnt[3] = 0;
			//lru_cnt[3] = 0;
			mru_cnt[num_blocks-1] = 0;
			lru_cnt[num_blocks-1] = 0;
		}
	}
	//count the total of page fault
	stat.page_fault = page_fault;
	stat.hit_rate = (float)hit_cnt / reference_cnt;
	return stat;

}

struct output_entry larc(int reference)
{

	
	output_entry stat;
	reference_cnt++;
	g_found = false;
	float search_time;
	const clock_t begin_time = clock();
	//first fill the Q
	reference_cnt++;
	found = false;
	//loop search
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
		//if (cnt < 4)
		if (cnt < num_blocks)
		{
			q[cnt] = reference;
			cnt++;
		}
		//replacement is happening
		else
		{
			//using the ghost queue
			page_fault++;

			char *index;
			char *value;
			index = (char*)malloc(256);
			value = (char*)malloc(256);
			sprintf(index, "%d", g_cnt);
			sprintf(value, "%d", reference);
			//using the hash search in stead of the loop
			if (g_cnt < 3)
			{
				//add to the hash table

				if (findhash(value) != NULL)
				{
					g_found = true;
					//move it into to the Q
					q[sizeof(q) / sizeof(int)-1] = reference;
				}
				else
				{
					addhash(value, index);
					g_cnt++;
				}
				
			}
			else
			{
				if (HASH_SEARCH){

					if (findhash(value) != NULL)
					{
						printf("%f\n", float(clock() - begin_time) / CLOCKS_PER_SEC);
						g_found = true;
						//move it into the Q
						q[sizeof(q) / sizeof(int)-1] = reference;
					}
					else
					{
						//lru replacement in ghost queue
						printf("%f\n", float(clock() - begin_time) / CLOCKS_PER_SEC);
						for (int j = 0; j < 2; j++)
						{
							if (g_mru_cnt[j] < g_mru_cnt[j + 1])
							{
								//swap the value
								int temp = ghost_q[j];
								ghost_q[j] = ghost_q[j + 1];
								ghost_q[j + 1] = temp;
								//swap the mru cnt
								int temp_mru_cnt = g_mru_cnt[j];
								g_mru_cnt[j] = g_mru_cnt[j + 1];
								g_mru_cnt[j + 1] = temp_mru_cnt;
								//swap the lru cnt
								int temp_lru_cnt = g_lru_cnt[j];
								g_lru_cnt[j] = g_lru_cnt[j + 1];
								g_lru_cnt[j + 1] = temp_lru_cnt;

							}
							else if (g_mru_cnt[j] == g_mru_cnt[j + 1])
							{
								if (g_lru_cnt[j] > g_lru_cnt[j + 1])
								{
									int temp2 = ghost_q[j];
									ghost_q[j] = ghost_q[j + 1];
									ghost_q[j + 1] = temp2;
									//swap the cnt
									int temp_cnt = g_mru_cnt[j];
									g_mru_cnt[j] = g_mru_cnt[j + 1];
									g_mru_cnt[j + 1] = temp_cnt;
									//swap the lru cnt
									int temp_lru_cnt = g_lru_cnt[j];
									g_lru_cnt[j] = g_lru_cnt[j + 1];
									g_lru_cnt[j + 1] = temp_lru_cnt;
								}
							}
						}
						//get the relaced block
						printf("the block %d is replaced\n", ghost_q[2]);
						ghost_q[2] = reference;
						//set the mru cnt for the new block
						g_mru_cnt[2] = 0;
						g_lru_cnt[2] = 0;
					}
				}
			}

			//hash implementation done
			if (!HASH_SEARCH)
			{
				for (int i = 0; i < g_cnt; i++)
				{
					g_lru_cnt[i]++;
					if (ghost_q[i] == reference)
					{
						//move it into the q
						printf("%f\n", float(clock() - begin_time) / CLOCKS_PER_SEC);
						q[sizeof(q) / sizeof(int)-1] = reference;
						g_found = true;
						break;
					}
				}
				printf("%f\n", float(clock() - begin_time) / CLOCKS_PER_SEC);
				if (!g_found)
				{
					if (g_cnt < 3)
					{
						ghost_q[g_cnt] = reference;
						g_cnt++;
					}
					else
					{
						//apply the lru algorithm for replacing
						for (int j = 0; j < 2; j++)
						{
							if (g_mru_cnt[j] < g_mru_cnt[j + 1])
							{
								//swap the value
								int temp = ghost_q[j];
								ghost_q[j] = ghost_q[j + 1];
								ghost_q[j + 1] = temp;
								//swap the mru cnt
								int temp_mru_cnt = g_mru_cnt[j];
								g_mru_cnt[j] = g_mru_cnt[j + 1];
								g_mru_cnt[j + 1] = temp_mru_cnt;
								//swap the lru cnt
								int temp_lru_cnt = g_lru_cnt[j];
								g_lru_cnt[j] = g_lru_cnt[j + 1];
								g_lru_cnt[j + 1] = temp_lru_cnt;

							}
							else if (g_mru_cnt[j] == g_mru_cnt[j + 1])
							{
								if (g_lru_cnt[j] > g_lru_cnt[j + 1])
								{
									int temp2 = ghost_q[j];
									ghost_q[j] = ghost_q[j + 1];
									ghost_q[j + 1] = temp2;
									//swap the cnt
									int temp_cnt = g_mru_cnt[j];
									g_mru_cnt[j] = g_mru_cnt[j + 1];
									g_mru_cnt[j + 1] = temp_cnt;
									//swap the lru cnt
									int temp_lru_cnt = g_lru_cnt[j];
									g_lru_cnt[j] = g_lru_cnt[j + 1];
									g_lru_cnt[j + 1] = temp_lru_cnt;
								}
							}
						}
						//get the relaced block
						printf("the block %d is replaced\n", ghost_q[2]);
						ghost_q[2] = reference;
						//set the mru cnt for the new block
						g_mru_cnt[2] = 0;
						g_lru_cnt[2] = 0;
					}
				}
			}
			//end of the loop search
		}
	}

	
	//count the total of page fault
	stat.page_fault = page_fault;
	stat.hit_rate = (float)hit_cnt / reference_cnt;
	return stat;

}
void algorithm1(char *sim_file, char *output)
    {
		//test hash code
		if (HASH_SEARCH)
		{
			foo();
		}

		//test_func();
		//
        printf("this is the 1th algorithm\n");
		int q[num_blocks] = { 0 };
		int mru_cnt[num_blocks] = { 0 };
		int lru_cnt[num_blocks] = { 0 };
		int cnt = 0;
		page_fault = 0;
		hit_cnt = 0;
		reference_cnt = 0;
		trace_parser(sim_file, output);
        printf("output the result to %s\n", output);
    }








