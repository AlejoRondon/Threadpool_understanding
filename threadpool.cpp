#include "threadpool.h"

ThreadPool::ThreadPool() : done(false) {

  // This returns the number of threads supported by the system. If the
  // function can't figure out this information, it returns 0. 0 is not good,
  // so we create at least 1
  auto numberOfThreads = std::thread::hardware_concurrency();
  if (numberOfThreads == 0) {
    numberOfThreads = 1;
  }
  std::cout << "Creating Threadpool of " << numberOfThreads << " threads" << std::endl;

  for (unsigned i = 0; i < numberOfThreads; ++i) {
    // The threads will execute the private member `doWork`. Note that we need
    // to pass a reference to the function (namespaced with the class name) as
    // the first argument, and the current object as second argument
    threadpool.push_back(std::thread(&ThreadPool::worker, this));
    std::cout << "Thread[" << i << "] was created and added to the threads pool(vector)" << std::endl;
  }
}

// This function will be called by the server, every time there is a request
// that needs to be processed by the thread pool
void ThreadPool::enqueueTask(std::string task_payload) {
  // Grab the mutex
  std::lock_guard<std::mutex> g(workQueueMutex);

  // Push the request to the queue
  tasksQueue.push(task_payload);
  std::cout <<"*"<< task_payload << " was enqueued in the thread pool" << std::endl;


  // Notify one thread that there are requests to process
  workQueueConditionVariable.notify_one();
}

// Function used by the threads to grab work from the queue
int numberOfWorkers = 0;
void ThreadPool::worker() {
  int worker_id = numberOfWorkers++;
  // Loop while the queue is not destructing
  std::cout<<"Worker[" << worker_id << "] say: I'm ready to work, I'll be waiting any task" << std::endl;

  while (!done) {
    std::string request;

    // Create a scope, so we don't lock the queue for longer than necessary
    {
      std::unique_lock<std::mutex> g(workQueueMutex);

      workQueueConditionVariable.wait(g, [&] {
        // Only wake up if there are elements in the queue or the program is
        // shutting down
        return !tasksQueue.empty() || done;
        });

      // If we are shutting down exit witout trying to process more work
      if (done) {
        break;
      }

      request = tasksQueue.front();
      tasksQueue.pop();
    }

    std::cout << "->Worker[" << worker_id << "] is solving: " << request << std::endl;
    //Simulating task latency
    std::this_thread::sleep_for(std::chrono::seconds(1 + ( std::rand() % ( 3 - 1 + 1 ))));
    std::cout << "<-Worker[" << worker_id << "] solved: " << request << std::endl;

    //    processRequest(request);
  }
}

// The destructor joins all the threads so the program can exit gracefully.
// This will be executed if there is any exception (e.g. creating the threads)
ThreadPool::~ThreadPool() {
    // So threads know it's time to shut down
    done = true;

    // Wake up all the threads, so they can finish and be joined
    workQueueConditionVariable.notify_all();
    for (auto& thread : threadpool) {
      if (thread.joinable()) {
        thread.join();
      }
    }
  }

