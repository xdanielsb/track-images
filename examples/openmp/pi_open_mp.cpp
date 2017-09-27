#include <iostream>
#include <chrono>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#define SUCCESS 0
#define PAD 8
#define THREADS 2
#define iters 1e9

//  g++ -std=c++11 pi_open_mp.cpp -fopenmp -o openmp

using namespace std;


int sinterval;
typedef long long ll;
double  pi_res = 0;
double  pi[THREADS][PAD] ;

inline void calc_pi (int nthread){  
  //compute the positions
	ll init = sinterval*nthread+1;
	ll end = init+sinterval;

  //compute value
	for (ll i=init; i< end; i+=4)
    pi[nthread][0] += 4.0/i - 4.0/(i+2);

}

inline void create_threads(){
  //Create the threads
  #pragma omp parallel num_threads(THREADS) 
  { // este corchete tiene que estar en una linea diferente
    int id = omp_get_thread_num();
    //Call the function to call pi
    calc_pi(id);
  }
  //Add the answer in the array
  for (int i =0; i < THREADS; i++)
    pi_res += pi[i][0];

}

int main(int argc, char** argv){


    // Start the counter
    auto start = chrono::high_resolution_clock::now();

    //Calc the interval
    sinterval = (iters/THREADS)*2;

    // Create the threads
    create_threads();

    // Finish the counter
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end-start;
  
    //Show the number of counters
    cout << THREADS <<", "<< diff.count()<<endl;

    //Show the answer
    printf(" The value of pi is %1.30f \n", pi_res);
  
    
    return 0;
}

