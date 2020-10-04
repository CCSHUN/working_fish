/*
  CAS模型自旋锁c++11实现
    其中flag.compare_exchange_strong(expected, true, std::memory_order_acquire)等价于flag.compare_exchange_strong(expected, true, std::memory_order_acquire, std::memory_order_acquire)。获得自旋锁的所有权的充分必要条件是当且仅当compare_exchange_strong比较成功且成功改变flag的值为true，也就是说当比较失败时是没有必要执行acquire操作的

    参考博客https://blog.csdn.net/sharemyfree/article/details/47338001
 */
#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>

class SpinMutex {
 public:
  SpinMutex() = default;
  SpinMutex(const SpinMutex& other) = delete;
  SpinMutex& operator=(const SpinMutex& other) = delete;
  void lock() { //加锁，
    bool expected = false;
    while (!flag_.compare_exchange_strong(expected, true, std::memory_order_acquire, std::memory_order_relaxed)) //循环等待直到获取到锁
      expected = false; //flag_!=expected时会 将flag_的值赋值到expected，所以需要恢复expected为false
  }
  void unlock() {
    flag_.store(false, std::memory_order_release);
  }
 private:
  std::atomic<bool> flag_ = ATOMIC_VAR_INIT(false); //默认不上锁
};

int num = 0;
SpinMutex sm;

void thread_proc()
{
  for(int i = 0; i < 100000; ++i) {
    std::lock_guard<SpinMutex> lock(sm);
    ++num;
  }
}

int main() {
  std::thread td1(thread_proc), td2(thread_proc);
  td1.join();
  td2.join();
  std::cout << num << std::endl;

  return 0;
}