/*
  TAS模型自旋锁实现
  使用lock_guard管理自定义自旋锁
  小技巧：
   这里只需要将lock和unlock函数名和std::mutex同名即可，这样就可以用std::lock_guard管理自定义自旋锁了
*/
#include <atomic>
#include <thread>
#include <mutex>
#include <iostream>

class spin_mutex {
  std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
public:
  spin_mutex() = default;
  spin_mutex(const spin_mutex&) = delete;
  spin_mutex& operator= (const spin_mutex&) = delete;
  void lock() {
    while(flag_.test_and_set(std::memory_order_acquire));
  }
  void unlock() { 
    flag_.clear(std::memory_order_release);
  }
};

int num = 0;
spin_mutex sm;

void thread_proc()
{

  for(int i = 0; i < 100000; ++i) {
    std::lock_guard<spin_mutex> lock(sm);
    ++num;
  }
}

int main()
{
  std::thread td1(thread_proc), td2(thread_proc);
  td1.join();
  td2.join();
  std::cout << num << std::endl;
  return 0;
}