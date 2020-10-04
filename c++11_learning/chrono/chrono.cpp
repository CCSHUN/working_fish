/*
  std::chrono::system_clock用于求系统真实时间
  std::chrono::steady_clock 求相对时间
*/
#include <chrono>
#include <iostream>

int main() {
  auto start = std::chrono::system_clock::now();
  auto end = std::chrono::system_clock::now();
  auto during = end - start;
  std::cout<<during.count()<<std::endl; //纳秒计数

  auto duration_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout<<duration_ms.count()<<std::endl; //微秒计数

  auto duration_mil = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout<<duration_mil.count()<<std::endl; //毫秒计数

  //稳定时钟
  auto start1 = std::chrono::steady_clock::now();
  auto end1 = std::chrono::steady_clock::now();
  auto duration1 = end1 - start1;
  std::cout<<duration1.count()<<std::endl;

  //duration test
  std::chrono::seconds sec{128};
  std::cout<<sec.count()<<std::endl;
  return 0;
}