/*
    错误版本1：
        func()
            std::thread 在使用上容易犯一个错误，即在 std::thread 对象在线程函数运行期间必须是有效的
            下面代码会返回执行会产生Aborted (core dumped)
        原因：
            func函数，thread t对象被析构，违反了std::thread 对象在线程函数运行期间必须是有效的原则
    错误版本2：
        func1()
            改用detach分离线程，这种方法虽然不再运行报错，但是仍然是不可取的方法，
        原因：
            我们的代码应该尽量保证线程对象在线程运行期间有效，而不是单纯地调用 detach 方法使线程对象与线程函数的运行分离。
*/
#include <stdio.h>
#include <thread>
#include <iostream>

void threadproc()
{
    while (true)
    {
        printf("I am New Thread!\n");
    }
}

void func()
{
    std::thread t(threadproc);
}

void func1()
{
    std::thread t(threadproc);
    t.detach();
}

int main()
{
    func1();
    
    while (true)
    {
        //Sleep(1000);
        //权宜之计，让主线程不要提前退出
    }
    
    return 0;
}