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
    for(int process = 0; process< process_count; process++){
      int arr_time, burst_time, prio_num;
      cin >> arr_time >> burst_time >> prio_num;
      proc_scheduler.inputProcess(arr_time, burst_time, prio_num);
    }
    proc_scheduler.printProcessDetails();
  }

}
