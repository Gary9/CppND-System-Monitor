#include "ncurses_display.h"
#include "system.h"
#include <iostream>

int main() {
  System system;

  //  std::cout<<"Memory utilization "<<system.MemoryUtilization() * 100<<std::endl;
   //  std::cout<< "UpTime " << system.UpTime() <<std::endl;
  //   std::cout<< "OS " << system.OperatingSystem() <<std::endl;
   //  std::cout<< "Kernel " << system.Kernel() <<std::endl;
   // long a {0};
  // while(1){
   //  for(size_t i{0}; i < 4; i++)
 //  std::cout << system.Cpu().Utilization() << std::endl ; //}
  //  std::cout << system.TotalProcesses() << std::endl;
//  for(auto i : system.Processes())
 //   std::cout << i.Pid();
  NCursesDisplay::Display(system);
}