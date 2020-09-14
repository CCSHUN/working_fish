# working_fish

摸鱼技术那家强

# 1.写时复制技术（copy_on_write，现代c++已放弃）

## 1.1std::string COW策略

参考博客[标准C＋＋类string的Copy-On-Write技术（一）_陈皓专栏　【空谷幽兰，心如皓月】-CSDN博客_c++ copy-on-write](https://blog.csdn.net/haoel/article/details/24058)

[标准C＋＋类string的Copy-On-Write技术（二）_陈皓专栏　【空谷幽兰，心如皓月】-CSDN博客_string的copy-on-write技术](https://blog.csdn.net/haoel/article/details/24065?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-3.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-3.nonecase)

[标准C＋＋类string的Copy-On-Write技术（三）_陈皓专栏　【空谷幽兰，心如皓月】-CSDN博客_basic_string copy on write](https://blog.csdn.net/haoel/article/details/24077?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase)[标准C＋＋类string的Copy-On-Write技术（三）_陈皓专栏　【空谷幽兰，心如皓月】-CSDN博客_basic_string copy on write](https://blog.csdn.net/haoel/article/details/24077?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase)[标准C＋＋类string的Copy-On-Write技术（三）_陈皓专栏　【空谷幽兰，心如皓月】-CSDN博客_basic_string copy on write](https://blog.csdn.net/haoel/article/details/24077?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase)

[c++再探string之eager-copy、COW和SSO方案 - CTHON - 博客园](https://www.cnblogs.com/cthon/p/9181979.html)[c++再探string之eager-copy、COW和SSO方案 - CTHON - 博客园](https://www.cnblogs.com/cthon/p/9181979.html)[c++再探string之eager-copy、COW和SSO方案 - CTHON - 博客园](https://www.cnblogs.com/cthon/p/9181979.html)

### 1.11初探std::string COW策略

下面测试代码在VC6.0运行，g++ 5.x以下版本运行，体现了std::string COW策略，原因后面再讲

```
/*
  下面的例子体现了copy on write机制
  注意点：
    gcc 5.x.x之后std::string 就不在使用cow机制
    所以
*/

#include <cstdio>
#include <string>
#include <iostream>

using namespace std;
int main() {
  string str1("hello_world");
  string str2 = str1;

  //在c++11(可以用vc6.0运行)之前可以看到两个地址一样，说明指向同一内存空间
  //但是高版本的编译器不再使用cow机制
  print("shared the address\n");
  printf("str address: %x\n", str1.c_str());
  printf("str address: %x\n", str2.c_str());

  str2[0] = 'H'; //直到写时才赋值，此时str2的地址改变
  print("copy on write\n");
  printf("str address: %x\n", str1.c_str());
  printf("str address: %x\n", str2.c_str());
  return 0;
}
```

输出结果：

       ![](/Users/chen/Library/Application%20Support/marktext/images/2020-07-20-17-54-06-image.png)

由输出结果可知，直到程序运行到str2[0] = 'H';语句地址才发生改变，体现了COW策略

### 1.12std::string COW策略的实现

COW策略存在以下缺点：

### 1.11COW缺点1

实际的`string`COW实现中，对于什么是”写操作”的认定和我们的直觉是不同的，考虑以下代码：

```
string a = "Hello";
string b = a;
cout << b[0] << endl;
```

以上代码显然没有修改`string b`的内容，此时似乎`a`和`b`是可以共享一块内存的，然而由于`string`的`operator[]`和`at()`会返回某个字符的引用，此时无法准确的判断程序是否修改了`string`的内容，为了保证COW实现的正确性，`string`只得统统认定`operator[]`和`at()`具有修改的“语义”。

这就导致`string`的COW实现存在诸多弊端（除了上述原因外，还有线程安全的问题，可进一步阅读文末参考资料），因此只有老版本的GCC编译器和少数一些其他编译器使用了此方式，VS、Clang++、GCC 5.x等编译器均放弃了COW策略，转为使用SSO策略。

### 1.12COW缺点2

       COW策略对多线程不友好

**原因讨论**

**参考**：

[C++ 工程实践(10)：再探std::string - 陈硕 - 博客园](https://www.cnblogs.com/Solstice/archive/2012/03/17/2403335.html)

https://www.zhihu.com/question/63078737/answer/216204035

https://stackoverflow.com/questions/31228579/why-is-cow-stdstring-optimization-still-enabled-in-gcc-5-1

## 1.2std::string SSO策略

# 2.std::string从COW到string_view

# 3.struct hack

参考：

[C语言中的柔性数组 C语言结构体中char[0]和char[1]的用法_imxiangzi的专栏-CSDN博客_c结构体 char[1]](https://blog.csdn.net/imxiangzi/article/details/90758437?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase)

测试代码

```
/*
  struct hack编程技巧
    由输出结果可见struct_hack1、struct_hack2、struct_hack3效果几乎等价
    其中 struct_hack1、struct_hack2结构体占用空间最小，都为4个字节
    struct_hack3结构体占用16字节，虽然struct_hack3结构体占用空间最大，
    但是业界更多地采用了struct_hack3的做法，原因是data[0] 和data[]在
    旧标准不支持
  最后：
    1.struct_hack0被弃用，原因是申请时需要分配两次空间，先申请结构体的空间，
    然后再申请data的空间，释放的时候先释放data指向的空间，然后释放结构体的空间
    增加了操作的复杂度，此外，这样的做法造成了空间的不连续，不易于管理

    2.struct_hack4的做法在旧标准的std::string cow策略就是这样用的

  总结：
    struct_hack1、struct_hack2、struct_hack3的做法操作最简单，struct_hack4最复杂，struct_hack0不可取
    建议采用struct_hack3的做法
*/

#include <cstdio>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>

/*
  以下定义方式编译均可通过
*/
struct struct_hack0 { 
  int size;
  char* data;
};
struct struct_hack1 {
  int size;
  char data[];
};

struct struct_hack2 {
  int size;
  char data[0];
};

struct struct_hack3 {
  int size;
  char data[1];
};

struct struct_hack4 {
  int size;
  char* data[1]; 
};


//等价于struct_hack4
struct struct_hack5 {
  int size;
  char* data[0]; 
};

/*
  假设代码在64位机器上运行
 */
int main() {
  std::cout<<sizeof(int)<<std::endl;//4
  std::cout<<sizeof(void*)<<std::endl;//8
  std::cout<<sizeof(struct_hack0)<<std::endl;//16
  std::cout<<sizeof(struct_hack1)<<std::endl;//4
  std::cout<<sizeof(struct_hack2)<<std::endl;//4
  std::cout<<sizeof(struct_hack3)<<std::endl;//8
  std::cout<<sizeof(struct_hack4)<<std::endl;//16

  int len = 11;
  char str[] = "hello_world";

  printf("--------begin hack0 analyse-------\n");
  struct_hack0* hack0 = (struct_hack0*)malloc(sizeof(struct_hack0) + len + 1);
  //strcpy(&hack0->data[0], str);//段错误
  printf("hack0 address=%p\n", hack0); //堆空间地址
  printf("hack0->data address=%p\n", hack0->data); //nil
  printf("&hack0->data address=%p\n", &hack0->data);//hack0地址 + 8
  printf("&hack0->data + 1 address=%p\n", &hack0->data + 1);//hack0地址 + 16
  printf("--------end hack0 analyse----------\n\n");

  printf("--------begin hack1 analyse-------\n");
  struct_hack1* hack1 = (struct_hack1*)malloc(sizeof(struct_hack1) + len + 1);
  memcpy(hack1->data, str, len);
  printf("hack1 address=%p\n", hack1); //堆空间地址
  printf("hack1->data address=%p\n", hack1->data); //hack + 4
  printf("hack1->data address=%p\n", hack1->data + 1);//hack + 5
  printf("hack1->data =%s\n", hack1->data);
  printf("--------end hack1 analyse----------\n\n");

  printf("--------begin hack2 analyse-------\n");
  struct_hack2* hack2 = (struct_hack2*)malloc(sizeof(struct_hack2) + len + 1);
  memcpy(hack2->data, str, len);
  printf("hack2 address=%p\n", hack2); //堆空间地址
  printf("hack2->data address=%p\n", hack2->data); //hack + 4
  printf("hack2->data address=%p\n", hack2->data + 1);//hack + 5
  printf("hack2->data =%s\n", hack2->data);
  printf("--------end hack2 analyse----------\n\n");

  printf("--------begin hack3 analyse-------\n");
  struct_hack3* hack3 = (struct_hack3*)malloc(sizeof(struct_hack3) + len + 1);
  memcpy(hack3->data, str, len);
  printf("hack3 address=%p\n", hack3); //堆空间地址
  printf("hack3->data address=%p\n", hack3->data); //hack + 4
  printf("hack3->data address=%p\n", hack3->data + 1);//hack + 5
  printf("hack3->data =%s\n", hack3->data);
  printf("--------end hack3 analyse----------\n\n");

  printf("--------begin hack4 analyse-------\n");
  struct_hack4* hack4 = (struct_hack4*)malloc(sizeof(struct_hack4) + len + 1);
  memcpy(hack4->data, str, len);
  printf("hack4 address=%p\n", hack4); //堆空间地址
  printf("hack4->data address=%p\n", hack4->data); //hack + 8
  printf("hack4->data address=%p\n", hack4->data + 1);//hack + 16
  printf("&hack4->data=%s\n", &hack4->data[0]);//这样才能真正取出data数据
  //printf("&hack4->data=%s\n", hack4->data[0]); //段错误 自己画内存分布图体验
  printf("--------end hack4 analyse-------\n\n");

  printf("--------begin hack5 analyse-------\n");
  struct_hack5* hack5 = (struct_hack5*)malloc(sizeof(struct_hack5) + len + 1);
  memcpy(hack5->data, str, len);
  printf("hack5 address=%p\n", hack5); //堆空间地址
  printf("hack5->data address=%p\n", hack5->data); //hack + 8
  printf("hack5->data address=%p\n", hack5->data + 1);//hack + 16
  printf("&hack5->data=%s\n", &hack5->data[0]);//这样才能真正取出data数据
  //printf("&hack5->data=%s\n", hack5->data[0]); //段错误 自己画内存分布图体验
  printf("--------end hack5 analyse-------\n\n");
  return 0;
}
```

运行结果：

![](/Users/chen/Library/Application%20Support/marktext/images/2020-07-21-10-17-13-image.png)

续上

![](/Users/chen/Library/Application%20Support/marktext/images/2020-07-21-10-17-49-image.png)

## struct hack编程技巧

 由输出结果可见struct_hack1、struct_hack2、struct_hack3效果几乎等价
 其中 struct_hack1、struct_hack2结构体占用空间最小，都为4个字节
 struct_hack3结构体占用16字节，虽然struct_hack3结构体占用空间最大，
 但是业界更多地采用了struct_hack3的做法，原因是data[0] 和data[]在
 旧标准不支持
 最后：
     1.struct_hack0被弃用，原因是申请时需要分配两次空间，先申请结构体的空间，
 然后再申请data的空间，释放的时候先释放data指向的空间，然后释放结构体的空间
 增加了操作的复杂度，此外，这样的做法造成了空间的不连续，不易于管理

    2.struct_hack4的做法在旧标准的std::string cow策略就是这样用的

总结：
 struct_hack1、struct_hack2、struct_hack3的做法操作最简单，struct_hack4最复杂，struct_hack0不可取
**<mark> 建议采用struct_hack3的做法</mark>**

# 4.记一次生产者消费者线程疑似消费者线程卡死的调试

代码的功能是 模拟生产者和消费者的生产消费过程

 1.一个生成者，两个消费者

### 以下的有问题的代码（消费者线程疑似卡死）

```代码的功能是
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
    while (!deque_.empty()) {
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
```

上诉代码运行结果：

![](/Users/chen/Library/Application%20Support/marktext/images/2020-07-22-15-00-46-image.png)

上述代码的运行正常来说是生产者正常生成任务，2个消费者也能正常消费到任务，但从实际效果来看，生产者正常生成数据，但是消费者线程只有消费者2消费到任务0，剩下的任务2~9没有被消费

#### 好了，问题如何解决呢？这里提供两种思路，

第一种是用GDB调试，让程序跑起来，执行到问题出现的地方，然后ctrl+c让程序中断掉，bt指令看堆栈，看看程序卡在哪里了,这种一般应该在开发的过程中调试解决

第二种是 代码运行在生产环境了，都是release版本，没有调试信息，那么应该如何解决呢，这里可以用<mark>ps+pstack+strace</mark>

    第一步：ps查看程序的pid

    ![](/Users/chen/Library/Application%20Support/marktext/images/2020-07-22-15-16-45-image.png)

 第二步：pstack 查看程序的的堆栈信息

    ![](/Users/chen/Library/Application%20Support/marktext/images/2020-07-22-15-23-27-image.png)

由于代码比较简单，从pstack的堆栈信息已经可以知道消费者线程1和消费者线程2卡在pthread_cond_wait这里，等待条件变量。为了调试效果我们再看看看strace输出结果吧

第三步：strace查看消费者线程系统调用

![](/Users/chen/Library/Application%20Support/marktext/images/2020-07-22-15-30-42-image.png)

果不其然。消费者线程卡死在pthread_cond_wait里面了

根据江湖广为流传的就近原则，找到cv_.wait()函数调用了pthread_con_wait，找到以下代码

![](/Users/chen/Library/Application%20Support/marktext/images/2020-07-22-15-48-39-image.png)

脑瓜子嗡嗡的吧，这串代码原本是用于避免虚假唤醒的，可是上面的写法变成了，当不为空是一直等待条件变量，所以，程序自然就卡死了，<mark>所以把!去掉</mark>，运行效果如下：

![](/Users/chen/Library/Application%20Support/marktext/images/2020-07-22-15-58-15-image.png)

    生产者生成的任务，消费者均已消费

### 修改后的代码如下

```
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
```

# 5.CountdownLatch的学习与使用

    今天阅读陈硕muduo多线程设计精要，学习了CountDownLatch的使用，觉得在开发中有以下应用

## 参考文章

    1.muduo库

    2.https://zhuanlan.zhihu.com/p/95835099

## 应用场合

1.在多线程编程中，主线程等待子线程初始化完成

2.在高性能服务器交流群，看到有小伙伴提问

![](/Users/chen/Library/Application%20Support/marktext/images/2020-07-23-14-47-59-image.png)

个人觉得，可以用以下思想，创建线程执行后台任务，前台调用wait阻塞，线程完成任务后调用countdown，当countdown为0是，通知主线程。

## 完整代码

头文件

```
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
```

cpp

```
/*
  模拟主线程等待 两个子线程初始化完成

*/
#include <functional>
#include <iostream>
#include "countdown_latch.h"

int main() {
  CountdownLatch coutdown_latch(2);
  auto thread_func1 = [&](){
    coutdown_latch.CountDown();
    std::cout<<"thread 1 inited"<<std::endl;
    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  };
  auto thread_func2 = [&](){
    coutdown_latch.CountDown();
    std::cout<<"thread 2 inited"<<std::endl;
    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  };
  std::thread thread2(thread_func2);
  std::thread thread1(thread_func1);
  coutdown_latch.Wait(); //主线程等待子线程初始化完成
  std::cout<<"all thread inited"<<std::endl;
  while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return 0;
}
```

## 运行效果

![](/Users/chen/Library/Application%20Support/marktext/images/2020-07-23-14-54-11-image.png)

# 6 记一次多线程死锁调试过程

注意点::在同一线程可重入，在多线程不可以重入

下面代码在不同操作系统运行结果不一样

```
![](file:///Users/chen/Library/Application%20Support/marktext/images/2020-07-23-18-51-20-image.png)
```

std::mutex mutex;

mutex.lock()

mutex.lock()

```

```

运行环境（gcc version 8.3.1 20190507 (Red Hat 8.3.1-4) (GCC)）：

![](/Users/chen/Library/Application%20Support/marktext/images/2020-07-23-18-49-20-image.png)

std::mutex 在上述运行环境不会死锁，换句话来说，在同一线程内支持重入，找了好久都没找到原因，

然后换了个环境调试（Apple clang version 11.0.3 (clang-1103.0.32.62)）

![](/Users/chen/Library/Application%20Support/marktext/images/2020-07-23-18-55-27-image.png)

在clang调试，发现同样的代码在死锁，不同运行环境差距竟然这么大？日了狗了

进入正题，死锁应该怎么调试呢，上代码

```
#include <stdio.h>
#include <mutex>
#include <thread>
#include <iostream>

std::mutex mutex;
std::vector<Foo> foos;
int main()
{
  auto func = [&](int index){
    while(1)
    {
      mutex.lock();
      std::cout<<"lock "<<std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  };
  std::thread thread1(func, 1);
  std::thread thread2(func, 2);

  thread1.join();
  thread2.join();
}
```

上诉代码在线程1 和线程2 对mutex重复上锁，不管在哪个操作系统，患无疑问都是死锁的，

那么怎么用gdb调试呢

1.先让程序跑起来

![](/Users/chen/Library/Application%20Support/marktext/images/2020-07-23-19-00-04-image.png)

2.用gdb attch pid 然后bt看堆栈，因为是多线程程序，此时bt看不错什么所以然，多线程调试应该用info threads查看线程调用，此时可以清楚地看到线程2，3都在等锁，很明显是死锁了

![](/Users/chen/Library/Application%20Support/marktext/images/2020-07-23-19-01-49-image.png)

# 7.智能指针

参考：

    https://www.cnblogs.com/goya/p/11967200.html

## 7.1enable_shared_from_this

# 关于enable_shared_from_this的原理分析

首先要说明的一个问题是：如何安全地将this指针返回给调用者。一般来说，我们不能直接将this指针返回。  
想象这样的情况，该函数将this指针返回到外部某个变量保存，然后这个对象自身已经析构了，但外部变量并不知道，此时如果外部变量使用这个指针，就会使得程序崩溃。

使用智能指针shared_ptr看起来是个不错的解决方法。但问题是如何去使用它呢？我们来看如下代码：

```c++
#include <iostream>
#include <boost/shared_ptr.hpp>
class Test
{
public:
    //析构函数
    ~Test() { std::cout << "Test Destructor." << std::endl; }
    //获取指向当前对象的指针
    boost::shared_ptr<Test> GetObject()
    {
        boost::shared_ptr<Test> pTest(this);
        return pTest;
    }
};
int main(int argc, char *argv[])
{
    {
        boost::shared_ptr<Test> p( new Test( ));
        std::cout << "q.use_count(): " << q.use_count() << std::endl; 
        boost::shared_ptr<Test> q = p->GetObject();
    }
    return 0;
}
```

运行后，程序输出：

```c
　　Test Destructor.
　　q.use_count(): 1
　　Test Destructor.
```

可以看到，对象只构造了一次，但却析构了两次。并且在增加一个指向的时候，shared_ptr的计数并没有增加。也就是说，这个时候，p和q都认为自己是Test指针的唯一拥有者，这两个shared_ptr在计数为0的时候，都会调用一次Test对象的析构函数，所以会出问题。  

那么为什么会这样呢？给一个shared_ptr传递一个this指针难道不能引起shared_ptr的计数吗？

答案是：对的，shared_ptr根本认不得你传进来的指针变量是不是之前已经传过。  

看这样的代码：

```c
int main()
{
    Test* test = new Test();
    shared_ptr<Test> p(test);
    shared_ptr<Test> q(test);
    std::cout << "p.use_count(): " << p.use_count() << std::endl;
    std::cout << "q.use_count(): " << q.use_count() << std::endl;
    return 0;
}
```

运行后，程序输出：

```c
p.use_count(): 1
q.use_count(): 1
Test Destructor.
Test Destructor.
```

也证明了刚刚的论述：shared_ptr根本认不得你传进来的指针变量是不是之前已经传过。

事实上，类对象是由外部函数通过某种机制分配的，而且一经分配立即交给 shared_ptr管理，而且以后凡是需要共享使用类对象的地方，必须使用这个 shared_ptr当作右值来构造产生或者拷贝产生（shared_ptr类中定义了赋值运算符函数和拷贝构造函数）另一个shared_ptr ，从而达到共享使用的目的。  

解释了上述现象后，现在的问题就变为了：**如何在类对象（Test）内部中获得一个指向当前对象的shared_ptr 对象？（之前证明，在类的内部直接返回this指针，或者返回return shared_ptr pTest(this);）不行，因为shared_ptr根本认不得你传过来的指针变量是不是之前已经传过，你本意传个shared_ptr pTest(this)是想这个对象use_count=2，就算this对象生命周期结束，但是也不delete，因为你异步回来还要用对象里面的东西。)**  
如果我们能够做到这一点，直接将这个shared_ptr对象返回，就不会造成新建的shared_ptr的问题了。

下面来看看enable_shared_from_this类的威力。  
enable_shared_from_this 是一个以其派生类为模板类型参数的基类模板，继承它，派生类的this指针就能变成一个 shared_ptr。  
有如下代码：

```c++
#include <iostream>
#include <memory>

class Test : public std::enable_shared_from_this<Test>        //改进1
{
public:
    //析构函数
    ~Test() { std::cout << "Test Destructor." << std::endl; }
    //获取指向当前对象的指针
    std::shared_ptr<Test> GetObject()
    {
        return shared_from_this();      //改进2
    }
};
int main(int argc, char *argv[])
{
    {
        std::shared_ptr<Test> p( new Test( ));
        std::shared_ptr<Test> q = p->GetObject();
        std::cout << "p.use_count(): " << p.use_count() << std::endl;
        std::cout << "q.use_count(): " << q.use_count() << std::endl;
    }
    return 0;
}
```

运行后，程序输出：

```c
    p.use_count(): 2
    q.use_count(): 2
    Test Destructor.
```

可以看到，问题解决了！

# 8.记录一次分支开发的过程

第一步：创建分支dev-1

    git branch dev-1

第二步：切换到分支dev-1

    git checkout dev-1

第三步：在分支上修改内容，然后

    git add .

    git commit -m "备注"

第四步:推送到dev-1分支 （这里在一些场合好像没有必要）

    git push --set-upstream origin dev-1

第五步：合并分支到master分支

  

<mark>备注</mark>  ：

    分支删除操作参考：[Git 如何删除本地分支和远程分支 - 泽哥的学习笔记 - 博客园](https://www.cnblogs.com/caozhengze/p/11268293.html)



111
