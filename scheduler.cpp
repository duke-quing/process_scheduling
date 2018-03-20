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
      process findLeastBurst(std::vector<process> *p){
         int newIndex = 0;
         //For loop left hand side of comparison
         for (int lhproc=0; lhproc<p->size(); lhproc++){
           //For loop right hand side of comparison
           newIndex = 0;
           for(int rhproc=0; rhproc<p->size(); rhproc++){
             if((*p)[lhproc].getBurst() > (*p)[rhproc].getBurst()){
               newIndex++;
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
  void sortByBurst(std::vector<process> *p){
      std::vector<process> temp;
      int size = p->size();
      for(int proc_count = 0; proc_count<size; proc_count++){
        temp.push_back(findLeastBurst(p));
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
          sortByBurst(&queue);
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
        for(int curr=0; curr<wait_queue.size(); curr++){
          if(timeElapsed == wait_queue[curr].getArrival()){
            wait_queue[curr].setArrived(timeElapsed);
            queue.push_back(wait_queue[curr]);
            queueAdded = true;
          }
        }
        if(!queue.empty() && queueAdded){
          sortByBurst(&queue);
          if(init){
            curr_run.push_back(queue[0]);
            queue.erase(queue.begin());
            init = false;
          }
          else{
          if(pre_empt(&curr_run, &queue)){
              // cout<<"lhs: "<< curr_run[0].getBurst()<<endl;
              // cout<<"rhs: "<< queue[0].getBurst()<<endl;
              timeArrived = curr_run[0].getArrived();
              cout<< "     "<<timeArrived<< "  "<<"\t\t"<< curr_run[0].getIndex()<<"\t\t"<<
              curr_run[0].getCpuTime()<<"\t \t"<<endl;
              //Reset the cpuTime count of the interrupted process
              curr_run[0].resetCpuTime();
              //Switch to the least burst time from queue
              swapProcess(&curr_run, &queue, 0, queue.size()-1);
              curr_run[0].setArrived(timeElapsed);
            }
          }
          queueAdded = false;
        }
        sortByBurst(&queue);
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
            queue.erase(queue.begin());
            curr_run[0].setArrived(timeElapsed+1);
          }
        }
        // cout<<"SET: "<<timeElapsed<<endl;
        // printProcessDetails(queue);
        // cout<<endl;
        // printProcessDetails(curr_run);
        // cout<<endl;
        timeElapsed++;
      }
    }
    void swapProcess(std::vector<process> *q, std::vector<process> *r, int qindex, int rindex){
      process tempr = (*q)[qindex];
      (*q)[qindex] = (*r)[rindex];
      (*r)[rindex] = tempr;
    }
    bool pre_empt(std::vector<process> *lhs, std::vector<process> *rhs){
      process holder = findLeastBurst(rhs);
      if((*lhs)[0].getBurst() > holder.getBurst()){
        rhs->push_back(holder);
        return true;
      }
      rhs->push_back(holder);
      return false;
    }

//DESC ORDER FUNC FOR SORT ALGO
    bool descOrder (int i, int j) { return (i>j); }
//MOVE VECTOR OBJECT TO THE BACK
    template <typename T>
    void moveItemToBack(std::vector<T>& v, size_t itemIndex)
    {
       std::swap(v[itemIndex], v.back()); // or swap with *(v.end()-1)
    }

    // Tick Using FCFS Schedule
    void runRobin(int quantum){
      int timeArrived = 0;
      int cpuTime = 0;
      int qcount = quantum;
      int counter = 0;
      int turn = 0;
      //FCFS QUEUE : PROCESS THAT RUNS
      std::vector<process>queue;
      std::vector<int>burst;
    
      cout<< "Time Arrived\t"<< "  Process Index \t   " << "Processed Time \t "<<endl;

      while(!wait_queue.empty()){
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
            cout<< "     "<<timeArrived<< "  "<<"\t\t"<< queue[0].getIndex()<<"\t\t"<<
            queue[0].getCpuTime()<<"x\t \t" << "TAT ->" << turn <<"\t \t" << "WT ->" << turn - burst[counter] << endl;
            queue[0].resetCpuTime();
            queue.erase(queue.begin());
            wait_queue.erase(wait_queue.begin()+queue[0].getIndex());
            queue[0].setArrived(timeElapsed);
            qcount = quantum;
            counter++;
          }

        }
          
          queue[0].tick();
          timeElapsed++;
          qcount--;

      }
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
