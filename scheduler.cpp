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
    // 2 Constructor for Scheduler class
      Scheduler(){
        timeElapsed = 0;
      }
      Scheduler(std::vector<process> p){
        process_vector = p;
        run_queue = p;
      }
      //From text file to vector of processes
      void inputProcess(int arr_time, int burst_time, int prio_num, int idx){
        process p = process(arr_time, burst_time, prio_num, idx);
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
// For testing purposes : Print Process details
    void printProcessDetails(std::vector<process> processes){
      cout<< "ProcessIndex\t"<< "  Arrival Time \t   " << "Burst Time \t " << "Priority Index"<<endl;
      for(int i = 0; i<processes.size(); i++){
        cout<< "     "<<i << "  "<<"\t\t"<< processes[i].getArrival()<<"\t\t"<<
        processes[i].getBurst()<<"\t \t"
        <<processes[i].getPrio()<<endl;

      }
    }
  // Creation of vector with scheduled based on fcfs
    void fcfs(){
      std::vector<process> fcfs;
      for(int proc_count = 0; proc_count<process_vector.size(); proc_count++){
        fcfs.push_back(findLeastArrival());
      }
      run_queue = fcfs;
      printProcessDetails(run_queue);
    }
// Tick Using FCFS Schedule
    void runFcfs(){
      int timeArrived;
      int cpuTime;
      std::vector<process>queue;
      cout<< "Time Arrived\t"<< "  Process Index \t   " << "Processed Time \t "<<endl;
      while(!run_queue.empty()){
        // if(timeElapsed<100){cout<<timeElapsed<<endl;}
        for(int curr=0; curr<run_queue.size(); curr++){
          if(timeElapsed == run_queue[curr].getArrival()){
            run_queue[curr].setArrived(timeElapsed);
            queue.push_back(run_queue[curr]);
          }
        }
        if(!queue.empty()){
          queue[0].setBurst(queue[0].getBurst()-1);
          // cout<<"CURRENT RUNNING PROCESS BURST TIME: "<<queue[0].getBurst()<< "  "<<timeElapsed<< endl;
          if(queue[0].getBurst()==0){
            timeArrived = queue[0].getArrived();
            cpuTime = (timeElapsed+1)-timeArrived;
            cout<< "     "<<timeArrived<< "  "<<"\t\t"<< queue[0].getIndex()<<"\t\t"<<
            cpuTime<<"x\t \t"<<endl;
            queue.erase(queue.begin());
            run_queue.erase(run_queue.begin()+queue[0].getIndex());
          }
        }
        timeElapsed++;
      }
    }
// Tick Using SJF Scheduler
    void runSJF(){
      int timeArrived;
      int cpuTime;
      std::vector<process>queue;
      cout<< "Time Arrived\t"<< "  Process Index \t   " << "Processed Time \t "<<endl;
      while(!run_queue.empty()){
        // if(timeElapsed<100){cout<<timeElapsed<<endl;}
        for(int curr=0; curr<run_queue.size(); curr++){
          if(timeElapsed == run_queue[curr].getArrival()){
            run_queue[curr].setArrived(timeElapsed);
            queue.push_back(run_queue[curr]);
          }
        }
        if(!queue.empty()){
          queue[0].setBurst(queue[0].getBurst()-1);
          // cout<<"CURRENT RUNNING PROCESS BURST TIME: "<<queue[0].getBurst()<< "  "<<timeElapsed<< endl;
          if(queue[0].getBurst()==0){
            timeArrived = queue[0].getArrived();
            cpuTime = (timeElapsed+1)-timeArrived;
            cout<< "     "<<timeArrived<< "  "<<"\t\t"<< queue[0].getIndex()<<"\t\t"<<
            cpuTime<<"x\t \t"<<endl;
            queue.erase(queue.begin());
            run_queue.erase(run_queue.begin()+queue[0].getIndex());
          }
        }
        timeElapsed++;
      }
    }

    void searchJob(std::vector<process> queue){
      for(int i = 0; i<queue.size(); i++){  
      }
    }
};
