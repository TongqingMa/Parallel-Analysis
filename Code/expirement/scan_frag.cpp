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
*/
#define NUM_THREADS 4 

struct Op
{
	Op(int a,bool b)
	{
		value=a;
		f=b;
	}
	int value;
	bool f;
};

Op operator + (const Op A,const Op B)
{
	if(!B.f)
		return Op(A.value+B.value,A.f|B.f);
	else
		return B;
}

void SegAddScan(vector<Op> &vec,int n,int m)
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
		SegAddScan(vec,(n>>1)+1,m*2);
	else
		SegAddScan(vec,(n>>1),m*2);

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
  vector<bool> flag(SIZE, false);
  vector<int> test(SIZE);

  srand(SIZE);
  data[0] = 0; flag[0] = true;
  for (int i = 0; i < flag.size() / 12; i++) {
    int index = rand() % flag.size();
    data[index] = 0; flag[index] = true;
  }
  for (int i = 0; i < data.size(); i++) test[i] = flag[i] ? data[i] : test[i-1] + data[i];

  double t = omp_get_wtime();
  // add your codes begin
  
    omp_set_num_threads(NUM_THREADS);
    
    /*
		构建新类型集合 pair 
	*/ 
  	vector<Op> vec;
  	for(int i=0;i<data.size();i++)
	{
		vec.push_back(Op(data[i],flag[i]));
	}
	
	/*
		递归 
	*/ 
	SegAddScan(vec,SIZE,2);
	
	/*
		得到结果 
	*/ 
	for(int i=0;i<data.size();i++)
	{
		data[i]=vec[i].value;
	}
  
  // add your codes end
  t = omp_get_wtime() - t;
  printf("time %f %d\n", t, SIZE);

  for (int i = 0; i < SIZE; i++) assert(data[i] == test[i]);
}

