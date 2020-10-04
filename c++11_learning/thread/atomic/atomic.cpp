/*
    如果读者想利用 C++ 11 提供的 std::atomic 库编写跨平台的代码，在使用 std::atomic 提供的方法时建议参考官方 std::atomic 提供的接口说明来使用，而不是想当然地认为一个方法在此平台上可以运行，在另外一个平台也能有相同的行为，避免出现上面说的这种情形。
    
    要注意点：
        std::atomic_flag 只有默认构造函数，拷贝构造函数已被禁用，因此不能从其他的 std::atomic_flag 对象构造一个新的 std::atomic_flag 对象。
        不支持move写法
        //参考 https://www.cnblogs.com/haippy/p/3252056.html

        atomic使用自定义数据结构锁注意事项，见atomic_UDT.cpp
*/
#include <atomic>
#include <stdio.h>
#include <iostream>

struct TestAtomic {
    int a;
};

int main()
{
    //std::atomic<int> value = 99; 这种写法在linux编译无法通过，原因拷贝构造函数被delete
    std::atomic<int> value(99); //正确写法
    value = 100; //正确写法
    printf("%d\n", (int)value);

    //自增1，原子操作
    value++;
    ++value;
    value += 10; //默认采用  memory_order_seq_cst语义
    //等价于 value += 10; 但不同是这里返回旧值，源对象add,所以输出112
    std::cout<<value.fetch_add(10, std::memory_order_seq_cst)<<std::endl;
    printf("%d\n", (int)value); //输出122

    std::atomic<int> value_1(33);
    std::atomic<int> value_2(66);
    //value_1 = value_2; //错误的写法，编译无法通过，原因赋值函数被delete掉
    //std::atomic<int> value_3(std::move(value_2)); 错误的写法，编译无法通过，不支持move

    std::atomic<TestAtomic> value_3;
    std::atomic<TestAtomic*> value_4; //学习无锁编程

    //如果某个对象满足 lock-free 特性，在多个线程访问该对象时不会导致线程阻塞
    std::cout<<value_1.is_lock_free()<<std::endl; //测试免锁
    std::cout<<value_2.is_lock_free()<<std::endl;//测试免锁
    std::cout<<value_3.is_lock_free()<<std::endl;//测试免锁
    std::cout<<value_4.is_lock_free()<<std::endl;//测试免锁 
    //value_4.fetch_add(1);
    return 0;
}