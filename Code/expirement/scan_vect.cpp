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

void addScan(vector<int> &vec,int n,int m)
{

	if(n==1)
		return ;

	/*
		与递归部分结合算出所有偶数部分 
	*/ 
	#pragma omp parallel for
	for(int i=1;i<=n/2;i++)
	{
		if(m*i<vec.size())
			vec[m*i]=vec[m*i-m/2]+vec[m*i];
	}
	
	/*
		递归部分 
	*/	
	if(n&1)
		addScan(vec,(n>>1)+1,m*2);
	else
		addScan(vec,(n>>1),m*2);

	/*
		计算每次递归奇数部分 
	*/
	#pragma omp parallel for
	for(int i=2;i<=n/2;i++)
	{
		if(m*i-m/2<vec.size())
			vec[m*i-m/2]=vec[m*i-m]+vec[m*i-m/2];
	}

}

// add your codes end


int main() {
  vector<int> data(SIZE, 1);
  data[0] = 0;

  double t = omp_get_wtime();
  // add your codes begin
	omp_set_num_threads(NUM_THREADS);
	
	/*
		递归 
	*/ 
	addScan(data,SIZE,2);	
	
  // add your codes end
  t = omp_get_wtime() - t;
  printf("time %f %d\n", t, SIZE);

  for (int i = 0; i < SIZE; i++) assert(data[i] == i);
  
}

