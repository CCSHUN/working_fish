/*
  测试存储类型为std::unique_ptr的vector resize后对象是否析构 
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
  Test(int i) : i_(i) {}
  ~Test() {
    std::cout<<i_<<" deconstructor"<<std::endl;
 }
 int i_;
};

int main() {
  std::vector<std::unique_ptr<Test>> vc_test;  
  vc_test.emplace_back(new Test(1));
  vc_test.emplace_back(new Test(2));
  vc_test.clear(); //对象1、2会被析构
  std::cout<<"vc_test size ="<<vc_test.size()<<std::endl;
  vc_test.resize(1);  //因为已经调用clear函数resize(1)再 resize(0)并不会调用Test析构函数
  vc_test.resize(0);
  while (1) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return 0;
} 