#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <time.h>

long int INSIDE=0;
long int N;
double PI;
void *routine(){
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
        }
    }
    PI = 4* INSIDE/(float)N;
    printf("%ld , %ld ,%f\n ",INSIDE, N, PI);
    pthread_exit(0); 
    return NULL; 

}

int main(int argc, char *argv[]) {
    printf("Enter the number of points:");
    scanf("%ld", &N);
    printf("You entered : %ld\n",N);
    if (N<1) {
        printf("Enter bigger Value");
        return -1;
    }
    pthread_t ChildThread;
    pthread_create(&ChildThread,NULL,routine,NULL);
    pthread_join(ChildThread,NULL) ;
    printf("%ld , %f",INSIDE, PI);
    return 0;

}
