#include "process.h"
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class Scheduler{
  private:
      int timeElapsed;
      std::vector<process> process_vector;
      std::vector<process> wait_queue;
  public:
    // 2 Constructor for Scheduler class
      Scheduler(){
        timeElapsed = 0;
      }
      Scheduler(std::vector<process> p){
        process_vector = p;
        wait_queue = p;
      }
      std::vector<process> *getWaitQueueAddress(){
        return &wait_queue;
      };
      std::vector<process> getWaitQueue(){
        return wait_queue;
      };
      //From text file to vector of processes
      void inputProcess(int arr_time, int burst_time, int prio_num, int idx){
        process p = process(arr_time, burst_time, prio_num, idx);
        process_vector.push_back(p);
        wait_queue.push_back(p);
      }
      //Used to find the least arrival time
      process findLeastArrival(){
         int newIndex = 0;
         //For loop left hand side of comparison
         for (int lhproc=0; lhproc<wait_queue.size(); lhproc++){
           //For loop right hand side of comparison
           newIndex = 0;
           for(int rhproc=0; rhproc<wait_queue.size(); rhproc++){
             if(wait_queue[lhproc].getArrival() > wait_queue[rhproc].getArrival()){
               newIndex++;
             }
           }
           if(newIndex==0){
             process temp = wait_queue[lhproc];
             wait_queue.erase(wait_queue.begin()+lhproc);
             return temp;
           }
         }
      }
      process findLeast(std::vector<process> *p, string criteria){
         int newIndex = 0;
         //For loop left hand side of comparison
         for (int lhproc=0; lhproc<p->size(); lhproc++){
           //For loop right hand side of comparison
           newIndex = 0;
           for(int rhproc=0; rhproc<p->size(); rhproc++){
             if(criteria == "BURST"){
               if((*p)[lhproc].getBurst() > (*p)[rhproc].getBurst()){
                 newIndex++;
               }
            }
            else if(criteria == "PRIORITY"){
              if((*p)[lhproc].getPrio() > (*p)[rhproc].getPrio()){
                newIndex++;
              }
            }
           }
           if(newIndex==0){
             process temp = (*p)[lhproc];
             p->erase(p->begin()+lhproc);
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
    // void fcfs(){
    //   std::vector<process> fcfs;
    //   for(int proc_count = 0; proc_count<process_vector.size(); proc_count++){
    //     fcfs.push_back(findLeastArrival());
    //   }
    //   wait_queue = fcfs;
    //   printProcessDetails(wait_queue);
    // }
  void sortBy(std::vector<process> *p, string criteria){
      std::vector<process> temp;
      int size = p->size();
      for(int proc_count = 0; proc_count<size; proc_count++){
        if(criteria == "BURST"){
          temp.push_back(findLeast(p, "BURST"));
        }
        else if(criteria == "PRIORITY"){
          temp.push_back(findLeast(p, "PRIORITY"));
        }
      }
      *p = temp;
  }


// Tick Using FCFS Schedule
    void runFCFS(){
      int timeArrived = 0;
      int cpuTime = 0;
      int counter = 0;
      int turn = 0;
      int wait_time = 0;
      int total_wait_time = 0;
      int total_response = 0;
      double avg_response = 0;
      double utilization = 0;
      int donothing = 0;

      std::vector<int>burst;
      int done_proc=0;
      //FCFS QUEUE : PROCESS THAT RUNS
      std::vector<process>queue;
      cout<< "Time Arrived\t"<< "  Process Index \t   " << "Processed Time \t "<<endl;
      while(done_proc!=process_vector.size()){
        // if(timeElapsed<100){cout<<timeElapsed<<endl;}
        //CHECKS IF ARRIVAL TIME IS SAME AS TIME ELAPSED
        for(int curr=0; curr<wait_queue.size(); curr++){
          if(timeElapsed == wait_queue[curr].getArrival()){
            wait_queue[curr].setArrived(timeElapsed);
            queue.push_back(wait_queue[curr]);
            burst.push_back(wait_queue[curr].getBurst());
          }
        }
        //CHECKS IF A PROCESS HAS ALREADY ARRIVED
        if(!queue.empty()){
          //TICK BURST TIME
          queue[0].tick();
          //CHECK IF PROCESS ALREADY OVER
          if(queue[0].isProcessOver()){
            timeArrived = queue[0].getArrived();
            turn = timeArrived + queue[0].getCpuTime();
            wait_time = turn - burst[counter];
            cout<< "     "<<timeArrived<< "  "<<"\t\t"<< queue[0].getIndex()<<"\t\t"<<
            queue[0].getCpuTime()<<"x\t \t" << endl;
            queue[0].resetCpuTime();
            queue.erase(queue.begin());
            queue[0].setArrived(timeElapsed+1);
            total_response += timeArrived;
            total_wait_time += wait_time;
            counter++;

            done_proc++;
          }
        }
        else donothing++;

        timeElapsed++;
        if (wait_queue.size() == done_proc) {
          avg_response = total_response/burst.size();
          utilization = ((((double)turn - (double)donothing)/(double)turn)*100);
          cout << endl << "Cumulative Waiting Time: " << total_wait_time << endl;
          cout << "Turnaround Time: " << turn << endl;
          cout << "Response Time: " << avg_response << endl;
          cout << "Utilization: " << utilization << "%" << endl;
      }
      }
    }
// Tick Using SJF Scheduler
    void runSJF(){
      int timeArrived;
      std::vector<process>queue;
      std::vector<process> curr_run;
      int done_proc=0;
      bool init = true;
      cout<< "Time Arrived\t"<< "  Process Index \t   " << "Processed Time \t "<<endl;
      while(done_proc != wait_queue.size()){
        for(int curr=0; curr<wait_queue.size(); curr++){
          if(timeElapsed == wait_queue[curr].getArrival()){
            wait_queue[curr].setArrived(timeElapsed);
            queue.push_back(wait_queue[curr]);
          }
        }
        if(!queue.empty()){
          sortBy(&queue, "BURST");
        }
        if(init){
          curr_run.push_back(queue[0]);
          queue.erase(queue.begin());
          init = false;
        }
        //Catch first iteration of  running
        if(!curr_run.empty()){
          //CHECK IF PROCESS ALREADY OVER
          curr_run[0].tick();
          if(curr_run[0].isProcessOver()){
            timeArrived = curr_run[0].getArrived();
            cout<< "     "<<timeArrived<< "  "<<"\t\t"<< curr_run[0].getIndex()<<"\t\t"<<
            curr_run[0].getCpuTime()<<"x\t \t"<<endl;
            curr_run[0].resetCpuTime();
            curr_run.erase(curr_run.begin());
            done_proc++;
          }
        }
        else{
          curr_run.push_back(queue[0]);
          curr_run[0].setArrived(timeElapsed);
          queue.erase(queue.begin());
          curr_run[0].tick();
        }
        // cout<<"QUEUE: " << timeElapsed<<endl;
        // printProcessDetails(queue);
        // cout<<endl;
        // printProcessDetails(curr_run);
        // cout<<endl;
        timeElapsed++;
      }
    }

    void runSRTF(){
      int timeArrived;
      int counter= 0;
      int turn;
      std::vector<process>queue;
      std::vector<process> curr_run;
      std::vector<int>burst;
      int done_proc=0;
      bool init = true;
      bool queueAdded = false;
      cout<< "Time Arrived\t"<< "  Process Index \t   " << "Processed Time \t "<<endl;
      while(done_proc!=process_vector.size()){
        //Find Process that Arrives already
        for(int curr=0; curr<wait_queue.size(); curr++){
          if(timeElapsed == wait_queue[curr].getArrival()){
            wait_queue[curr].setArrived(timeElapsed);
            wait_queue[curr].setPos(curr);
            queue.push_back(wait_queue[curr]);
            queueAdded = true;
            burst.push_back(wait_queue[curr].getBurst());

          }
        }
        //If there was a change in the queue vector
        if(!queue.empty() && queueAdded){
          sortBy(&queue,"BURST");
          //if first process to run
          if(init){
            curr_run.push_back(queue[0]);
            queue.erase(queue.begin());
            init = false;
          }
          else{
          //check if there is a smaller process time in queue than the current running
          if(pre_empt(&curr_run, &queue, "BURST")){
              timeArrived = curr_run[0].getArrived();
              cout<< "     "<<timeArrived<< "  "<<"\t\t"<< curr_run[0].getIndex()<<"\t\t"<<
              curr_run[0].getCpuTime()<<"\t \t"<<endl;
              //Reset the cpuTime count of the interrupted process
              curr_run[0].resetCpuTime();
              //Switch to the least burst time from queue
              swapProcess(&curr_run, &queue, 0, queue.size()-1);
              //set the arrival time for the newly running process
              curr_run[0].setArrived(timeElapsed);
            }
          }
          queueAdded = false;
        }
        //Catch first iteration of  running
        if(!curr_run.empty()){
          curr_run[0].tick();
          //CHECK IF PROCESS ALREADY OVER
          if(curr_run[0].isProcessOver()){
            timeArrived = curr_run[0].getArrived();
            turn = timeArrived + curr_run[0].getCpuTime();
            cout<< "     "<<timeArrived<< "  "<<"\t\t"<< curr_run[0].getIndex()<<"\t\t"<<
            curr_run[0].getCpuTime()<<"x\t \t"<< "TAT ->" << turn <<"\t \t" << "WT ->" << turn - curr_run[0].getCpuTime() << endl;
            //Remove from list of process to run
            wait_queue.erase(wait_queue.begin()+curr_run[0].getIndex());
            //curr_run[0].getCpuTime()<<"x\t \t" << endl;
            //Change the current running to next in queue
            //queue[0].resetCpuTime();
            curr_run.erase(curr_run.begin());
            curr_run.push_back(queue[0]);
            queue.erase(queue.begin());
            curr_run[0].setArrived(timeElapsed+1);
            counter++;
            done_proc++;
            sortBy(&queue, "BURST");
          }
        }
        else{
          curr_run.push_back(queue[0]);
          curr_run[0].setArrived(timeElapsed);
          queue.erase(queue.begin());
          curr_run[0].tick();
        }
        // cout<<"QUEUE: " << timeElapsed<<endl;
        // printProcessDetails(queue);
        // cout<<endl;
        // printProcessDetails(curr_run);
        // cout<<endl;
        timeElapsed++;
      }
    }

    void runPriority(){
      int timeArrived;
      std::vector<process>queue;
      std::vector<process> curr_run;
      bool init = true;
      bool queueAdded = false;
      int done_proc = 0;
      cout<< "Time Arrived\t"<< "  Process Index \t   " << "Processed Time \t "<<endl;
      while(done_proc != process_vector.size()){
        //Find Process that Arrives already
        for(int curr=0; curr<wait_queue.size(); curr++){
          if(timeElapsed == wait_queue[curr].getArrival()){
            wait_queue[curr].setArrived(timeElapsed);
            queue.push_back(wait_queue[curr]);
            queueAdded = true;
          }
        }
        //If there was a change in the queue vector
        if(!queue.empty() && queueAdded){
          sortBy(&queue,"PRIORITY");
          //if first process to run
          if(init){
            curr_run.push_back(queue[0]);
            queue.erase(queue.begin());
            init = false;
          }
          else{
          //check if there is a smaller indices in queue than the current running
          if(pre_empt(&curr_run, &queue, "PRIORITY")){
              timeArrived = curr_run[0].getArrived();
              cout<< "     "<<timeArrived<< "  "<<"\t\t"<< curr_run[0].getIndex()<<"\t\t"<<
              curr_run[0].getCpuTime()<<"\t \t"<<endl;
              //Reset the cpuTime count of the interrupted process
              curr_run[0].resetCpuTime();
              //Switch to the least burst time from queue
              swapProcess(&curr_run, &queue, 0, queue.size()-1);
              //set the arrival time for the newly running process
              curr_run[0].setArrived(timeElapsed);
            }
          }
          queueAdded = false;
        }

        // Catch first iteration of  running
        if(!curr_run.empty()){
          curr_run[0].tick();
          //CHECK IF PROCESS ALREADY OVER
          if(curr_run[0].isProcessOver()){
            timeArrived = curr_run[0].getArrived();
            // cout<<"PROCESS ENDED : "<<endl;
            cout<< "     "<<timeArrived<< "  "<<"\t\t"<< curr_run[0].getIndex()<<"\t\t"<<
            curr_run[0].getCpuTime()<<"x\t \t"<<endl;
            //Remove from list of process to run
            wait_queue.erase(wait_queue.begin()+curr_run[0].getIndex());
            //Change the current running to next in queue
            curr_run.erase(curr_run.begin());
            sortBy(&queue, "PRIORITY");
            done_proc++;
          }
        }
        else{
          curr_run.push_back(queue[0]);
          queue.erase(queue.begin());
          curr_run[0].setArrived(timeElapsed);
          curr_run[0].tick();
        }
        timeElapsed++;
      }
    }

    void swapProcess(std::vector<process> *q, std::vector<process> *r, int qindex, int rindex){
      process tempr = (*q)[qindex];
      (*q)[qindex] = (*r)[rindex];
      (*r)[rindex] = tempr;
    }
    bool pre_empt(std::vector<process> *lhs, std::vector<process> *rhs, string criteria){
      if(criteria == "BURST"){
        process holder = findLeast(rhs, "BURST");
        if((*lhs)[0].getBurst() > holder.getBurst()){
          rhs->push_back(holder);
          return true;
        }
        rhs->push_back(holder);
        return false;
      }
      else if(criteria == "PRIORITY"){
        process holder = findLeast(rhs, "PRIORITY");
        if((*lhs)[0].getPrio() > holder.getPrio()){
          rhs->push_back(holder);
          return true;
        }
        rhs->push_back(holder);
        return false;
      }
    }

//DESC ORDER FUNC FOR SORT ALGO
    bool descOrder (int i, int j) { return (i>j); }
//MOVE VECTOR OBJECT TO THE BACK
    template <typename T>
    void moveItemToBack(std::vector<T>& v, size_t itemIndex)
    {
       std::swap(v[itemIndex], v.back()); // or swap with *(v.end()-1)
    }

    void runRobin(int quantum){
      int timeArrived = 0;
      int cpuTime = 0;
      int qcount = quantum;
      int counter = 0;
      int turn = 0;
      int wait_time = 0;
      int total_wait_time = 0;
      int total_response = 0;
      double avg_response = 0;
      double utilization = 0;
      int donothing = 0;

      int done_proc = 0;
      //FCFS QUEUE : PROCESS THAT RUNS
      std::vector<process>queue;
      std::vector<int>burst;

      cout<< "Time Arrived\t"<< "  Process Index \t   " << "Processed Time \t "<<endl;

      while(done_proc!=process_vector.size()){
        // if(timeElapsed<100){cout<<timeElapsed<<endl;}
        //CHECKS IF ARRIVAL TIME IS SAME AS TIME ELAPSED
        for(int curr=0; curr<wait_queue.size(); curr++){
          if(timeElapsed == wait_queue[curr].getArrival()){
            wait_queue[curr].setArrived(timeElapsed);
            queue.push_back(wait_queue[curr]);
            burst.push_back(wait_queue[curr].getBurst());
          }
        }
        //CHECKS IF A PROCESS HAS ALREADY ARRIVED
        if(!queue.empty()){
          //TICK BURST TIME
          //cout << qcount << "          " << queue[0].getBurst() << endl;
          //printProcessDetails(queue);

          if(!queue[0].isProcessOver() && qcount == 0){
            timeArrived = queue[0].getArrived();
            cout<< "     "<<timeArrived<< "  "<<"\t\t"<< queue[0].getIndex()<<"\t\t"<<
            queue[0].getCpuTime()<<"\t \t"<< endl;
            queue[0].resetCpuTime();
            moveItemToBack(queue,0);
            //queue.push_back(queue[0]);
            //queue.erase(queue.begin());
            queue[0].setArrived(timeElapsed);
            qcount = quantum;
          }

          //CHECK IF PROCESS ALREADY OVER
          if(queue[0].isProcessOver()){
            timeArrived = queue[0].getArrived();
            turn = timeArrived + queue[0].getCpuTime();
            wait_time = turn - burst[counter];
            cout<< "     "<<timeArrived<< "  "<<"\t\t"<< queue[0].getIndex()<<"\t\t"<<
            queue[0].getCpuTime()<<"x\t \t" << endl;
            queue[0].resetCpuTime();
            queue.erase(queue.begin());
            queue[0].setArrived(timeElapsed);
            qcount = quantum;
            total_response += timeArrived;
            total_wait_time += wait_time;
            counter++;
            done_proc++;
          }

        }
        else donothing++;

          queue[0].tick();
          timeElapsed++;
          qcount--;

          if (wait_queue.size() == done_proc) {
          avg_response = total_response/burst.size();
          utilization = ((((double)turn - (double)donothing)/(double)turn)*100);
          cout << endl << "Cumulative Waiting Time: " << total_wait_time << endl;
          cout << "Turnaround Time: " << turn << endl;
          cout << "Response Time: " << avg_response << endl;
          cout << "Utilization: " << utilization << "%" << endl;
      }

      }
    }
};
