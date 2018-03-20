#include <iostream>
#include <cstdlib>
#include "scheduler.cpp"

using namespace std;

int main(){
  int testCases, process_count;
  cin >> testCases;
  for(int cases = 0; cases<testCases; cases++){
    cin>> process_count;
    Scheduler proc_scheduler;
    for(int process_idx = 0; process_idx< process_count; process_idx++){
      int arr_time, burst_time, prio_num;
      cin >> arr_time >> burst_time >> prio_num;
      proc_scheduler.inputProcess(arr_time, burst_time, prio_num, process_idx);
    }
<<<<<<< HEAD
    proc_scheduler.runPriority();
=======
    proc_scheduler.runRobin(25);
    //proc_scheduler.runRobin(15);
>>>>>>> fefc1b08001f06de62fb2e469903be2b66776ff0
    // proc_scheduler.printProcessDetails(proc_scheduler.getWaitQueue());
    // proc_scheduler.sortBy(proc_scheduler.getWaitQueueAddress(), "PRIORITY");
    // proc_scheduler.printProcessDetails(proc_scheduler.getWaitQueue());

  }
}
