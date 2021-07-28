#include "processor.h"
#include <iostream>
#include <vector>

// Return the aggregate CPU utilization
//With help from https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization() {
       
      Jiffies(LinuxParser::CpuUtilization()) ;
      std::vector<float> times = Jiffies();

      float PrevIdle = PrevIdel() +  PrevIowait();
      float Idle =  times.at(3) +times.at(4);

      float PrevNonIdle = PrevUser() + PrevNice() + PrevSystem()  + PrevIrq() + PrevSoftirq() + PrevSteal();
      float NonIdle = times.at(0) + times.at(1)+times.at(2)+times.at(5)+times.at(6)+times.at(7); 
   
      float PrevTotal = PrevIdle + PrevNonIdle;
      float Total = Idle + NonIdle;

      float totalDiff = Total - PrevTotal;
      float idleDiff = Idle - PrevIdle;
      float result =  (totalDiff - idleDiff)/totalDiff;
      
      PrevUser(times.at(0));
      PrevNice(times.at(1));
      PrevSystem(times.at(2));
      PrevIdel(times.at(3));
      PrevIowait(times.at(4));
      PrevIrq(times.at(5));
      PrevSoftirq(times.at(6));
      PrevSteal(times.at(7));
      PrevGuest(times.at(8));
      PrevGuestnice(times.at(9));

      return  result;
 }
