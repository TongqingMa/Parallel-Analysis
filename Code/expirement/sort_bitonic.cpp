//student name:马同庆 
//id number:201700130075 

/*
	这是更新完善版本代码，这个为最终代码。 
*/ 

#define CUTOFF 32

#include <omp.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


// add your codes begin 
 
 #define SIZE 1000000
 
void bitonicSplit(vector<int> &vec,int begin,int end,int type)
{
	int n= end-begin+1;
	//#pragma omp parallel for //Be careful  the cache  
	for(int i=begin;i<begin+n/2;i++)
	{
		int a=vec[i];	int b=vec[i+n/2];//Be careful a&b must be private
		if((type && a > b)||(!type && a <= b))
		{
			vec[i]=b;
			vec[i+n/2]=a;
		}
	}
}
 
 void bitonicMerge(vector<int> &vec,int begin,int end,bool type)
 {
 	int n=end-begin+1;
 	if(n>=2)
 	{
 		for(int i=n;i>=2;i=i/2)
 		{
 		//	#pragma omp parallel for //Be careful for cache
 			for(int j=begin;j<end;j+=i)
 			{
 				bitonicSplit(vec,j,j+i-1,type);	
			}	
		}	
 	/*	bitonicSplit(vec,begin,end,type);
 		bitonicMerge(vec,begin,begin+n/2-1,type);
 		bitonicMerge(vec,begin+n/2,end,type);
	*/
 	}
 }
 
 void genBitonic(vector<int> &vec,int begin,int end)
 {
 	int n=end-begin+1;
 	if(n>=2)
 	{
		for(int i=4;i<=n;i=i*2)
		{
			#pragma omp parallel for 
			for(int j=begin;j<end;j+=i)
			{
				bitonicMerge(vec,j,j+i/2-1,true);
				bitonicMerge(vec,j+i/2,j+i-1,false);
			}
		}	
	/*	
	 	genBitonic(vec,begin,begin+n/2-1);
		genBitonic(vec,begin+n/2,end);
		bitonicMerge(vec,begin,begin+n/2-1,true);
		bitonicMerge(vec,begin+n/2,end,false);	
	*/
	}
 } 

 void bitonicSort(vector<int> &vec)
 {
 	genBitonic(vec,0,vec.size()-1);
 	bitonicMerge(vec,0,vec.size()-1,true);
 }
// add your codes end


int main() {
  vector<int> data(SIZE);

  srand(SIZE);
  for (int i = 0; i < SIZE; i++) data[i] = rand() % (SIZE * 10);

  double t = omp_get_wtime();
  // add your codes begin
  	int i=1;
	do{
		i*=2;
	}while(i<SIZE);
	data.resize(i);
  omp_set_num_threads(omp_get_max_threads());
  bitonicSort(data);
  // add your codes end
  t = omp_get_wtime() - t;
  printf("time %f %d\n", t, SIZE);

  for (int i = 0; i < SIZE-1; i++) assert(data[i] <= data[i+1]);
}

