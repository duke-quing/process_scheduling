class process{
  public:
        int arr_time;
        int burst_time;
        int prio_num;

        process(int arr, int burst, int prio){
          arr_time = arr;
          burst_time=  burst;
          prio_num = prio;
        }
        int getArrival()const{ return arr_time;}
        int getBurst(){ return burst_time; }
        int getPrio(){ return prio_num; }
};
