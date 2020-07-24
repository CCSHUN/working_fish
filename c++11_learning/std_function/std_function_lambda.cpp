/*
 std::function + lamba表达式
*/

#include <thread>
#include <atomic>
#include <iostream>
#include <functional>

using namespace std;

struct Foo {
  std::unique_ptr<int> p;
  std::function<void()> f() {
    p.reset(new int(1));
    return [this] { cout << *p << endl; };
  }
};

int main(){
  //错误的写法std::function<void>    注意()不能去掉
  std::function<void()> func = []()->void{
    std::cout<<"test"<<std::endl;
  }; 
  return 0;
}