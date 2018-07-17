#include <stdio.h>
#include <iostream>
#include <chrono>
#include <omp.h>
#define THREADS 2
//#define iters 1e9

using namespace std;

double pi =0.0;


inline void create_threads(){
  //Create the threads
  int iters = 1e9;
  #pragma omp parallel for  reduction (+:pi)
  for (int i =1; i < iters; i+=4){
    pi += 4.0/i - 4.0/(i+2);
  }
}


int main(){

    // Start the counter
    auto start = chrono::high_resolution_clock::now();


    create_threads();
    printf(" The value of pi is %1.30f \n", pi);

    // Finish the counter
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end-start;

    //Show the number of counters
    cout << THREADS <<", "<< diff.count()<<endl;
}

