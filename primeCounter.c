#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdatomic.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>

// Shared data
atomic_int total_counter = ATOMIC_VAR_INIT(0);
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

// // Function to check if a number is prime
// bool isPrime(int n) {
//     if (n <= 1) {
//         return false;
//     }
//     for (int i = 2; i * i <= n; i++) {
//         if (n % i == 0) {
//             return false;
//         }
//     }
//     return true;
// }

// Function to check if a number is prime
bool isPrime(int num)
{
    if (num <= 1)
        return false;
    if (num <= 3)
        return true;
    if (num % 2 == 0 || num % 3 == 0)
        return false;
    int limit = sqrt(num);
    for (int i = 5; i <= limit; i += 6)
    {
        if (num % i == 0 || num % (i + 2) == 0)
            return false;
    }
    return true;
}

// int main() {
//     int num;
//     int total_counter = 0;

//     // Read numbers from stdin until end of file
//     while (scanf("%d", &num) != EOF) {   
//         if (isPrime(num)) {
//             total_counter++;
//         }
//     }

//     printf("%d total primes.\n", total_counter);

//     return 0;
// }

// Function for each thread to process numbers
void *thread_function(void *arg)
{
    int thread_id = *(int *)arg;
    int local_counter = 0;
    int num;

    // Read numbers from stdin in chunks
    while (scanf("%d", &num) != EOF)
    {
        // Check if the number is prime
        if (isPrime(num))
        {
            local_counter++;
        }
    }

    // Atomically update the global counter
    pthread_mutex_lock(&counter_mutex);
    total_counter += local_counter;
    pthread_mutex_unlock(&counter_mutex);

    return NULL;
}

int main()
{
    int num_threads;
    // Determine number of threads to use
    num_threads = sysconf(_SC_NPROCESSORS_ONLN);

    pthread_t threads[num_threads];
    int thread_ids[num_threads];
    int i;

    // Create threads
    for (i = 0; i < num_threads; i++)
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    // Join threads
    for (i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Print the total number of prime numbers found
    printf("%d total primes.\n", total_counter);

    return 0;
}
