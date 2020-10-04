/*
  lambda表达式
    [] // 沒有定义任何变量。使用未定义变量会引发错误
    [x, &y] // x以传值方式传入（默认），y以引用方式传入
    [&] // 任何被使用到的外部变量都隐式地以引用方式加以引用
    [=] // 任何被使用到的外部变量都隐式地以传值方式加以引用
    [&, x] // x显式地以传值方式加以引用。其余变量以引用方式加以引用
    [=, &z] // z显式地以引用方式加以引用。其余变量以传值方式加以引用
    
    类内this传入
注意点：
  {}结尾一定要有;分号
*/
/*
  lambda函数
  https://www.jianshu.com/p/d4c0702d99d0
  https://bewaremypower.github.io/2018/12/23/lambda%E6%8D%95%E8%8E%B7this%E6%8C%87%E9%92%88/
*/
#include <thread>
#include <atomic>
#include <iostream>
#include <functional>

//lamba函数编译器实现原理
class A {
 public:
  A(int a, int b) :a_(a),b_(b){}
  int operator()(int a, int b) {
    return a + b;
  };
 private:
  int a_;
  int b_;
};

//测试this传入
class this_test {
 public:
  void test() {
    std::cout<<"test"<<std::endl;
  }
  void lambda_test() {
    //在lambda函数调用test
    /* 错误写法，编译错误，必须显示的捕获this
    auto lambda = []()-> void {
      test();
    };
    */
   //写法1
   auto lambda1 = [=]()-> void {
      test();
    };
   //写法2
   auto lambda2 = [this]()-> void {
      test();
    };
  }
};

int main(){
  //lambda函数实现原理 自定义lambda函数
  A lambda(1, 1);
  std::cout<<lambda.operator()(1, 1)<<std::endl;

  //不捕获外部变量的lambda表达式
  auto lambda1 = [](int a, int b)->int{
    return a + b;
  }; 
  std::cout<<lambda1(1, 1)<<std::endl;

  //引用外部变量1 不允许改变外部变量
  int a = 3;
  int b = 4;
  auto lambda2 = [=](void)->int {
    //a++; 不允许修改外部变量 编译出错
    return a + b;
  };
  std::cout<<lambda2()<<std::endl;

  //引用外部变量2 不允许改变外部变量 
  auto lambda3 = [=](int a, int b)->int {
    //a++; 不允许修改外部变量 编译出错
    return a + b;
  };
  //输出2 ，不是7，尽管有定义外部变量 a = 3， b = 4
  //原因局部变量屏蔽外部变量，这种做法引起歧义，程序一般不会这样用
  std::cout<<lambda3(1, 1)<<std::endl; 

  //引用外部变量4 允许改变外部变量 
  auto lambda4 = [&]()->int {
    a++;
    b++;
    return a + b;
  };
  std::cout<<lambda4()<<std::endl;  //输出9
  std::cout<<a<<":"<<b<<std::endl;  //a = 4 b =5 外部变量的值被改变
  
  //引用外部变量5 x以引用的方式传入,y、z以值传递的方式传入
  int x = 3;
  int y = 5; 
  int z = 7;
  auto lambda5 = [&x, y, z]()->int {
    x++; 
    //y++; 编译不通过，y不允许被修改
    //z++; 编译不通过，x不允许被修改
    return x + y + z;
  };
  std::cout<<lambda5()<<std::endl;
  std::cout<<x<<std::endl; //x被修改为4

  //i以引用方式传入，其他变量以值传递方式传入 ， 注意位置不能调换
  int i = 1;
  int j = 2;
  int k = 3;
  auto lambda6 = [=, &i]()->int { 
    i++; 
    return i + j + k;
  };
  std::cout<<lambda6()<<std::endl; //输出7
  std::cout<<i<<std::endl; //i被修改为2

  //q以值传递的方式传入，其他为引用
  int q = 1;
  int w = 2;
  int e = 3;
  auto lambda7 = [&, q]()->int { 
    w++;
    e++;
    return q + w + e;
  };
  std::cout<<lambda7()<<std::endl; //输出8
  std::cout<<w<<std::endl; //w被修改为3

  return 0;
}