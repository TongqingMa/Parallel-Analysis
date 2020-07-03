// K = 2^k
#define K 128
// MAX_LENGTH = 2^k - 2
#define MAX_LENGTH 30 
// INF >> -log(0.01)
#define INF 1e8

#include <omp.h> 
//#include <openacc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <algorithm>
using namespace std;


struct Edge {
  int id, prev, next;
  float score;
};

struct Path {
  float score;
  int length;
  int edge[MAX_LENGTH];
};

// add your codes begin


bool operator == (Path a,Path b)
{
	if(a.score==b.score&&a.length==b.length)
	{
		for(int i=0;i<a.length;i++)
		{
			if(a.edge[i]!=b.edge[i])
				return false;
		}
		return true;
	}
	else
		return false;
}

bool cmp_next(Edge a, Edge b)
{
	return a.next<b.next;
}

bool cmp_id(Edge a, Edge b)
{
	return a.id<b.id;
}

#define NUM_NODES 10000
#define NUM_EDGES 20

// add your codes end


int main() {
  const int size = NUM_NODES * NUM_EDGES;
  Edge* graph = new Edge[size];
  int resource = 0, product = 0;
  Path* result = new Path[K]();
  for (int i = 0; i < K; i++) result[i].score = -INF;

  srand(size);
  for (int i = 0; i < size; i++) {
    graph[i].id = i;
    graph[i].prev = rand() % NUM_NODES;
    graph[i].next = rand() % NUM_NODES;
    if (graph[i].prev == graph[i].next) {
      i--;
      continue;
    }
    graph[i].score = log(float(rand() % 100) / 100);
    if (graph[i].score < -INF) {
      i--;
      continue;
    }
  }
  while (resource == product) {
    resource = rand() % NUM_NODES;
    product = rand() % NUM_NODES;
  }

  double t = omp_get_wtime();
  // add your codes begin
  
  sort(graph, graph+size, cmp_next);
  
  #pragma acc data copyin(graph[0:size]) copy(result[0:K],resource,product)
  {
	int in[NUM_NODES];
	int out[NUM_NODES];
	int vis[NUM_NODES];
	int nvis[NUM_NODES];

	for(int i=0;i<size;i++)
	{
		out[graph[i].next]=i;
		in[graph[size-i-1].next]=size-i-1;
	}
	
	#pragma acc parallel loop
	for(int i=0;i<NUM_NODES;i++)
	{
		nvis[i] = 0;
		vis[i] = 0;
	}
	
	vis[resource] = 1;
	
	Path last[NUM_NODES][K];
	
	for(int LEV=0;LEV<30;LEV++)
	{
		#pragma acc parallel loop 
		for(int i = 0;i<NUM_NODES;i++)
		{
			for(int j = in[i]; j <= out[i]; j++)
			{
				// this two loops are to  parallel for each edge
				if(vis[graph[j].prev])
				{
					Path tresult[K]; int m=0,n=0;
					 
					for(int k=0; k < K ; k++)
					{
						if(graph[j].prev != resource &&  last[graph[j].prev][m].score ==0)
							m=K;
						if( last[i][n].score==0 )
							n=K;
						
						Path temp = last[graph[j].prev][m];
						temp.edge[temp.length] = graph[j].id;
						temp.score = temp.score+graph[j].score;
						temp.length++;
						
						if(m<K&&n<K)
						{
							if(temp.score > last[i][n].score)
							{
								tresult[k]=temp;	m++;
								nvis[i]=1;
							}
							else if(temp.score < last[i][n].score)
							{
								tresult[k]=last[i][n];	n++;	
							}
							else
							{
								if(temp==last[i][n])
								{
									tresult[k]=temp;	m++;	n++;
									nvis[i]=1;
								}
								else
								{
									tresult[k]=temp;	m++;
									nvis[i]=1;
								}
							}
						}
						else
						{
							if(m<K)
							{
								tresult[k]=temp;	nvis[i]=1;	m++;	
							}
							
							if(n<K)
								tresult[k]=last[i][n++];
						}
						
						if(graph[j].prev == resource && last[graph[j].prev][k].score ==0)
							m=K;			
					}
					
					for(int k=0;k<K;k++)
					{
						if(tresult[k].score==0)
							break;
						last[i][k]=tresult[k];
					} 
				}
			}
		}
		
		
		#pragma acc parallel loop 
		for(int i = 0;i<NUM_NODES;i++)
		{
			vis[i] = nvis[i];
			nvis[i] = 0;
		}
		
		#pragma acc parallel loop 
		for(int i=0;i<K;i++)
			result[i] = last[product][i];		
	}
  } 
 
  sort(graph, graph+size, cmp_id);
  
  
  // add your codes end
  t = omp_get_wtime() - t;
  printf("time %f %d %d\n", t, NUM_NODES, NUM_EDGES);

  for (int k = 0; k < K; k++) {
    Path p = result[k];
    if (p.score <= -INF) break;

    float s = 0;
    printf("p:%d n:%d", k, graph[p.edge[0]].prev);
    for (int l = 0; l < p.length; l++) {
      assert(graph[p.edge[l]].id == p.edge[l]);
      printf(" e:%d:%f n:%d", p.edge[l], graph[p.edge[l]].score, graph[p.edge[l]].next);
      s += graph[p.edge[l]].score;
    }
    printf(" l:%d c:%f\n", p.length, p.score);
    assert(s == p.score);
    if (k > 0) assert(p.score <= result[k-1].score);
  }
}

