/*
  参考https://www.cnblogs.com/goya/p/11967200.html
  std::enable_shared_from_this的正确用法
*/
#include <memory>
#include <iostream>

class Test : public std::enable_shared_from_this<Test> {
 public:  
  Test(){}
  std::shared_ptr<Test> GetObject() {
    return shared_from_this();
  }
};

int main() {
  //和错误写法对比看看是否会增加对象引用,
  std::shared_ptr<Test> ptr = std::make_shared<Test>();
  std::shared_ptr<Test> ptr1 = ptr->GetObject();
  std::cout<<ptr.use_count()<<std::endl; //输出2
  std::cout<<ptr1.use_count()<<std::endl;//输出2

  return 0;
}