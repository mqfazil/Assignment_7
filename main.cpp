#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t forks[2];

void* philosopher(void* num) {
    int id = *(int*)num;
    int first = (id == 0) ? 0 : 0; // Phil 0 picks 0 then 1
    int second = (id == 0) ? 1 : 1; // Phil 1 picks 0 then 1 (Order is key!)
    // Phil 1 picks up fork 0 before fork 1 (prevents deadlock)
    if (id == 1) {
        first = 0;
        second = 1;
    } else {
        first = 0;
        second = 1;
    }
    for (int i = 0; i < 3; i++) {
        printf("Philosopher %d is thinking...\n", id);
        sleep(1);
        pthread_mutex_lock(&forks[first]);
        printf("Philosopher %d picked up fork %d\n", id, first);
        pthread_mutex_lock(&forks[second]);
        printf("Philosopher %d picked up fork %d and is eating\n", id, second);
        sleep(1); // Eat
        pthread_mutex_unlock(&forks[second]);
        pthread_mutex_unlock(&forks[first]);
        printf("Philosopher %d finished eating and dropped forks\n", id);
    }
    return NULL;
}

int main() {
    pthread_t phils[2];
    int ids[2] = {0, 1};
    for (int i = 0; i < 2; i++) pthread_mutex_init(&forks[i], NULL);
    for (int i = 0; i < 2; i++) pthread_create(&phils[i], NULL, philosopher, &ids[i]);
    for (int i = 0; i < 2; i++) pthread_join(phils[i], NULL);
    for (int i = 0; i < 2; i++) pthread_mutex_destroy(&forks[i]);
    return 0;
}