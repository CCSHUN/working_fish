#include <thread>
#include <future>
#include <iostream>
#include <string>
#include <chrono>
#include <functional>

class Test {
 public:
  std::string getString(std::string sz) {
    return sz;
  }
  //阻塞函数
  std::string getStringBlock(std::string sz) {
    while (true);
    return sz;
  }
};

int main () {
  std::cout<<"main thread_id="<<std::this_thread::get_id()<<std::endl;
  //普通函数
  auto func1 = []()->int{
    return 1;
  };
  std::future<int> fut1 = std::async(func1);
  std::cout<<fut1.get()<<std::endl;

  //类成员函数
  Test test;
  std::future<std::string> fut2 = std::async(&Test::getString, &test, "hello");
  std::cout<<fut2.get()<<std::endl;
  
  //类成员函数阻塞 std::launch::deferred表示延迟调用
  std::future<std::string> fut3 = std::async(std::launch::deferred, &Test::getStringBlock, &test, "world");
  //std::cout<<fut3.get()<<std::endl;
  std::cout<<"Blocking"<<std::endl;

  return 0;
}