#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>
#include <string>
#include "linux_parser.h"

class Processor {
 public:

  float Utilization();  // TODO: See src/processor.cpp
     //Setters
    
    void Jiffies(std::vector<std::string> data ){
      jiffies.clear();
      for(auto s : data){        
        jiffies.push_back(std::stof(s));
      }
    }
 
    void PrevUser(float u){
      prevUser = u;
    }
    void PrevNice(float n){
      prevNice = n;
    }
    void PrevSystem(float s){
      prevSystem = s;
    }
    void PrevIdel(float i){
      prevIdle = i;
    }
    void PrevIowait(float i){
      prevIowait = i;
    }
    void PrevIrq(float i){
      prevIrq = i;
    }
    void PrevSoftirq(float i){
      prevSoftirq = i;
    }
    void PrevSteal(float s){
      prevSteal = s;
    }
    void PrevGuest(float g){
      prevGuest = g;
    }
    void PrevGuestnice(float g){
      prevGuestnice = g;
    }
    //Getters
     std::vector<float> Jiffies(){      
       return jiffies;      
    }
    float PrevUser() {
      return this->prevUser;
    }
    float PrevNice() const {
       return prevNice;
    }
    float PrevSystem() const {
      return prevSystem;
    }
    float PrevIdel() const {
      return  prevIdle;
    }
    float PrevIowait() const {
      return prevIowait;
    }
    float PrevIrq() const {
      return prevIrq;
    }
    float PrevSoftirq() const {
      return prevSoftirq;
    }
    float PrevSteal() const {
      return prevSteal;
    }
    float PrevGuest() const {
      return prevGuest;
    }
    float PrevGuestnice() const {
      return prevGuestnice;
    }

    std::vector<float> OldJiffies() {
      return oldjiffies;
    }
       void OldJiffies( std::vector<float> v) {
          for(auto i : v)
            oldjiffies.push_back(i);
    }
  // TODO: Declare any necessary private members
private:
    std::vector<float> jiffies;
    int count;
    std::vector<float> oldjiffies;
    float prevUser{0};
    float prevNice{0};
    float prevSystem{0};
    float prevIdle{0};
    float prevIowait{0};
    float prevIrq{0};
    float prevSoftirq{0};
    float prevSteal{0};
    float prevGuest{0};
    float prevGuestnice{0};
};

#endif