/*
    注意点：
        第一个，第二个参数的值， 为模板T类型，而不是atomic<T> 类型
    

*/
#include <atomic>
#include <stdio.h>
#include <iostream>


int main()
{
    std::atomic<int> a(100);
    std::atomic<int> b(101);
    std::atomic<int> c(100);
    int value1 = 100;
    int value2 = 12;
    //a.compare_exchange_weak(b, c); //错误写法，编译器不通过
    //a.compare_exchange_weak(100, 10); //错误写法，编译器不通过 ,参数应该为左值
    std::cout<<a.compare_exchange_weak(value1, value2)<<std::endl; //正确写法,输出true(1)
    
    return 0;
}