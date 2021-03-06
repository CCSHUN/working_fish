/*
  static_pointer_cast，实现shared_ptr之间的转换，基类不需要虚函数
  参考https://blog.csdn.net/adream307/article/details/81607550
*/
#include <memory>
#include <iostream>
 
class base{
 public:
    base(){std::cout << "base" << std::endl;}
    ~base(){std::cout << "~base" << std::endl;}
    void print(){std::cout << "base::print" << std::endl;}
};
 
class derived:public base{
 public:
    derived(){std::cout << "derived" << std::endl;}
    ~derived(){std::cout << "~derived" << std::endl;}
    void print(){std::cout << "derived::print" << std::endl;}
};
 
int main()
{
    std::shared_ptr<base> b_ptr = std::make_shared<derived>();
    b_ptr->print();
    auto d_ptr = std::static_pointer_cast<derived>(b_ptr);
    d_ptr->print();
    printf("b_ptr.use_count()=%d\n", b_ptr.use_count()); //输出2

    //错误做法
    auto b_test = static_cast<derived*>(b_ptr.get());
    printf("b_ptr.use_count()=%d\n", b_ptr.use_count()); //输出2
    return 0;
}