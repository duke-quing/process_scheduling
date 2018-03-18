#include "process.h"
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class Scheduler{
  private:
      int timeElapsed;
      std::vector<process> process_vector;
      std::vector<process> run_queue;
  public:
      Scheduler(){}
      Scheduler(std::vector<process> p){
        process_vector = p;
        run_queue = p;
      }
      //From text file to vector of processes
      void inputProcess(int arr_time, int burst_time, int prio_num){
        process p = process(arr_time, burst_time, prio_num);
        process_vector.push_back(p);
        run_queue.push_back(p);
      }
      //Used to find the least arrival time
      process findLeastArrival(){
         int newIndex = 0;
         //For loop left hand side of comparison
         for (int lhproc=0; lhproc<run_queue.size(); lhproc++){
           //For loop right hand side of comparison
           newIndex = 0;
           for(int rhproc=0; rhproc<run_queue.size(); rhproc++){
             if(run_queue[lhproc].getArrival() > run_queue[rhproc].getArrival()){
               newIndex++;
             }
           }
           if(newIndex==0){
             process temp = run_queue[lhproc];
             run_queue.erase(run_queue.begin()+lhproc);
             return temp;
           }
         }
      }

    void printProcessDetails(std::vector<process> processes){
      cout<< "ProcessIndex\t"<< "  Arrival Time \t   " << "Burst Time \t " << "Priority Index"<<endl;
      for(int i = 0; i<processes.size(); i++){
        cout<< "     "<<i << "  "<<"\t\t"<< processes[i].getArrival()<<"\t\t"<<
        processes[i].getBurst()<<"\t \t"
        <<processes[i].getPrio()<<endl;

      }
    }
    std::vector<process> fcfs(){
      std::vector<process> fcfs;
      for(int proc_count = 0; proc_count<process_vector.size(); proc_count++){
        fcfs.push_back(findLeastArrival());
      }
      printProcessDetails(fcfs);
      return fcfs;
    }
    int getIndex(process p){
      for(int i =0; i<process_vector.size(); i++){
        if(p == process_vector[i]){
          return i;
        }
      }
    }

    void runFcfs(){
      int timeArrived;
      std::vector<process> queue;
      cout<< "ProcessIndex\t"<< "  Arrival Time \t   " << "Burst Time \t " << "Priority Index"<<endl;
      while(!fcfs().empty()){
        timeElapsed++;
        for(int curr=0; curr<run_queue.size(); curr++){
          if(timeElapsed == fcfs()[curr].getArrival()){
            queue.push_back(fcfs()[curr]);
            timeArrived = timeElapsed;
          }
          if(!queue.empty()){
            queue[0].getBurst()-=1;
            if(queue[0].getBurst()==0){
              cout<< "     "<<timeElapsed<< "  "<<"\t\t"<< getIndex(queue[0])<<"\t\t"<<
              timeElapsed<<"x\t \t"<<endl;
              queue.erase(queue.begin());
              fcfs().erase(fcfs().begin());
            }
          }
        }
      }
    }
};
