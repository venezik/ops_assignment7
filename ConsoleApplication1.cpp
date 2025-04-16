#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>

std::mutex fork0;
std::mutex fork1;

std::random_device rd; // Random number generator
std::mt19937 gen(rd()); // Seed for random number generator
std::uniform_int_distribution<> think_time(1000, 3000); // Thinking time between 1 and 3 seconds
std::uniform_int_distribution<> eat_time(1000, 2000);

void philosopher(int id) {
	std::mutex* left_fork = (id == 0) ? &fork0 : &fork1; // Left fork for philosopher 0 is fork0, for philosopher 1 is fork1
	std::mutex* right_fork = (id == 0) ? &fork1 : &fork0; // Right fork for philosopher 0 is fork1, for philosopher 1 is fork0

    while (true) {
        // Thinking
        std::cout << "Philosopher " << id << " is thinking.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(think_time(gen)));

        // Always lock lower-addressed fork first to avoid deadlock
        std::mutex* first = std::min(left_fork, right_fork);
        std::mutex* second = std::max(left_fork, right_fork);

        first->lock();
        std::cout << "Philosopher " << id << " picked up first fork.\n";
        second->lock();
        std::cout << "Philosopher " << id << " picked up second fork.\n";

        // Eating
        std::cout << "Philosopher " << id << " is eating.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(eat_time(gen)));

        // Release forks
        second->unlock();
        first->unlock();
        std::cout << "Philosopher " << id << " put down both forks.\n";
    }
}

int main() {
    std::thread phil0(philosopher, 0);
    std::thread phil1(philosopher, 1);

    phil0.join();
    phil1.join();

    return 0;
}
