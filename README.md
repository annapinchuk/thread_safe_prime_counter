# Thread Safe Prime Counter

### Background for this task </br>
You have an endless data stream that should be processed fast. For example, security camera data is where an 
intrusion should be detected.</br>
In this assignment, we will use a random number generator that simulates the endless stream, the task is to count the number of prime numbers in this stream.</br>

The task in detail:</br>
We have a random number generator called randomGenerator.c and a basic implementation of the prime counter app, called primeCounter.c
The usage of the generator is as follows:</br>
``` ./randomGenerator <seed> <num_of_numbers> ``` </br>
example: </br>
``` ./randomGenerator 10 10 ``` will give you 10 numbers, generated with random seed 10. </br>
Usage of the counter: </br>
``` ./randomGenerator 10 100 | ./primesCounter ``` the expected output is 5 total primes. </br>

## This repository involves
a) The best solution code. </br>
and in the README: </br>
b) Screenshot with 10M numbers checked by the provided primeCounter.c + time </br>
c) Screenshot with 10M numbers checked by our solution + time </br>
d) proof of RAM usage below 2MB </br>

## How to run
1. ``` make all ```
2. ``` ./randomGenerator <seed> <num_of_numbers> | ./primeCounter ```

For example:
1. ``` make all ```
2. ``` ./randomGenerator 10 10000 | ./primeCounter ```

To see the time:
1. ``` make all ```
2. ``` time ./randomGenerator 10 10000 | ./primeCounter ```

To see the usage of RAM:
1. ``` make all ```
2. ``` valgrind --tool=massif --massif-out-file=massif.out ./randomGenerator 10 10000 | ./primeCounter  ```
3. ``` ms_print massif.out ```
 

## Time improvement:
Base code:
![image](https://github.com/annapinchuk/thread_safe_prime_counter/assets/78349342/400ad7d4-295d-4b78-b362-8dd170faf42e)

Better isPrime function:
![image](https://github.com/annapinchuk/thread_safe_prime_counter/assets/78349342/90164e16-bca2-4856-999b-78e6b5bdcb04)

Usage of better isPrime function and threads:
![image](https://github.com/annapinchuk/thread_safe_prime_counter/assets/78349342/7469aae7-d38b-42b8-96ae-4923b8b236a1)

## RAM space:
As we can see in the images the peak memory usage is 4,104 bytes.
![image](https://github.com/annapinchuk/thread_safe_prime_counter/assets/78349342/c978b7e1-297b-4bf3-8546-e803d4a3b6b1)
![image](https://github.com/annapinchuk/thread_safe_prime_counter/assets/78349342/1347f12c-5579-420b-bd44-be3c875e3a2e)


