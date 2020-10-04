/*
    atomic使用自定义数据结构
*/
#include <atomic>
#include <stdio.h>
#include <iostream>

struct MY_UDT {
    int a;
};

class MY_UN_UDT {
 public:
  MY_UN_UDT()=default;
  MY_UN_UDT(const MY_UN_UDT& other) : b(-1) {};
 private:
  int b;
};

int main()
{
    //满足以下5个条件才能符合atomic自定义数据类型
    std::cout<<std::is_trivially_copyable<MY_UDT>::value<<std::endl;
    std::cout<<std::is_copy_constructible<MY_UDT>::value<<std::endl;
    std::cout<<std::is_move_constructible<MY_UDT>::value<<std::endl;
    std::cout<<std::is_copy_assignable<MY_UDT>::value<<std::endl;
    std::cout<<std::is_move_assignable<MY_UDT>::value<<std::endl;

    std::atomic<MY_UDT> value1; //正确
    //std::atomic<MY_UN_UDT> value2; //错误，编译不通过，不满足下述条件
    std::atomic<MY_UN_UDT*> value3;  //正确，指针变量是可以的
    std::cout<<std::is_trivially_copyable<MY_UN_UDT>::value<<std::endl;
    
    return 0;
}