# Lesson1 - The GPU Programming Model

+  Q1: To be power-efficient, what are we optimizing for?
  —— Minimizing latency. Latency is the amount of time to complete a task.
  —— Throughput. Throughput is tasks completed per unit time.

+ Q2: Core GPU Design tenets
  1. lost of simple compute units that together can perform a large amount of computation.
  2. GPUs have an explicitly parallel prgramming model.
  3. GPUs optimize for throughput,not latency.

+ Q3:A typical GPU Program
  1. cudaMalloc
  2. cudaMemcpy
  3. kernels launch
  4. cudaMemcpy
+ Q4:What is the GPU good at?
  1. efficienty launching lots of threads
  2. running lots of threads in parallel

+ Q5:GPU are good at Map
  1. GPUs have many parallel processors.
  2. GPUs if efficient at delegating the computation of individual element to those processors.
  3. GPU is optimized for throughput rather than latency.So as a programmer you're probably more interested in optimizing the rate at which entire map operations can complete. Instead of the time it takes for any individual element to complete.

# Lesson 2-GPU Hardware and Parallel

+ Map : Tasks read from and write to data elements. One to One. i->i
+ Gather : Each calculation gathers input data elements together from different places to compute an output result.
+ Scatter : One to multiple or to one. i->f(i)
          Each thread computing where to write its result.
+ Stencil: Tasks read input from a fixed neighborhood in an array. Update each element in an array using neighboring array elements in a fixed pattern called the stencil.
+ Transpose: Tasks re-order data elements in memory.
    AOS->SOA

## The GPU hardware

### Thread blocks：group of threads that cooperate to solve a subproblem.

+ SM: streaming multiprocesssor
  1. A SM in turn as many simple processors that can run a bunch of parallel threads.
  2. The imprtant thing to understand is that the GPU is responsible for allocating blocks to SMs.
  3. Give the GPU a big pile of thread blocks, and the GPU will take care of assigning them to run on the hardware SMs. All the SMs run in parallel and independently.
  4. `Advantage`
  Among the advantages that the GPU gains form this programing model is that the hardware can run things really efficiently,because it has so much flexibility.
  For example,if ont thread block completes quickly, the SM can immediately schedule another thread block without waiting for any others to complete.But the biggest advantage is scalability.Because you've made no guarantees about where the thread blocks will run or how many thread blocks might be running at a time. Importantly,the scalability also applies to future GPUs, you can be sure that GPUs wilil get more and more SMs, by writing the code in such a way that it can run on an arbitrary number of SMs and doesn't depend on a certain number of SM's a certain number ofthread blocks being resident at a time.
  `Consequences:`
  you can't make assumptions about bolcks will run no what SM,and you can't have any explicit communication between blocks.
  You can't simply have a thread that hangs around forever,processing input or doing something, because that thread must complete in order for the block that it's in to complete so that other threads and blocks can get scheduled onto that SM.

### CUDA guarantees that:
1. that all the threads in the block are guaranteed to run on the same SM at the same time.
2. all blocks in a kernel finish any blocks from the next kernel are launched.But `streams` will releax.

## Memory Model
1. All threads from a block can access the same variable in that blocks shared memory.
2. Threads from two different blocks can access the same variable in global memory.
3. Threads from different or same blocks have their own copy of local variables in local memory.

+ 1. CUDA is hierarchy of computation : threads thread blocks kernels
+ 2. with the corresponding hierarchy of memory spaces (local shared and global  even texture,that is not clear) and synchronization primitives ( sync threads barriers and implicit barrier in between 2 synchronous kernels).

## Writing Efficient Programs
1. Maximize Arithmetic intensity
   Math/Memory:the amount of math we do per amount of memory that we access.
  - maximizing the work we do per thread
  - minimizing the time spend on memory we do per thread
    + Move frequently accessed data to fast memory
    + Coalesce global memory accesses
     Whenever a thread on the GPU reads or writes global Memory, it always accesses a large chunk of memory at once. Even if that thread only needs to read or write a small subset of the data in that large chunk. This means the GPU is at its most efficient when threads read or write contiguous global memory locations. We say such an access pattern is `coalesced`.
     Global memory is going to be fastest when successive threads read or write adiacent locations in a continuous stretch of memory.
     strided pattern.
 2. Limitations of atomics
  + Only certain operations, data types.
    You can't do every operation. And only certain data types are supported. In particular,mostly integer types.
  + Still no ordering constraints.
    The differnet threads in a thread block and the different thread blocks thenselves will be running in some unspecified order. It will be different every time.
    The operations that we're performing on memory using a atomics are still going to happen in an unspecified order.
    This is important,because floating-point arithmetic is actually non-associative.  a+(b+c)!=(a+b)+c
  + GPU is still forcing each thread to take turns giving access to that memory.  It's serializing the access to memory among the different threads.

  3. Avoid thread divergence
    When parallel threads,like threads in our trusty thread block , do something different,we say they diverge.
    Due to the loop.

## Optimizing GPU Programs

APOD-analyze parallelize optimize deploy

Levels of Optimize Programs
1. Picking good Algorithms
Complexity
2. Basic principles for efficiency
Cache
3. Architecture-specific detailed optimizations
Block for the L1 cache (依据具体模型进行优化)
Vector register -SSE,AVX
4. Sort of bit twiddling micro-optimization at the instructions Level

### Amdahl's law
  The total speedup you get is limited by the portion of the time you spend doing something that can be parallelized.
max speedup = 1/(1-P) P=parallelizable time

### coalesced
  Whenever we see really low DRAM utilization or really low percentage bandwidth, our first guess is always coalescing.
  1. A way to think about coalescing is that the GPU is always accessing global memory. accessing the DRAM in pretty large chunks,32 or 128 bytes at a time.And this means that we are going to need the fewest total memory transactions when the threads in a warp access contiguous adjacent memory locations.
  If the threads in a warp are reading and writing completely random locations and memory,then we will go to get poor coalescing.So,a much more common access pattern is what's called strided, and this is where threads access a locationg memory that's a function of their thread ID times some stride.

### Littles law

  1. If the memory system can't deliver data to all of these processors and store results from all those processors,then we're not going to get the full speed out of our GPU.Our subgoal is really to utilize all the available memory bandwidth.Hence our foucus on global memory coalescing,DRAM utilization,and so on.
  2. Little's Law states that the number of bytes delivered equals the average latency of each memory transaction times the bandwidth.
   Let's be a little more precise and emphasize that we care about the useful bytes delivered, and the problem with global memory accesses is that not all of the bytes in every memory transaction are actually being used. That's why coalescing global memory accesses helps ensure that every byte delivered in a memory transaction will be used.
  + What can we do to improve bandwidth?
    Increase the number of bytes delivered;
    Decrease the latency (time between transactions)

Occupancy: the limit of the relation of (SM threads memory )

+ Control the amount of shared memory needed by a thread block.
+ Change the number of threads and thread blocks

This is always a tradeoff.For example,decreasing the tile size and the transpose code will let more thread blocks run.It decreases the amount of time spent waiting at barriers,but if we get to too small a tile.We will lose the whole point of tiling,which was to coalesce the global memory accesses.

+ compilation options to control register usage.

### Goal
+ maximize useful computation/second
1.  minimize time waiting at barriers;
2.  minimize thread divergence
>>warp: set of threads that execute the same instruction at a time
>>SIMD: the general technique is called SIMD,that stands for Single Instruction,Multiple Data.

 Branch divergence
 + Avoid branchy code
 Consider whether or not adjacent threads are likely to take different branches.
 + Beware large imbalance in thread workloads
 Sometimes 1 thread can take much, much longer than the average thread to complete a task,and when this happens, the long-running thread can hold up the rest of the warp or even the rest of the thread block.
 So look carefully at loops and recursive calls to decide if this kind of work imbalance might be hurting you.

### Assorted math optimizations

1. use double precision only when you mean it
  fp64>fp32
  float a = b+2.5 > float a = b+2.5f

2. use intrinsics when possible

### streams
