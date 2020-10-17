/*
    用c98方法封装简单封装下线程
    注意点用静态函数调用pthread_create,这个需要熟练掌握的一个小技巧
        static void* ThreadFunc(void* arg); 
    这样做的原因是pthread_create第三个参数为void *(*start_routine) (void *)
    而类成员函数C++ 编译器在编译时，会将类的实例对象地址（也就是this指针）作为第一个参数传递给该方法
    所以和pthread_create第三个参数类型不匹配，而类静态方法则不会加上this指针，这就是需要在静态方法
    ThreadFunc调用pthread_create创建线程的原因

        在类内定义静态方法 或者 类外定义静态方法都是ok的，因为都不属于该类
*/
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <sstream>

class ThreadBase {
 public:
  ThreadBase() = default; 
  ~ThreadBase() = default;
  virtual int Svc() = 0;
  bool Start(); 
  bool Join();
  int GetThreadID();
 private:
  static void* ThreadFunc(void* arg); 
  pthread_t tid_;
};

void* ThreadBase::ThreadFunc(void* arg) {
    ThreadBase* p = (ThreadBase*)arg; 
    p->Svc();
    return p;
}

bool ThreadBase::Start() {
    return (-1 != ::pthread_create(&tid_, NULL, &ThreadFunc, this)) ? (true) : (false);
}

bool ThreadBase::Join() {
    return (-1 != pthread_join(tid_, NULL));
}

int ThreadBase::GetThreadID() {
    return ::pthread_self();
}

class MyThread : public ThreadBase{
 public:
  MyThread() = default;
  virtual ~MyThread() = default;
  int Svc() override;
};
int MyThread::Svc() {
    while (true) {
        printf("%ldd\n", GetThreadID());
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

int main()
{
    MyThread t;
    t.Start();
    t.Join();
    return 0;
}