#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>




int iSIZE, SIZE=2;
long long int Fibbonachi_cache[1000]= {0,1};
int Fibbonachi_index[1000]= {0,1};

bool isvalueinarray(int val, int *arr, int size){
    int i;
    for (i=0; i < size; i++) {
        if (arr[i] == val)
            return true;
    }
    return false;
}

void *fib(void *s){
    int ss = (intptr_t) s;
    long long int newval;
    if (isvalueinarray(ss,Fibbonachi_index,SIZE)){
        return (void *) (intptr_t)Fibbonachi_cache[ss];
    } else {
        newval = (intptr_t) fib( (void *) (intptr_t) (ss-2))+(intptr_t)fib((void *) (intptr_t) (ss-1));
        if( !(newval<0)){
            SIZE++;
            Fibbonachi_index[ss] = ss;
            Fibbonachi_cache[ss] = newval;
        }else {
            printf("Too Big to Store");
        }
    }
    return (void *) (intptr_t) Fibbonachi_cache[ss];
}


int main(int argc, char *argv[]) {
    iSIZE = atoi(argv[1]);
    pthread_t child;
    pthread_create(&child, NULL, fib,(void *) (intptr_t) iSIZE);
    pthread_join(child,NULL);
    int  i =0;
    for (i=0; i<SIZE;i++){
        printf("%d %lld\n ",i,Fibbonachi_cache[i]);
    }
    printf("The end.\n");
}


