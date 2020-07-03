//student name:ÂíÍ¬Çì 
//id number:201700130075 

#include <omp.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>
using namespace std;

 
// add your codes begin
#define SIZE 1000000
bool visit[SIZE+10]={0};

void find_prime(vector<int> &vec)
{
	int sqrt_SIZE=(int)sqrt(SIZE);
	//dynamic model is more efficient to run with the increasing of the threads num;
	//More details will be provided in the TXT document. 
	#pragma omp parallel for schedule(dynamic)
	for(int i=2;i<=sqrt_SIZE;i++){
		if(!visit[i])
		{
			for(int j=i;i*j<SIZE;j++)
				visit[i*j]=true;		
		}	
	}
	
//	#pragma omp parallel for
	for(int i=3;i<SIZE;i++)
	{
		if(!visit[i])
		{
		//	#pragma omp critical
			vec.push_back(i); 
		}		
	}	
}
// add your codes end


int main() {
  vector<int> prime;
  prime.push_back(2);

  double t = omp_get_wtime();
  // add your codes begin
  omp_set_num_threads(omp_get_max_threads());
  find_prime(prime); 
  // add your codes end
  t = omp_get_wtime() - t;
  printf("time %f %d\n", t, SIZE);

  sort(prime.begin(), prime.end());
  printf("prime");
  for (int i = 0; i < prime.size(); i++) printf(" %d", prime[i]);
  printf("\nsize %d\n", int(prime.size()));
}

