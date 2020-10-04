/*
  使用std::function绑定函数
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
    //std::function<static int(int, int)> func; 错误的写法 不用加static
    std::function<int(int, int)> func; 
    func = &add; //普通函数 等价于 func = add
    std::cout<<func(3, 4)<<std::endl;
    func = &sub;//静态函数 等价于 func = sub
    std::cout<<func(3, 4)<<std::endl;

    //类静态成员函数
    func = A::sub; //等价于 func = &A::sub;
    std::cout<<func(5, 10)<<std::endl;
    
    //类普通函数 需要类对象
    A object;
    //func = object.add; //错误，类普通函数需要用 
    //std::function<int(const A&, int, int)> func1 = A::add; 
    
    /*可以用std::bind实现
    std::function<int(int, int)>func2 = std::bind(&A::add, &object, std::placeholders::_1, std::placeholders::_2);
    std::cout<<func2(10, 20)<<std::endl;
    */
    return 0;
}