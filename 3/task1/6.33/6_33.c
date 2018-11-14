#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <time.h>

#define NUM_THREAD 8
long int INSIDE=0;
long int N;
double PI;
pthread_mutex_t lock;

void *routine(){

    pthread_mutex_lock(&lock);        
    int x,y,i = 0;
    srand(time(NULL));
    float xf,yf,result;
    for (i=0;i<N;i++){
        x= rand();
        y= rand();
        xf = pow(-1,x)* x / (float) RAND_MAX;
        yf = pow(-1,y)* y / (float) RAND_MAX;
     //   printf("%f , %f ",xf,yf);
        result = sqrt(xf*xf+yf*yf); 
        if (result<1){
            INSIDE++;
//            printf("%ld\n",INSIDE);
        }
    }
    pthread_mutex_unlock(&lock);
    pthread_exit(0); 
    return NULL; 

}

int main(int argc, char *argv[]) {
     if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    printf("Enter the number of points:");
    scanf("%ld", &N);
    printf("You entered : %ld\n",N);
    if (N<1) {
        printf("Enter bigger Value");
        return -1;
    }
    pthread_t threads[NUM_THREAD];
    int i = 0;
    for (i=0;i<NUM_THREAD;i++){
        pthread_create(&threads[i],NULL,routine,NULL);
    }
    
    
    for (i=0;i<NUM_THREAD;i++){
        pthread_join(threads[i],NULL) ;
    }
    pthread_mutex_destroy(&lock);
    PI = 4* INSIDE/(float)(N*NUM_THREAD);
    printf("%ld , %ld ,%f\n ",INSIDE, N*NUM_THREAD, PI);
    
    return 0;

}
