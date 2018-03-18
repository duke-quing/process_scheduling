class process{
    int arr_time;
    int burst_time;
    int prio_num;
    int timeArrived;
    int index;
public:

        process(int arr, int burst, int prio, int idx){
          arr_time = arr;
          burst_time=  burst;
          prio_num = prio;
          timeArrived = 0;
          index = idx;
        }
        int getArrival(){ return arr_time;}
        int getBurst(){ return burst_time; }
        int getPrio(){ return prio_num; }
        int getArrived(){ return timeArrived; }
        int getIndex(){ return index; }

        void setBurst(int val){
          burst_time = val;
        }
        void setArrived(int val){
          timeArrived = val;
        }
        bool operator==( process & rhs){
          return (arr_time==rhs.getArrival() && burst_time==rhs.getBurst() && prio_num==rhs.getPrio());
        }
};
