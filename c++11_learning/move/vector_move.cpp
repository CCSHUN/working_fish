/*
  测试存储类型为std::unique_ptr的vector move对象后是否析构 
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
  std::unique_ptr<Test> ptr = std::move(vc_test.back()); //此时不析构
  vc_test.pop_back();//此时不析构，因为对象控制权已经转移
  printf("%s_%d\n",__FILE__, __LINE__);
  ptr.reset(); //此时析构，ptr拥有对象控制权，ptr reset会调用析构函数
  printf("%s_%d\n",__FILE__, __LINE__);
  while (1) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return 0;
} 