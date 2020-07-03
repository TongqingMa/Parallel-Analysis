//student name:ÂíÍ¬Çì 
//id number:201700130075 

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;


// add your codes begin

#define SIZE 1000000

vector<int> visit(SIZE, SIZE+10);

	int lencircle(vector<int> &next,int begin)
	{
		int front=begin;
		int len=1;	visit[begin]=front;
		while(next[begin]!=front)
		{
			len++;
			begin=next[begin];
			if(visit[begin]<front)
				return -1;
			else
				visit[begin]=front;
		}
		return len;
	}
	void findcircle(vector<int> &data,vector<int> &next)
	{
		vector<int> result;
		#pragma omp parallel for
		for(int i=0;i<SIZE;i++)
		{
			int len;
			if((visit[i]==SIZE+10)&&data[i]>0)
			{
				len=lencircle(next,i);
				if(len>0)
				{
					#pragma omp critical 
					result.push_back(len);
				} 	
			}
		}
		data.assign(result.begin(),result.end());
	}
// add your codes end


int main() {
  vector<int> data(SIZE, -1);
  vector<int> prev(SIZE, -1);
  vector<int> next(SIZE, -1);
  vector<int> test;

  srand(20200218);
  { int empty = SIZE / 10;
    int head, tail, tmp;
    while (empty > 0) {
      int size = rand() % empty + 1;
      for (int i = 0; i < size; i++) {
        int idx = rand() % SIZE;
        while (data[idx] >= 0) idx = (idx + 1) % SIZE;

        data[idx] = 1;
        if (i == 0) {
          head = idx;
          tail = idx;
        } else if (i == size-1) {
          prev[idx] = tmp; next[tmp] = idx;
          tail = idx;
        } else {
          prev[idx] = tmp; next[tmp] = idx;
        }

        tmp = idx;
      }
      prev[head] = tail; next[tail] = head;
      test.push_back(size);
      empty -= size;
    }
    sort(test.begin(), test.end());
  }

  double t = omp_get_wtime();
  // add your codes begin
  
  	omp_set_num_threads(omp_get_max_threads());
	findcircle(data,next);
  
  // add your codes end
  t = omp_get_wtime() - t;
  printf("time %f %d\n", t, SIZE);

  sort(data.begin(), data.end());
  printf("circle");
  for (int i = 0; i < data.size(); i++) printf(" %d", data[i]);
  printf("\nsize %d\n", int(data.size()));
  assert(data.size() == test.size());
  for (int i = 0; i < test.size(); i++) assert(data[i] == test[i]);
}

