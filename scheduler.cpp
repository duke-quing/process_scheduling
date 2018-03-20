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
      //FCFS QUEUE : PROCESS THAT RUNS
      std::vector<process>queue;
      cout<< "Time Arrived\t"<< "  Process Index \t   " << "Processed Time \t "<<endl;
      while(!wait_queue.empty()){
        // if(timeElapsed<100){cout<<timeElapsed<<endl;}
        //CHECKS IF ARRIVAL TIME IS SAME AS TIME ELAPSED
        for(int curr=0; curr<wait_queue.size(); curr++){
          if(timeElapsed == wait_queue[curr].getArrival()){
            wait_queue[curr].setArrived(timeElapsed);
            queue.push_back(wait_queue[curr]);
          }
        }
        //CHECKS IF A PROCESS HAS ALREADY ARRIVED
        if(!queue.empty()){
          //TICK BURST TIME
          queue[0].tick();
          //CHECK IF PROCESS ALREADY OVER
          if(queue[0].isProcessOver()){
            timeArrived = queue[0].getArrived();
            cout<< "     "<<timeArrived<< "  "<<"\t\t"<< queue[0].getIndex()<<"\t\t"<<
            queue[0].getCpuTime()<<"x\t \t"<<endl;
            queue[0].resetCpuTime();
            queue.erase(queue.begin());
            wait_queue.erase(wait_queue.begin()+queue[0].getIndex());
            queue[0].setArrived(timeElapsed+1);
          }
        }
        timeElapsed++;
      }
    }
// Tick Using SJF Scheduler
    void runSJF(){
      int timeArrived;
      std::vector<process>queue;
      std::vector<process> curr_run;
      bool init = true;
      cout<< "Time Arrived\t"<< "  Process Index \t   " << "Processed Time \t "<<endl;
      while(!wait_queue.empty()){
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
          curr_run[0].tick();
          //CHECK IF PROCESS ALREADY OVER
          if(curr_run[0].isProcessOver()){
            timeArrived = curr_run[0].getArrived();
            cout<< "     "<<timeArrived<< "  "<<"\t\t"<< curr_run[0].getIndex()<<"\t\t"<<
            curr_run[0].getCpuTime()<<"x\t \t"<<endl;
            curr_run[0].resetCpuTime();
            wait_queue.erase(wait_queue.begin()+curr_run[0].getIndex());
            curr_run.erase(curr_run.begin());
            curr_run.push_back(queue[0]);
            curr_run[0].setArrived(timeElapsed+1);
            queue.erase(queue.begin());
          }
        }
        timeElapsed++;
      }
    }

    void runSRTF(){
      int timeArrived;
      std::vector<process>queue;
      std::vector<process> curr_run;
      bool init = true;
      bool queueAdded = false;
      cout<< "Time Arrived\t"<< "  Process Index \t   " << "Processed Time \t "<<endl;
      while(!wait_queue.empty()){
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
            cout<< "     "<<timeArrived<< "  "<<"\t\t"<< curr_run[0].getIndex()<<"\t\t"<<
            curr_run[0].getCpuTime()<<"x\t \t"<<endl;
            //Remove from list of process to run
            wait_queue.erase(wait_queue.begin()+curr_run[0].getIndex());
            //Change the current running to next in queue
            curr_run.erase(curr_run.begin());
            curr_run.push_back(queue[0]);
            queue.erase(queue.begin());
            curr_run[0].setArrived(timeElapsed+1);
          }
        }
        timeElapsed++;
      }
    }

    void runPriority(){
      int timeArrived;
      std::vector<process>queue;
      std::vector<process> curr_run;
      bool init = true;
      bool queueAdded = false;
      cout<< "Time Arrived\t"<< "  Process Index \t   " << "Processed Time \t "<<endl;
      while(!wait_queue.empty()){
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
        cout<<"QUEUE: " << timeElapsed<<endl;
        printProcessDetails(queue);
        cout<<endl;
        printProcessDetails(curr_run);
        cout<<endl;
        //Catch first iteration of  running
        if(!curr_run.empty()){
          curr_run[0].tick();
          //CHECK IF PROCESS ALREADY OVER
          if(curr_run[0].isProcessOver()){
            timeArrived = curr_run[0].getArrived();
            cout<<"PROCESS ENDED : "<<endl;
            cout<< "     "<<timeArrived<< "  "<<"\t\t"<< curr_run[0].getIndex()<<"\t\t"<<
            curr_run[0].getCpuTime()<<"x\t \t"<<endl;
            //Remove from list of process to run
            wait_queue.erase(wait_queue.begin()+curr_run[0].getIndex());
            //Change the current running to next in queue
            curr_run.erase(curr_run.begin());
          }
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

    // //RETURN VECTOR SORTED BASED ON GIVEN SORT STRING
    // std::vector<process> SortBy(std::vector<process> processes, string SortString){
    //    	std::vector<int> Sorter;
    //    	std::vector<int> ProduceBurst;
    //    	std::vector<int> ProduceArrival;
    //    	std::vector<int> ProducePriority;
    //    	std::vector<int> ProduceIndex;
    //     std::vector<process> final_form;
    //     cout<< "ProcessIndex\t"<< "  Arrival Time \t   " << "Burst Time \t " << "Priority Index"<<endl;
    //     for(int i = 0; i<processes.size(); i++){
    //       // cout<< "     "<<processes[i].getIndex() << "  "<<"\t\t"<< processes[i].getArrival()<<"\t\t"<<
    //       // processes[i].getBurst()<<"\t \t"
    //       // <<processes[i].getPrio()<<endl;
    //     //GET SORT BY WHICH STRING, PUSH BACK TO SORTER container to use sort function
    // 		if (SortString == "Arrival"){Sorter.push_back(processes[i].getArrival());}
    // 		else if (SortString == "Priority"){Sorter.push_back(processes[i].getPrio());}
    // 		else if (SortString == "Burst"){Sorter.push_back(processes[i].getBurst());}
    // 		else Sorter.push_back(processes[i].getIndex());
    //       }
    //       //Sorter Index
    //       	int ice = 0;
    //       //Number to compare
    //       	int comp = 0;
    //       // SORT THE SORTER Then compare with original vector
    //         std::sort (Sorter.begin(), Sorter.end(), descOrder);
    //
    //     	cout << endl;
    //
    //           while(!Sorter.empty()){
    //    					int val = Sorter.back();
    //    					if (SortString == "FCFS"){comp = processes[ice].getArrival();}
    //    					else comp = processes[ice].getBurst();
    //
    //     		 		if (SortString == "Arrival"){comp = processes[ice].getArrival();}
    //     				else if (SortString == "Priority"){comp = processes[ice].getPrio();}
    //     				else if (SortString == "Burst"){comp = processes[ice].getBurst();}
    //     				else comp = processes[ice].getIndex();
    //
    //      				if(val == comp){
    //                	ProduceBurst.push_back(processes[ice].getBurst());
    //                	ProduceArrival.push_back(processes[ice].getArrival());
    //                	ProducePriority.push_back(processes[ice].getPrio());
    //                	ProduceIndex.push_back(processes[ice].getIndex());
    //                	moveItemToBack(processes, ice);
    //                	Sorter.pop_back();
    //           	   	ice = -1;
    //    				      }
    //    				  ice++;
    //       		}
    //
    //       	for (int i = 0; i< ProduceArrival.size(); i++){
    // 		    process p = process(ProduceIndex[i], ProduceArrival[i], ProduceBurst[i], ProducePriority[i]);
    //         final_form.push_back(p);
    // 		    }
    //
    // 		return final_form;
    //   }
};
