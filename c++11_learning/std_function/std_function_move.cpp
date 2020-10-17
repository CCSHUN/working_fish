/*
  std::function 支持std::move
*/

#include <thread>
#include <atomic>
#include <iostream>
#include <functional>

using namespace std;

int add(int a, int b){
  return a + b;
}


int main(){
  std::function<int(int, int)> func; 
  func = &add; //普通函数 等价于 func = add
  std::cout<<func(3, 4)<<std::endl;

  //没绑定参数move
  std::function<int(int, int)> func_move = std::move(func);
  std::cout<<func_move(1, 2)<<std::endl;
  //std::cout<<func(1, 2)<<std::endl; //对应权已经转移，不能这样操作，会导致运行错误

  //绑定参数move 返回值 和参数都可以不匹配
  //std::function<void()> func_bind_1 = std::bind(&add, 1, 2);  //可以编译通过
  std::function<int()> func_bind = std::bind(&add, 1, 2);
  std::cout<<func_bind()<<std::endl; //输出3

  std::function<int()> func_bind_move = std::move(func_bind);
  std::cout<<func_bind_move()<<std::endl; //输出3 --->可见绑定参数一样可以move
  return 0;
}