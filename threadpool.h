#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <thread> // std::thread
#include <vector> // std::vector
#include <queue> // std::queue
#include <mutex> // std::mutex
#include <condition_variable> // std::condition_variable

// This class manages a thread pool that will process tasks
class ThreadPool {
  public:
  ThreadPool();

  // The destructor joins all the threads so the program can exit gracefully.
  // This will be executed if there is any exception (e.g. creating the threads)
  ~ThreadPool();

  // This function will be called by the server, every time there is a request
  // that needs to be processed by the thread pool
  void enqueueTask(std::string request);

  private:
  // This condition variable is used for the threads to wait until there is work
  // to do
  std::condition_variable_any workQueueConditionVariable;

  // We store the threads in a vector, so we can later stop them gracefully
  std::vector<std::thread> threadpool;

  // Mutex to protect workQueue
  std::mutex workQueueMutex;

  // Queue of requests waiting to be processed
  std::queue<std::string> tasksQueue;

  // This will be set to true when the thread pool is shutting down. This tells
  // the threads to stop looping and finish
  bool done;

  // Function used by the threads to grab work from the queue
  void worker();

  void processRequest(std::string item) {
    // // Pretend we are doing a lot of work
    // std::this_thread::sleep_for(std::chrono::seconds(5));

    // // Send a message to the connection
    // std::string response = "Good talking to you\n";
    // send(item.first, response.c_str(), response.size(), 0);

    // // Close the connection
    // close(item.first);
  }
};
#endif // THREADPOOL_H
