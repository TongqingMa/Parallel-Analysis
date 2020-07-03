//Name:ÂíÍ¬Çì
//ID:201700130075 

#define INF 1e7

#include <omp.h>
#include <openacc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <algorithm>
using namespace std;


inline int index(const int i, const int j) {
  return i * SIZE + j;
}

inline float max(const float i, const float j)
{
	if(i>j)
		return i;
	else
		return j;
}
// add your codes begin
// add your codes end


int main() {
  const int size2 = SIZE * SIZE;
  float* data = new float[size2];
  for (int i = 0; i < size2; i++) data[i] = -INF;

  srand(SIZE);
  for (int i = 0; i < SIZE*20; i++) {
    int prev = rand() % SIZE;
    int next = rand() % SIZE;
    if ((prev == next) || (data[index(prev, next)] > -INF)) {
      i--;
      continue;
    }
    data[index(prev, next)] = log((rand() % 99 + 1.0) / 100);
  }

  double t = omp_get_wtime();
  // add your codes begin
  			
	float *row=new float[SIZE];
	float *colum=new float[SIZE];	
		
		//loop
		#pragma omp parallel for
		for(int m=0;m<SIZE;m++)
			row[m]=data[index(0,m)];
		//loop
		#pragma omp parallel for	
		for(int n=0;n<SIZE;n++)
			colum[n]=data[index(n,0)];
	
  	
  	int block_num=6;
  	omp_set_num_threads(block_num);  	
	int block_width=SIZE/block_num;
	int my_num=acc_get_num_devices(acc_device_nvidia);
	printf("#num_dev:%d \n#num_thread:%d\n",my_num,block_num);

    #pragma omp parallel
	{
	  int my_gpu=omp_get_thread_num();
	  acc_set_device_num(my_gpu,acc_device_nvidia);//acc_get_device_type()); 

 		int start=my_gpu*block_width;
 		int end=start+block_width;
 		if(end+block_width>SIZE)
			end=SIZE;

		printf("#device_id:%d   %d   %d   %d\n",my_gpu,start,end,end-start);
		
		//if start and end should be created in GPU?
	#pragma acc data create(data[0:size2],row[0:SIZE],colum[0:SIZE])
	{
		#pragma acc update device(data[0:size2])
			
		for(int k=0;k<SIZE;k++)
		{		
			#pragma acc update device(row[0:SIZE],colum[start:end-start])
			#pragma omp barrier
			
			//loop
			#pragma acc parallel loop collapse(2) //gang vector  
			for(int id_i=start;id_i<end;id_i++)
			{
				for(int id_j=0;id_j<SIZE;id_j++)
				{
					data[index(id_i,id_j)]=max(colum[id_i]+row[id_j],data[index(id_i,id_j)]);
				}
			}
			
			#pragma omp barrier	
			
			if((k+1<SIZE)&&(k+1>=start)&&(k+1<end))
			{
				//loop
				#pragma acc parallel loop 
				for(int m=0;m<SIZE;m++)
					row[m]=data[index(k+1,m)];
				#pragma acc update self(row[0:SIZE])
			}
			
			if(k+1<SIZE)	
			{
				//loop
				#pragma acc parallel loop 
				for(int n=start;n<end;n++)
					colum[n]=data[index(n,k+1)];
				#pragma acc update self(colum[start:end-start])
			}
			
			#pragma omp barrier	
			
		}
		
		#pragma acc update self(data[start*SIZE:(end-start)*SIZE])
		
		#pragma acc wait
		#pragma omp barrier
		
		#pragma acc exit data delete(data[0:size2])
	}
		
}

  // add your codes end
  t = omp_get_wtime() - t;
  printf("time %f %d\n", t, SIZE);

  for (int i = 0; i < 20; i++) {
    int prev = rand() % SIZE;
    int next = rand() % SIZE;
    if (prev == next) {
      i--;
      continue;
    }
    printf("test %d %d %f\n", prev, next, data[index(prev, next)]);
  }
  delete data;
}

