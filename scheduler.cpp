#include "process.h"
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class Scheduler{
  private:
      std::vector<process> process_vector;
  public:
      Scheduler(){}
      Scheduler(std::vector<process> p){
        process_vector = p;
      }
      void inputProcess(int arr_time, int burst_time, int prio_num){
        process p = process(arr_time, burst_time, prio_num);
        process_vector.push_back(p);
      }
      void sortByArrival(){
        sort(&process_vector.begin(), &process_vector.end(), ascArrival);
      }
      bool ascArrival(const process & p, const process & q){
        return p.arr_time < q.arr_time;
      }
    void printProcessDetails(){
      cout<< "ProcessIndex\t"<< "  Arrival Time \t   " << "Burst Time \t " << "Priority Index"<<endl;
      for(int i = 0; i<process_vector.size(); i++){
        cout<< "     "<<i << "  "<<"\t\t"<< process_vector.at(i).getArrival()<<"\t\t"<<
        process_vector.at(i).getBurst()<<"\t \t"
        <<process_vector.at(i).getPrio()<<endl;

      }
    }
};
