//[1] https://ncona.com/2019/05/using-thread-pools-in-cpp/

#include "threadpool.h"
#include<windows.h>
#include <iostream>

#include <chrono>
#include <thread>

#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>

int t = 0;
int main() {
  using namespace std::this_thread; // sleep_for, sleep_until
  using namespace std::chrono; // nanoseconds, system_clock, seconds

  ThreadPool* threadpool = new ThreadPool();
  usleep(3000000);

//    int count = 1;
    for (;;)
    {
     usleep(500000);
      // print

      std::string task_str = "task" + std::to_string(t++);
      threadpool->enqueueTask(task_str);

    }
    return 0;

  delete threadpool;
  return(0);
}
