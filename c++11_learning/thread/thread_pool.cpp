/*
  线程池设计要点
  同步队列+线程组
*/
#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <functional>
#include <thread>
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>

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


class ThreadPool {
 typedef std::function<void()> Task;
 public:
  ThreadPool(size_t thread_size) 
      : thread_size_(thread_size), running_(false) {
        assert(0 < thread_size_);
      }
  ~ThreadPool() {
    Shutdown();
  }
  void AddTask(Task task);
  void Start();
  void Shutdown();
 private:
  void ThreadFunc(int index);
 private:
  size_t thread_size_; 
  std::vector<std::unique_ptr<std::thread>> threads_; //线程
  BlockDeque<Task> queue_;
  std::atomic_bool running_;
};

void ThreadPool::Start() {
  running_.store(true);
  threads_.reserve(thread_size_);
  for (auto i = 0; i < thread_size_; i++) {
    threads_.emplace_back(new std::thread(std::bind(&ThreadPool::ThreadFunc, this, i)));
  }
}

void ThreadPool::Shutdown() {
  running_.store(false);
  queue_.Breakout();
  for (auto& it : threads_) {
    if (it->joinable()) {
      it->join();
    }
  }
}

void ThreadPool::ThreadFunc(int index) {
  std::cout<<"threadID "<< index <<" start "<<std::endl;
  while (running_) {
    Task task;
    if (queue_.Get(task)){
      task();
    }
  }
  std::cout<<"threadID "<< index <<" exit "<<std::endl;
}

void ThreadPool::AddTask(Task task) {
  queue_.Push(task);
}

int main () {
  int i = 0;
  auto task = [&]()->void{
    std::cout<<"tasks "<<i<<" add "<<std::endl;
    i++;
  };
  ThreadPool thread_pool(10); //初始化线程池 10个线程
  thread_pool.Start();
  thread_pool.AddTask(task);
  thread_pool.AddTask(task);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  thread_pool.Shutdown();
  //std::this_thread::sleep_for(std::chrono::seconds(3));
}