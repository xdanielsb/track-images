#include "omp.h"
#include <stdio.h>
/*
 * Hello world program en OPENMP
 * how to compile it? g++ name.cpp -o sal -fopenmp export omp_num_threads=4
 */

int main(){
  #pragma omp parallel num_threads(4) 
  { // este corchete tiene que estar en una linea diferente
    int id = omp_get_thread_num();
    printf("hello %d", id);
  }
  return 0;
}
