#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
 
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memTotal{0.0}, memFree{0.0};
  string key{""}, value{""}, line{""};

  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  if (stream.is_open()){
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');        
      std::replace(line.begin(), line.end(), 'k', ' ');
      std::replace(line.begin(), line.end(), 'B', ' ');
      std::istringstream lineStream(line); 
      while(lineStream >> key >> value){
        if(key == "MemTotal"){
          memTotal = std::stof(value) ;//* 1000;
        }
        if(key == "MemFree"){
          memFree = std::stof(value) ;// * 1000;         
        }
      }
    }
  }  
  return (memTotal - memFree)/memTotal;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string upTime{""}, line{""};  
  std::ifstream filestream( kProcDirectory + kUptimeFilename);

   if (filestream.is_open()){
        std::getline(filestream,  line);
        std::istringstream stringStream(line);
         stringStream >> upTime;
    }
//std::cout << stol(upTime) << std::endl;
  return stol(upTime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) {return 0;}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
    string line{""}, key{""}, value{""};
    vector<string> cpuTimings;
    std::ifstream fileStream(kProcDirectory + kStatFilename);

    if(fileStream.is_open()){
      std::getline(fileStream, line);
      std::istringstream stringStream(line);
      while(stringStream >> key ){
        if(key == "cpu"){
          for(size_t i = 0; i < 10; i++){
            stringStream >> value;
            cpuTimings.push_back(value);
          }
        }
      }
    }
   return cpuTimings;
 }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
   return Pids().size();
 }

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line{""}, key{""}, value{""};
  std::ifstream fileStream(kProcDirectory + kStatFilename);

  if(fileStream.is_open()){
    while(std::getline(fileStream, line)){
      std::istringstream stream(line);
      while(stream >> key >> value){      
        if(key == "procs_running"){          
          int p = std::stoi(value);
          return p ;
        }
      }
    }
  }
  return 0;
}

//  Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line{""};  
  std::ifstream fileStream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);

  if(fileStream.is_open()){
      std::getline(fileStream, line);
        return line;       
  }  
  return string(); 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line{""}, key{""}, value{""};
  std::ifstream fileStream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  if(fileStream.is_open()){
    while(std::getline(fileStream, line)){
         std::replace(line.begin(), line.end(), ':', ' ');   
      std::istringstream stream(line);
      while(stream >> key >> value){
        if(key == "VmSize"){          
          return to_string(std::stoi(value)/1000);
        }
      }
    }  
  }  
  return string();
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line{""}, key{""}, value{""};
  std::ifstream fileStream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  if(fileStream.is_open()){
    while(std::getline(fileStream, line)){
      std::replace(line.begin(), line.end(),':',' ');
      std::istringstream stream(line);
      while(stream >> key >> value){
        if(key == "Uid"){
           return value;
        }
      }
    }  
  }  
  return string();
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line{""}, key{""}, value{""}, uid{""};
  string sPid = LinuxParser::Uid(pid);
  std::ifstream fileStream(kPasswordPath);
  
  if(fileStream.is_open()){
    while(std::getline(fileStream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stream(line);
       while(stream >> key >> value >> uid){
          if(uid == sPid){
            return key;
          }
       }
    }  
  }  
  
  return string(); }

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  
  string line{""}, value{""}; 
  long uptime;
  std::ifstream fileStream(kProcDirectory + std::to_string(pid) + kStatFilename);

  if(fileStream.is_open()){      
    std::getline(fileStream, line);
    std::istringstream stream(line);
    vector<string> stats;
    while(stream >> value){
        stats.push_back(value);
    }

    for(auto i{0}; i < 23; i++){
        if(i==21)
          uptime = std::stol(stats[i])/sysconf(_SC_CLK_TCK);
    }
    return uptime;          
     
  }  
   return 0;
 }
