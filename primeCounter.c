#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdatomic.h>
#include <math.h>
#include <stdint.h>

#define MAX_ARRAY_SIZE 500000

int array[MAX_ARRAY_SIZE];
atomic_int shared_counter;
int total_primes = 0;
pthread_mutex_t total_primes_lock;

// Function to check if a number is prime
bool isPrime(int num)
{
    if (num <= 1) return false;
    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;
    int limit = sqrt(num);
    for (int i = 5; i <= limit; i += 6)
    {
        if (num % i == 0 || num % (i + 2) == 0) return false;
    }
    return true;
}

// Worker function to find prime numbers in the array
void *worker(void *arg)
{
    while (true)
    {
        int start = atomic_fetch_sub(&shared_counter, 1000);
        if (start <= 0) break;

        int end = (start - 1000 < 0) ? 0 : start - 1000;
        int local_counter = 0;

        for (int index = start - 1; index >= end; index--)
        {
            if (isPrime(array[index]))
            {
                local_counter++;
            }
        }

        pthread_mutex_lock(&total_primes_lock);
        total_primes += local_counter;
        pthread_mutex_unlock(&total_primes_lock);
    }
    return NULL;
}

int main()
{
    int num;
    pthread_t threads[4];

    // Initialize mutex
    pthread_mutex_init(&total_primes_lock, NULL);

    // Read numbers from stdin and store them in the array until end of file or array full
    int array_size = 0;
    while (scanf("%d", &num) != EOF && array_size < MAX_ARRAY_SIZE)
    {
        array[array_size++] = num;
    }
    atomic_store(&shared_counter, array_size);

    // Create 4 worker threads
    for (int i = 0; i < 4; i++)
    {
        pthread_create(&threads[i], NULL, worker, NULL);
    }

    // Wait for all threads to complete
    for (int i = 0; i < 4; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("%d total primes.\n", total_primes);

    // Destroy mutex
    pthread_mutex_destroy(&total_primes_lock);

    return 0;
}
