/*
  参考https://www.cnblogs.com/goya/p/11967200.html
*/
#include <memory>
#include <iostream>

/*
  错误写法1：用指针去初始化std::shared_ptr对象
  错误写法2：类成员函数直接返回this指针给shared_ptr对象
*/
class Test {
 public:  
  Test(){}
  std::shared_ptr<Test> GetObject() {
    std::shared_ptr<Test> temp(this);
    return temp;
  }
};

int main() {
  //错误写法2成员函数返回this指针  
  std::shared_ptr<Test> ptr2 = std::make_shared<Test>();
  std::shared_ptr<Test> ptr3 = ptr2->GetObject();
  std::cout<<ptr2.use_count()<<std::endl; //输出1 可见返回this 指针并不会增加引用
  std::cout<<ptr3.use_count()<<std::endl;//输出1 

  //错误写法1，会导致 a对象被析构两次，导致程序crash
  int* a = new int(3);
  std::shared_ptr<int> ptr0(a); 
  std::shared_ptr<int> ptr1(a); 
  std::cout<<ptr0.use_count()<<std::endl; //输出1
  std::cout<<ptr1.use_count()<<std::endl;//输出1

  return 0;
}