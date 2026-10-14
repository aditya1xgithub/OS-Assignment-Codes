#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#define N 5
sem_t chopstick[N];//Semaphore for each chopstick
sem_t mutex;//Controls chopstick acquistion

void* philospher (void* arg){
    int i =*(int*) arg; //philospher id 
    printf("Philospher  %d is thinking \n",i);
    sem_wait(&mutex);
    //Odd philospher take i first , then i+1
    if(i%2==0){
        sem_wait(&chopstick[i]);
        sem_wait(&chopstick[(i+1)%N]);
    }
    else{
        sem_wait(&chopstick[(i+1)%N]);
        sem_wait(&chopstick[i]);
    }
    sem_post(&mutex);
    printf("Philospher %d is eating \n",i);

    if(i%2==0){
        sem_post(&chopstick[(i+1)%N]);
        sem_post(&chopstick[i]);
    }
    else{
        sem_post(&chopstick[i]);
        sem_post(&chopstick[(i+1)%N]);
    }
    printf("Philospher %d finished eating \n",i);
    return NULL;
}
int main(){
    pthread_t philospher_thread[N];//Thread for each philospher 
    int id[N];//Philospher ID
    sem_init(&mutex,0,1);//Initialize mutex to 1
    for(int i=0;i<N;i++){
        sem_init(&chopstick[i],0,1);//Each chopstick is initially available 
        id[i]=i;
    }
    //Create all philospher threads 
    for(int i = 0 ; i< N ; i++){
        pthread_create(&philospher_thread[i],NULL,philospher,&id[i]);
    } 
    //Wait for all philospher to finish
    for(int i =0 ; i<N;i++){
        pthread_join(philospher_thread[i],NULL);
    }
    //Destroy all chopsticks 
    for(int i = 0 ; i<N;i++){
        sem_destroy(&chopstick[i]);
    }
    sem_destroy(&mutex);//Destroy mutex 
    return 0;

}