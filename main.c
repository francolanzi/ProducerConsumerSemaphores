#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 100
#define MAX 1000

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

int buffer[N];
int size = 0;

int produce() {
  sleep(1);
  return rand() % 10;
}

void consume(int elem) {
  sleep(1);
  printf("%d ", elem);
  fflush(stdout);
}

void insert_elem(int elem) {
  buffer[size++] = elem;
}

int remove_elem() {
  int elem = buffer[0];
  size--;
  for(int i = 0; i < size; i++) {
    buffer[i] = buffer[i + 1];
  }
  return elem;
}

void *producer() {
  for (int i = 0; i < MAX; i++) {
    int elem = produce();
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    insert_elem(elem);
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
  }
  pthread_exit(NULL);
}

void *consumer() {
  for (int i = 0; i < MAX; i++) {
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    int elem = remove_elem();
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    consume(elem);
  }
  pthread_exit(NULL);
}

int main() {
  pthread_t pro, con;
  srand(time(NULL));
  pthread_mutex_init(&mutex, NULL);
  sem_init(&empty, 0, N);
  sem_init(&full, 0, 0);
  pthread_create(&pro, NULL, &producer, NULL);
  pthread_create(&con, NULL, &consumer, NULL);
  pthread_join(pro, NULL);
  pthread_join(con, NULL);
  sem_destroy(&full);
  sem_destroy(&empty);
  pthread_mutex_destroy(&mutex);
}