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
    proc_scheduler.runSRTF();
    // proc_scheduler.printProcessDetails(proc_scheduler.getWaitQueue());
    // proc_scheduler.sortByBurst(proc_scheduler.getWaitQueueAddress());
    // proc_scheduler.printProcessDetails(proc_scheduler.getWaitQueue());

  }
}
