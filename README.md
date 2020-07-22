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

```
