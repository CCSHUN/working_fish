/*
  验证std::launch::deferred 和 std::launch::async运行方式
    std::launch::deferred不新建线程执行，
    std::launch::async新建线程执行
  
  注：不指定默认为std::launch::async

  详解c++ 并发编程4.2.1
*/
#include <thread>
#include <future>
#include <iostream>
#include <string>
#include <chrono>
#include <functional>

int main () {
  std::cout<<"main thread_id="<<std::this_thread::get_id()<<std::endl;

  auto func1 = [](int index){
    std::cout<<index<<"="<<std::this_thread::get_id()<<std::endl; 
  };

  std::future<void> fut1 = std::async(std::launch::async, func1, 1); 

  std::future<void> fut2 = std::async(std::launch::deferred, func1, 2); 
  fut2.get();

  std::future<void> fut3 = std::async(std::launch::deferred | std::launch::async, func1, 3);
  return 0;
}