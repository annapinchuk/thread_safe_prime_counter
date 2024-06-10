#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <cmath>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

const size_t BATCH_SIZE = 5000; // Size of each batch to process

std::atomic<int> total_primes(0);
std::mutex mtx;
std::condition_variable cv;
std::queue<std::vector<int>> task_queue;
bool done = false;

bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    if (n < 9) return true; // 2, 3, 5, 7 are prime
    if (n % 5 == 0) return false;

    int sqrtN = static_cast<int>(std::sqrt(n));
    for (int i = 5; i <= sqrtN; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

void worker() {
    while (true) {
        std::vector<int> numbers;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, []{ return !task_queue.empty() || done; });
            if (task_queue.empty() && done) return;
            numbers = std::move(task_queue.front());
            task_queue.pop();
        }

        int local_prime_count = 0;
        for (int num : numbers) {
            if (isPrime(num)) {
                local_prime_count++;
            }
        }
        total_primes.fetch_add(local_prime_count, std::memory_order_relaxed);
    }
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();

    size_t num_threads = std::thread::hardware_concurrency(); // Dynamically adjust number of threads
    std::vector<std::thread> workers;
    for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back(worker);
    }

    std::vector<int> numbers;
    numbers.reserve(BATCH_SIZE);
    int num;

    while (std::cin >> num) {
        numbers.push_back(num);
        if (numbers.size() >= BATCH_SIZE) {
            {
                std::lock_guard<std::mutex> lock(mtx);
                task_queue.push(std::move(numbers));
            }
            cv.notify_one();
            numbers.clear();
            numbers.reserve(BATCH_SIZE);
        }
    }

    if (!numbers.empty()) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            task_queue.push(std::move(numbers));
        }
        cv.notify_one();
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        done = true;
    }
    cv.notify_all();

    for (auto& worker : workers) {
        worker.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_spent = end_time - start_time;

    std::cout << total_primes << " total primes.\n";
    std::cout << "Time taken: " << time_spent.count() << " seconds.\n";

    return 0;
}
