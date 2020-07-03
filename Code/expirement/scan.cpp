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
#define SIZE 1000000000 

void addScan(vector<int> &vec,int n)
{
	int m=1;
	bool done=false;
	while(!done)
	{
		m=m*2;	done=true;
		#pragma omp parallel for reduction(and:done)
			for(int i=m-1;i<n;i=i+m)
			{
				vec[i]=vec[i]+vec[i-m/2];
				done=false;
			}
	}
	while(m>2)
	{
		m=m/2;
		#pragma omp parallel for
			for(int i=m-1;i<n;i=i+m)
			{
				if(i+m/2<n)
				{
					vec[i+m/2]=vec[i+m/2]+vec[i];
				}
			}
	}
}

// add your codes end


int main() {
  vector<int> data(SIZE, 1);
  data[0] = 0;

  double t = omp_get_wtime();
  // add your codes begin
	omp_set_num_threads(NUM_THREADS);

	addScan(data,SIZE);	
	
  // add your codes end
  t = omp_get_wtime() - t;
  printf("time %f %d\n", t, SIZE);

  for (int i = 0; i < SIZE; i++) assert(data[i] == i);
  
}

