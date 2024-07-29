#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// Mutex and condition variable for thread synchronization
std::mutex mutx;
std::condition_variable threadVar;
bool ready = false; // Signal for the second thread

// Function for the first thread to count up to 20
void countUp() {
    for (int i = 0; i <= 20; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Including a delay
        std::cout << "Count Up: " << i << std::endl; // Printing the current count
    }
    // Starting the count down of the second thread
    std::unique_lock<std::mutex> lck(mutx); // Getting the mutex
    ready = true; // Switching to true when the first thread is done
    threadVar.notify_one(); // Signaling countDown thread to proceed
}

// Function for the second thread to count down from 20 to 0
void countDown() {
    std::unique_lock<std::mutex> lck(mutx); // Getting the mutex

    // Waiting until the first thread has finished counting up
    threadVar.wait(lck, []{ return ready; }); // Blocking until 'ready' is true
    for (int i = 20; i >= 0; --i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
        std::cout << "Count Down: " << i << std::endl; // Printing the current count
    }
}

int main() {
    // Creating two threads: one for counting up and one for counting down
    std::thread t1(countUp);
    std::thread t2(countDown);

    // Waiting for both threads to finish before exiting the program
    t1.join(); 
    t2.join(); 

    return 0;
}
