/*
  模拟主线程给多个子线程发送信号
*/
#include <future>
#include <iostream>
#include <chrono>

int main() {
  std::promise<void> pro1;
  std::shared_future<void> fut1(pro1.get_future());
  auto thread_func1 = [=](){
    std::cout<<std::this_thread::get_id()<<std::endl;
    fut1.wait();
    std::cout<<std::this_thread::get_id()<<" fun1 get signal"<<std::endl;
  };
  auto thread_func2 = [=](){
    std::cout<<std::this_thread::get_id()<<std::endl;
    fut1.wait();
    std::cout<<std::this_thread::get_id()<<"func2 get signal"<<std::endl;
  };
  //std::async(std::launch::deferred, thread_func1);
  //std::async(std::launch::deferred, thread_func2);
  std::thread thread1(thread_func1);
  std::thread thread2(thread_func2);
  std::this_thread::sleep_for(std::chrono::seconds(3));
  pro1.set_value();
  std::cout<<"end"<<std::endl;
  thread1.join();
  thread2.join();
  return 0;
}