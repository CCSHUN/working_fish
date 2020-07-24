/*
  使用std::bind绑定函数
    1.绑定普通函数
    2.绑定静态函数
    3.绑定类成员函数
    4.bind 占位符的使用
*/

#include <thread>
#include <atomic>
#include <iostream>
#include <functional>

using namespace std;

int add(int a, int b){
  return a + b;
}

static int sub(int a, int b) {
  static int result;
  result = a - b;
  return result;
}

class A {
 public:
  int add(int a, int b) {
    return a + b;
  }
  static int sub(int a, int b) {
    return a - b;
  }
};

int main(){
    //绑定普通函数 1 用占位符绑定法
    std::function<int(int, int)> func;
    func = std::bind(&add, std::placeholders::_1, std::placeholders::_2);
    std::cout<<func(1, 1)<<std::endl;

    //测试绑定时传参
    func = std::bind(&add, 2, 3);
    std::cout<<func(1, 1)<<std::endl; //绑定时已经传参，此时会输出5 而不是2

    //绑定普通函数 1 绑定时传参
    std::function<int(void)> func0;
    func0 = std::bind(&add, 1, 1);
    std::cout<<func0()<<std::endl;

    //绑定静态函数 绑定是传参
    std::function<int(void)> func1 = std::bind(&sub, 2, 3); 
    std::cout<<func1()<<std::endl; 

    //绑定类静态函数
    std::function<int(void)> func2 = std::bind(&A::sub, 1, 1);
    std::cout<<func2()<<std::endl; //输出0

    //绑定类普通成员函数 需要定义对象
    A object;
    std::function<int(void)> func3 = std::bind(&A::add, object, 1, 1);
    std::cout<<func2()<<std::endl; //输出2
    return 0;
}