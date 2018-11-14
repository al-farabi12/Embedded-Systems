#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


int average =0;
int size =0;
int minimum;
int maximum;

void *avT(void *arg);
void *minim(void *arg);
void *maxim(void *arg);
int main(int argc, char *argv[]) {
    if (argc<2){
        printf("Provide list of Numbers \n");
        return -1;
    }

    int *nums = (int*) malloc((argc-1)*sizeof(int));



    int i;
    for (i=1; i<argc;i++){
        nums[i-1]=atoi(argv[i]);
        size++;
    }
    pthread_t avrThread;
    pthread_t maximThread;
    pthread_t minimThread;
    pthread_create(&avrThread,NULL,avT,(void *)nums);
    pthread_create(&maximThread,NULL,maxim,(void *)nums);
    pthread_create(&minimThread,NULL,minim,(void *)nums);
    pthread_join(avrThread,NULL);
    pthread_join(maximThread,NULL);
    pthread_join(minimThread,NULL);
    printf("The average value is %d\n",average);
    printf("The minimum value is %d\n",minimum);
    printf("The maximum value is %d\n",maximum);
    free(nums);
}


void *avT(void *arg){
    int *val = (int *) arg;
    int sum = 0;
    int i=0;
    for (i=0;i<size; i++){
        sum += val[i];
    }
    average = sum /(size);
    pthread_exit(0);
}
void *minim(void *arg){
    int *values = (int *) arg;
    minimum = values[0];
    int i = 1;
    for (i=1;i<size;i++){
        if (minimum>values[i]){
            minimum=values[i];
        }
    }
    pthread_exit(0);
}
void *maxim(void *arg){
    int *values = (int *) arg;
    maximum = values[0];
    int i = 1;
    for (i=1;i<size;i++){
        if (maximum<values[i]){
            maximum=values[i];
        }
    }
    pthread_exit(0);
}

