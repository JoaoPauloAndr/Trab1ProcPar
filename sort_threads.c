#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>

#define LENGTH 100

int array[LENGTH];

struct index_info{
    unsigned long begin;
    unsigned long end; 
};


int cmpfunc(const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

static void intercala(unsigned long p, unsigned long q, unsigned long r){
   unsigned long i, j;
   int *w;
   w = malloc ((r-p) * sizeof (int));

   for (i = p; i < q; ++i) w[i-p] = array[i];
   for (j = q; j < r; ++j) w[r-p+q-j-1] = array[j];
   i = 0; j = r-p-1;
   for (int k = p; k < r; ++k)
      if (w[i] <= w[j]) array[k] = w[i++];
      else array[k] = w[j--];
   free(w);
}

//n = end
void mergesort_i(unsigned long n, unsigned long begin){
   unsigned long b = 1;
   while (b < n) {
      unsigned long p = begin;
      while (p + b < n) {
         unsigned long r = p + 2*b;
         if (r > n) r = n;
         intercala (p, p+b, r);
         p = p + 2*b; 
      }
      b = 2*b;
   }
}

void* sortfunc(void* arg){
    struct index_info *indexes  = (struct index_info*) arg;
    mergesort_i(indexes->end, indexes->begin);
    //unsigned long      temp_len = indexes->end - indexes->begin;
    //int*               temp     = malloc(temp_len*sizeof(int));
    //unsigned long      i;
    //unsigned long      aux      = indexes->begin; 

    /*for(i = 0; i < temp_len; i++){
        temp[i] = array[aux];
        aux++;
    }

    qsort(temp, temp_len, sizeof(int), cmpfunc);

    aux = 0;
    for(i = indexes->begin; i < indexes->end; i++){
        array[i] = temp[aux];
        aux++;
    }*/

    //free(temp);
    pthread_exit(0);
}

int main(int argc, char **argv){
    unsigned long i;
    clock_t       t;
    double        cpu_time;
 
    srand(time(NULL));
    for(i = 0; i < LENGTH; i++){
        int num = rand()%1000;
        array[i]  = num;
        //printf("%d, ", num); 
    }

    t = clock();
    int k = atoi(argv[1]);

    //Criar threads e para ordenar o vetor
    do
    {
        pthread_t     tids[k];
        unsigned long fraction = LENGTH/(unsigned long)k;

        for(i = 0; i < k; i++){
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            struct index_info args;
            args.begin = i * fraction;
            args.end   = (i+1) * fraction;
            pthread_create(&tids[i], &attr, sortfunc, &args);
        }

        for(i = 0; i < k; i++){
            pthread_join(tids[i], NULL);
        }
        k = k/2;
    } while (k > 0);

    t        = clock() - t;
    cpu_time = ((double)t)/CLOCKS_PER_SEC;
    printf("\nTempo gasto: %fs\n", cpu_time);

    for(i = 0; i < LENGTH; i++){
        printf("%d, ", array[i]);
    }
    return 0;    
}