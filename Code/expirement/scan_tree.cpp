//student name:马同庆 
//id number:201700130075 
/*
	采用scan_link 程序中的思想，只不过对一条链表的操作改成对一棵从叶子结点到根节点的路径
	与上一程序的函数结构相同，不过有一些需要注意的点 已经标出。------------------------ 
*/
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
#define NUM_THREADS 1 

bool hasGrandparent(int k,vector<int> &pare)
{
	return (k>=0)&&(pare[k]>=0)&&(pare[pare[k]]>=0);
	/*
		there must be >=  !
		keep in mind! 
	*/
} 


void updateRanks(vector<int> &Rin,vector<int> &Rout,vector<int> &pare)
{
	#pragma omp parallel for
		for(int i=0;i<SIZE;i++)
		{
			if(pare[i]!=-1)
			{
				Rout[i]=Rin[i]+Rin[pare[i]];
			}
		}
}

void adopt(vector<int> &pare,vector<int> &Gpare)
{
	#pragma omp parallel for
		for(int i=0;i<SIZE;i++)
		{
			if(hasGrandparent(i,pare))
			{
				Gpare[i]=pare[pare[i]];
			}
			else
			{
				Gpare[i]=pare[i];
			}
		}
}

void findRoots(vector<int> &data,vector<int> &pare)
{
		vector<int> R1(data),R2(data);
		vector<int> P1(pare),P2(pare);
		
		for(int i=1;i<SIZE*10;i=i*2)
		{
			updateRanks(R1,R2,P1);
			adopt(P1,P2);

			R1.assign(R2.begin(),R2.end());
			P1.assign(P2.begin(),P2.end());
		}
		
		data.assign(R2.begin(),R2.end());	
}

// add your codes end


int main() {
  vector<int> data(SIZE, -1);
  vector<int> pare(SIZE, -1);
  vector<int> test(SIZE, -1);

  srand(SIZE);
  { 
  	vector<int> tmp;
    for (int i = 0; i < SIZE/10; i++) {
      int idx = rand() % SIZE;
      while (data[idx] >= 0) idx = (idx + 1) % SIZE;
      if (i > 0) {
        data[idx] = 1;
        pare[idx] = tmp[rand() % tmp.size()];
        test[idx] = test[pare[idx]] + data[idx];
      } else {
        data[idx] = 0;
        test[idx] = data[idx];
      }
      tmp.push_back(idx);
    }
  }

  double t = omp_get_wtime();
  // add your codes begin
	
  omp_set_num_threads(NUM_THREADS);  	
  findRoots(data,pare);

  // add your codes end
  t = omp_get_wtime() - t;
  printf("time %f %d\n", t, SIZE);

  for (int i = 0; i < SIZE; i++) assert(data[i] == test[i]);
}

