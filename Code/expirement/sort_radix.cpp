//student name:ÂíÍ¬Çì 
//id number:201700130075 

#include <omp.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


// add your codes begin
#define SIZE 1000000

void Radix_sort(vector<int> &data)
{
	vector<int> temp0(SIZE);
	vector<int> temp1(SIZE);

	for(int i=0;i<31;i++)
	{	
		int index0=0,index1=0;
		int t=(1<<i);
	//	#pragma omp parallel for schedule(dynamic)
		for(int j=0;j<data.size();j++)
		{
			if(data[j]&t)
			{
				temp1[index1]=data[j];
			//	#pragma omp atomic
					index1++;
			}	
			else
			{
				temp0[index0]=data[j];
				//#pragma omp atomic
					index0++;
			}	
		}
		
		#pragma omp parallel for
		for(int i=0;i<index0;i++)
		{
			data[i]=temp0[index0];
		}
		#pragma omp parallel for
		for(int i=index0;i<SIZE;i++)
		{
			data[i]=temp1[i-index0];
		}
	}	
}


// add your codes end


int main() {
  vector<int> data(SIZE);

  srand(SIZE);
  for (int i = 0; i < SIZE; i++) data[i] = rand() % (SIZE * 10);

  double t = omp_get_wtime();
  // add your codes begin
  
  omp_set_num_threads(omp_get_max_threads());
  Radix_sort(data);
  
  // add your codes end
  t = omp_get_wtime() - t;
  printf("time %f %d\n", t, SIZE);

  for (int i = 0; i < SIZE-1; i++) assert(data[i] <= data[i+1]);
}

