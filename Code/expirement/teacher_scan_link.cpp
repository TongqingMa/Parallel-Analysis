//student name:马同庆 
//id number:201700130075 

#include <omp.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <iostream>
using namespace std;


// add your codes begin
#define SIZE 100

void scan(vector<int>& data, vector<int> ptr, vector<int> tmpdata, vector<int> tmpptr) {
  bool done = false;
  while (!done) {
    done = true;
    #pragma omp parallel for reduction(and:done)
    for (int i = 0; i < SIZE; i++) {
      if (ptr[i] >= 0) {
        tmpdata[i] = data[i] + data[ptr[i]];
        tmpptr[i] = ptr[ptr[i]];			// difference is here!  往后加  而不是往前加！ Be careful 
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
	
	for(auto i:data)
	{
		cout<<i<<' ';
		}
	cout<<"\n\n\n";	
	
  omp_set_num_threads(omp_get_max_threads());
  scan(data,prev,data,prev);
  
  	for(auto i:data)
	{
		cout<<i<<' ';
		}
	cout<<"\n\n\n";
	for(auto i:test)
	{
		cout<<i<<' ';
		}
	cout<<"\n\n\n";
  
  // add your codes end
  t = omp_get_wtime() - t;
  printf("time %f %d\n", t, SIZE);

  for (int i = 0; i < SIZE; i++) assert(data[i] == test[i]);

}

