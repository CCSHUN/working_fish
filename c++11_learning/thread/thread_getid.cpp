/*
    C++11的线程库可以使用 std::this_thread 类的 get_id 获取当前线程的 id，这是一个类静态方法。
    当然也可以使用 std::thread 的 get_id 获取指定线程的 id，这是一个类实例方法。
    但是 get_id 方法返回的是一个包装类型的 std::thread::id 对象，不可以直接强转成整型，也没有提供任何转换成整型的接口。所以，我们一般使用 std::cout 这样的输出流来输出，或者先转换为 std::ostringstream 对象，再转换成字符串类型，然后把字符串类型转换成我们需要的整型。这一点，个人觉得算是 C++11 线程库获取线程 id一个不太方便的地方

    注意点
        std::thread 的 join 方法就是用来等待线程退出的函数。当然使用这个函数时，必须保证该线程还处于运行中状态，也就是说等待的线程必须是可以 “join”的，如果需要等待的线程已经退出，此时调用join 方法，程序会产生崩溃。因此，C++ 11 的线程库同时提供了一个 joinable 方法来判断某个线程是否可以join，如果不确定您的线程是否可以”join”，可以先调用 joinable 函数判断一下是否需要等待。
*/
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <sstream>

void threadproc()
{
    while (true)
    {
        printf("I am New Thread!\n");
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

int main()
{
    std::thread t(threadproc);
    //int tid = (int)t.get_id();//错误写法，因为返回的不是整形，而是std::thread::id对象
    
    std::thread::id t_id = t.get_id();
    std::cout<<t_id<<std::endl;
    std::ostringstream ostream;
    ostream<<t_id;
    std::cout<<std::stoull(ostream.str().c_str())<<std::endl; //这里等价于pthread_self()
    
    //这里是LWP 线程ID   注意区分LWP 线程ID和 pthread_self()的区别
    int tid2 = syscall(SYS_gettid);
    std::cout<<tid2<<std::endl;

    if (t.joinable()) //最好加此判断
        t.join();

    return 0;
}