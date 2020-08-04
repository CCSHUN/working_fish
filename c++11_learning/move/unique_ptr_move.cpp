/*
  unique_ptr支持move操作，
  支持operator bool 操作，查看是否关联对象
*/
#include <pthread.h>
#include <stdio.h>
#include <mutex>
#include <vector>
#include <memory>
#include <iostream>
#include <thread>

class Test {
 public:
  ~Test() {
    std::cout<<"deconstructor"<<std::endl;
 }
};

int main() {
  std::vector<std::unique_ptr<Test>> vc_test;  
  vc_test.emplace_back(new Test);
  vc_test.erase(vc_test.begin(), vc_test.end()); //自动析构对象

  //std::move转移unique_ptr管理权
  std::unique_ptr<Test> vc_test1;
  if (!vc_test1) //没reset前，没有管理对象
    std::cout<<"vc_test1 has no control object"<<std::endl;
  vc_test1.reset(new Test);
  std::unique_ptr<Test> vc_test2 = std::move(vc_test1);
  if (!vc_test1) 
    std::cout<<"vc_test1 moved"<<std::endl;
  while (1) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return 0;
} 