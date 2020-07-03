//student name:马同庆 
//id number:201700130075 

#include <omp.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <iostream>
using namespace std;


// add your codes begin
/*
	The number of the threads;
	you can change it!
	线程个数  自行更改 
*/
#define NUM_THREADS 4 

	void updateRanks(vector<int> &Rin,vector<int> &Rout,vector<int> &N)
	{
		#pragma omp parallel for
			for(int i=0;i<SIZE;i++)
			{
				if(N[i]!=-1)
				{
					Rout[N[i]]=Rin[i]+Rin[N[i]];
				}
			}
	}
	
	void jumpList(vector<int> &Nin,vector<int> &Nout)
	{
		#pragma omp parallel for
			for(int i=0;i<SIZE;i++)
			{
				if(Nin[i]!=-1)
				{
					Nout[i]=Nin[Nin[i]];
				}
			}
	}

	void rankList(vector<int> &data,vector<int> &next)
	{
		vector<int> R1(data),R2(data);
		vector<int> N1(next),N2(next);
		
		for(int i=1;i<SIZE/5;i=i*2)
		{
			updateRanks(R1,R2,N1);
			jumpList(N1,N2);
			
		/*	R1.swap(R2); 
			N1.swap(N2);	It is wrong!  can't swap
		*/
			R1.assign(R2.begin(),R2.end());
			N1.assign(N2.begin(),N2.end());
		}
		
		data.assign(R2.begin(),R2.end());	
	}

// add your codes end


int main() {
  vector<int> data(SIZE, -1);
  vector<int> prev(SIZE, -1);
  vector<int> next(SIZE, -1);
  vector<int> test(SIZE, -1);

  srand(SIZE);
  { 
  	int tmp = -1;
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
  // add your codes begin
	
  omp_set_num_threads(NUM_THREADS);
  rankList(data,next);

  
  // add your codes end
  t = omp_get_wtime() - t;
  printf("time %f %d\n", t, SIZE);

  for (int i = 0; i < SIZE; i++) assert(data[i] == test[i]);

}

