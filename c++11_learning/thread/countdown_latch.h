/*
  实现countdown功能，参考muduo countdown
  原理和应用：
    https://zhuanlan.zhihu.com/p/95835099
*/
#include <cassert>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

class CountdownLatch {
 public:
  explicit CountdownLatch(int count = 1) 
    : count_(count) { 
      assert(count > 0); 
    }
  ~CountdownLatch() = default;
  CountdownLatch(const CountdownLatch& other) = delete;
  CountdownLatch& operator=(const CountdownLatch& other) = delete;

  void Wait() {
    std::unique_lock<std::mutex> lk(mutex_);
    while (0 < count_)
      cv_.wait(lk);
  }

  void CountDown() {
    std::lock_guard<std::mutex> lk(mutex_);
    count_--;
    if (0 == count_) {
      cv_.notify_all();
    }
  }

  int GetCount() const {
    std::lock_guard<std::mutex> lk(mutex_);
    return count_;
  }

 private:
  size_t count_;
  mutable std::mutex mutex_; //在const修饰的函数使用，需要定义为mutable
  std::condition_variable cv_;
};