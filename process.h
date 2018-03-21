class process{
    int arr_time;
    int burst_time;
    int prio_num;
    int timeArrived;
    int index;
    int cpuTime;
    int pos;
public:
        process(){}
        process(int arr, int burst, int prio, int idx){
          arr_time = arr;
          burst_time=  burst;
          prio_num = prio;
          timeArrived = 0;
          cpuTime = 0;
          index = idx;
          pos = 0;
        }
        int getArrival(){ return arr_time;}
        int getBurst(){ return burst_time; }
        int getPrio(){ return prio_num; }
        int getArrived(){ return timeArrived; }
        int getIndex(){ return index; }
        int getCpuTime(){ return cpuTime; }
        int getPos(){ return pos; }

        void setBurst(int val){
          burst_time = val;
        }
        void setArrived(int val){
          timeArrived = val;
        }
        void resetCpuTime(){
          cpuTime = 0;
        }
        void setPos(int val){
          pos = val;
        }
        void tick(){
          burst_time-=1;
          cpuTime++;
        }
        bool operator==( process & rhs){
          return (arr_time==rhs.getArrival() && burst_time==rhs.getBurst() && prio_num==rhs.getPrio());
        }
        bool isProcessOver(){
          return (burst_time==0);
        }


};
