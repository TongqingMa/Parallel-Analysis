//student name:ÂíÍ¬Çì 
//id number:201700130075 

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <iostream>
using namespace std;


// add your codes begin

void ParIndSet(vector<int> next,vector<int> &GI,long seed[][8])
{
	vector<bool> Coin(SIZE),Coins(SIZE);
	#pragma omp parallel for
		for(int i=0;i<SIZE;i++)
		{
			srand(seed[omp_get_thread_num()][0]+i);
			Coins[i]=Coin[i]=rand()%2;	
		}
		
	#pragma omp parallel for
		for(int i=0;i<SIZE;i++)
		{
			if(Coins[i]&&next[i]>=0&&Coins[next[i]])
				Coin[i]=false;	
		}	
	
	for(int i=0;i<SIZE;i++)
	{
		if(Coin[i])
			GI.push_back(i);
	}
}

void deletenode(vector<int> &GI,vector<int>& data, vector<int> &next,vector<int> &prev)
{
	
	#pragma omp parallel for
	for(int i=0;i<GI.size();i++)
	{
		if(data[GI[i]]>=0)
		{
			int dn=GI[i];
			if(next[dn]>=0&&prev[dn]>=0)
			{
				data[next[dn]]+=data[dn];
				next[prev[dn]]=next[dn];
				prev[next[dn]]=prev[dn];
			}
			else if(next[dn]<0)
			{
				next[prev[dn]]=next[dn];
			}
			else if(prev[dn]<0)
			{
				data[next[dn]]+=data[dn];
				prev[next[dn]]=prev[dn];
			}
			data[dn]=-1;
			prev[dn]=-1;
			next[dn]=-1;
		}
	}
}

// add your codes end

void scan(vector<int>& data, vector<int> ptr, vector<int> tmpdata, vector<int> tmpptr) {
  bool done = false;
  while (!done) {
    done = true;
    #pragma omp parallel for reduction(and:done)
    for (int i = 0; i < SIZE; i++) {
      if (ptr[i] >= 0) {
        tmpdata[i] = data[i] + data[ptr[i]];
        tmpptr[i] = ptr[ptr[i]];
        done = false;
      }
    }
    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++) {
      data[i] = tmpdata[i];
      ptr[i] = tmpptr[i];
    }
  }
}


int main() {
  vector<int> data(SIZE, -1);
  vector<int> prev(SIZE, -1);
  vector<int> next(SIZE, -1);
  vector<int> test(SIZE, -1);

  srand(20200218);
  { int tmp = -1;
    for (int i = 0; i < SIZE/10; i++) {
      int idx = rand() % SIZE;
      while (data[idx] >= 0) idx = (idx + 1) % SIZE;
      if (i > 0) {
        data[idx] = 1;
        prev[idx] = tmp;
        next[tmp] = idx;
      } else {
        data[idx] = 0;
      }
      test[idx] = i;
      tmp = idx;
    }
  }

  double t = omp_get_wtime();
  long seed[omp_get_max_threads()][8];
  for (int i = 0; i < omp_get_max_threads(); i++) seed[i][0] = rand();

  for (int round = 0; round < 4; round++) {
    // add your codes begin
    omp_set_num_threads(omp_get_max_threads());
    vector<int> GI;
    ParIndSet(next,GI,seed);
    deletenode(GI,data,next,prev);
    
    // add your codes end
  }
  t = omp_get_wtime() - t;
  printf("time %f %d\n", t, SIZE);

  int ndel = 0;
  int nkeep = 0;
  scan(data, prev, data, prev);
  
  for (int i = 0; i < SIZE; i++) {
    if (data[i] < 0) {
      if (test[i] >= 0) ndel++;
    } else {
      assert(data[i] == test[i]);
      nkeep++;
    }
  }
  assert(ndel > 0);
  printf("delete %d %.1f%%\n", ndel, 100.0 * ndel / (nkeep + ndel));
}

