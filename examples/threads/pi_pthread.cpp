#include <iostream>
#include <chrono>
#include <stdio.h>
#include <math.h>
#include <pthread.h> /* pthread */
#define SUCCESS 0
#define iters 1e9
/*
 * Compile and link with -pthread.
 * g++ -pthread -std=c++11 pi_pthread.cpp
 */

using namespace std;

double static pi =0;
int THREADS ;
int sinterval;
typedef long long ll;


void * function (void *th){
	int nthread = *(int*)th;
    double four =4.0, aux =0.0;

    //compute the positions
  	ll init = sinterval*nthread+1;
  	ll end = init+sinterval;

    //compute value
	  for (ll i=init; i< end; i+=4){
      aux += four/i - four/(i+2);
    }

    pi += aux;

}

void create_threads(){
    pthread_t hilo[THREADS];
    int *values = new int[THREADS];
    for (int i=1; i<=THREADS; i++)
        values[i-1] = i-1;

    int result;
    int arg =1,c; /*  c -> control variable */

     for (int i =0; i< THREADS; i++)
         result = pthread_create(&hilo[i], NULL,  function, (void*) &values[i]);
     for (int i =0; i< THREADS; i++)
        result = pthread_join (hilo[i], NULL); //Wait until the threads finish

     
}

int main(int argc, char** argv){


    THREADS =2;
    
    pi=0;
    
    auto start = chrono::high_resolution_clock::now();
    sinterval = (iters/THREADS)*2;
    
    create_threads();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end-start;
  
    cout << THREADS <<","<< diff.count()<<endl;
    printf(" The value of pi is %1.30f \n", pi);
    
    return 0;
}

