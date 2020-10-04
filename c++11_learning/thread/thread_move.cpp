/*
    thread 的move语义
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
    std::thread t1(threadproc);
    std::thread t2(std::move(t1));

    if (t1.joinable()) //最好加此判断,当t1不可joinable，调用join程序会Aborted (core dumped)
        t1.join();
    if (t2.joinable()) //最好加此判断，当t2不可joinable，调用join程序会Aborted (core dumped)
        t2.join();

    return 0;
}