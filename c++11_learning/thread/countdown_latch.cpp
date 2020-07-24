/*
  模拟主线程等待 两个子线程初始化完成

*/
#include <functional>
#include <iostream>
#include "countdown_latch.h"

int main() {
  CountdownLatch coutdown_latch(2);
  auto thread_func1 = [&](){
    coutdown_latch.CountDown();
    std::cout<<"thread 1 inited"<<std::endl;
    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  };
  auto thread_func2 = [&](){
    coutdown_latch.CountDown();
    std::cout<<"thread 2 inited"<<std::endl;
    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  };
  std::thread thread2(thread_func2);
  std::thread thread1(thread_func1);
  coutdown_latch.Wait(); //主线程等待子线程初始化完成
  std::cout<<"all thread inited"<<std::endl;
  while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return 0;
}