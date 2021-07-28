#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


Process::Process(int p) : pid_(p) {}

// Return this process's ID
int Process::Pid() {
      
     return this->pid_;
}

// Return this process's CPU utilization
//with help from https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
float Process::CpuUtilization() const { 
     string line{""}, value{""};
     float uptime{0.0}, utime{0.0}, stime{0.0}, cutime{0.0}, cstime{0.0}, starttime{0.0}, hertz{0.0}, totalTime{0.0}, seconds{0.0}, cpuUsage{0.0};
     
     std::ifstream fileStream ( LinuxParser::kProcDirectory + std::to_string(this->pid_) + LinuxParser::kStatFilename);
     std::getline(fileStream, line); 

     std::istringstream stream(line);
     vector<string> stats;
     
     //push stats to a vector to use in a for loop
     while(stream >> value){
          stats.push_back(value);
     }
     //Loop over a switch case, and extract the relevant data
     for(int i{0};i<22;i++){
          switch (i)
          {
               case 13:
                    utime = std::stof(stats[i]);
                    break;
               case 14:
                    stime = std::stof(stats[i]);
                    break;
               case 15:
                    cutime = std::stof(stats[i]);                    
                    break;
               case 16:
                    cstime = std::stof(stats[i]);                    
                  break;
               case 21:
                    starttime = std::stof(stats[i]);                    
                    break;
               default:
                    break;
          }
     }

     uptime = LinuxParser::UpTime();
     hertz =  (float)sysconf(_SC_CLK_TCK);

     //total time spent for the process:
     totalTime = utime + stime;
     
     //include the time from children processes
     totalTime += cutime + cstime;

     //total elapsed time in seconds since the process started:
     seconds = uptime - (starttime / hertz);

     //CPU usage percentage
     cpuUsage = (totalTime / hertz) / seconds;


     return cpuUsage;
 }

// Return the command that generated this process
string Process::Command() {
 
      return LinuxParser::Command(this->pid_);
}

// Return this process's memory utilization
string Process::Ram() {

      return LinuxParser::Ram(this->pid_); 
}

// Return the user (name) that generated this process
string Process::User() { 

     return LinuxParser::User(this->pid_);
}

// Return the age of this process (in seconds)
long int Process::UpTime() { 
     
     return LinuxParser::UpTime(this->pid_);
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a[[maybe_unused]]) const {

      return a.CpuUtilization() < this->CpuUtilization();   
}
