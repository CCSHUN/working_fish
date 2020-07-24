/*
  同步队列设计要点
    1.提供push get 函数
    2.在get函数要注意避免虚假唤醒
    3.get函数如果在线程池中使用，一定要设置退出标志，否则线程池无法Shutdown,原因
    notify_all唤醒所有线程的时候，以下代码在队列为空的时候会一直调用wait导致无法退出线程池
    while (deque_.empty()) {
      cv_.wait(lk);
    }
*/
#include <iostream>
#include <deque>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <thread>

template<typename T>
class BlockDeque {
 public:
  BlockDeque() :size_(0),break_out_(false) {}
  ~BlockDeque() { Breakout(); } 
  size_t size() {return size_;}
  void Push(const T& value) {
    std::lock_guard<std::mutex> lk(mutex_);
    deque_.push_back(value);  
    size_++;
    cv_.notify_one();
  }
  void Push(T&& value) {
    std::lock_guard<std::mutex> lk(mutex_);
    deque_.push_back(std::move(value)); 
    size_++;
    cv_.notify_one();  
  }
  bool Get(T& value) {
    std::unique_lock<std::mutex> lk(mutex_);
    if (break_out_) {
      return false;
    }
    //避免虚假唤醒
    while (deque_.empty()) {
      cv_.wait(lk);
      if (break_out_) { //用于退出队列
        return false;
      }
    }
    size_--;
    value = deque_.front();
    deque_.pop_front();
    return true;
  }
  void Breakout() {
    std::lock_guard<std::mutex> lock(mutex_);
    break_out_ = true;
    cv_.notify_all();
  }
 public:
  std::atomic<size_t> size_;
  std::condition_variable cv_;
  std::mutex mutex_;
  std::deque<T> deque_;
  bool break_out_; //退出标志
};

int main() {
  BlockDeque<int> block_deque;
  //模拟一个生产者线程 两个消费者线程
  auto producer = [&]()->void {
    for (auto i = 0; i < 10; i++){
      block_deque.Push(i);
      std::cout<<"produce produce task:"<<i<<std::endl;
    }
  };
  auto consumer = [&](int index)->void{
    while (true) {
      int i;
      block_deque.Get(i);
      std::cout<<"consumer "<<index<<" consume task:"<<i<<std::endl;
    }   
  };
  std::thread thread_consumer1(consumer, 1);
  std::thread thread_consumer2(consumer, 2);
  std::thread thread_produce(producer);

  if (thread_produce.joinable())
    thread_produce.join();
  if (thread_consumer1.joinable())
    thread_consumer1.join();
  if (thread_consumer2.joinable())
    thread_consumer2.join();
  return 0;
}