/*
  shared_ptr 是c++ 管理资源的利器，也是C++ 相对于其他语言最大区别的RAII特性的最直观体现。
  shared_ptr一大好处是其创建时即捕获析构动作，即使基类析构函数不是虚函数，也能正确完成子类与析类的析构动作。
  参考文章
  https://blog.csdn.net/elija940818/article/details/102868664?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.nonecase
*/
#include <memory>
#include <iostream>

class Base {
 public:
  ~Base()
  {
      std::cout << "Base destructor" << std::endl;
  }
};

class Derived : public Base {
 public:
  ~Derived()
  {
      std::cout << "Derived destructor" << std::endl;
  }
};

void virtual_destructor() {
    {
        std::cout << "--------------------" << std::endl;
        std::shared_ptr<Base> sharedA(new Derived);
    }

    //子类无法析构
    std::cout << "--------------------" << std::endl;
    Base * a = new Derived;
    delete a;

    //正常析构
    std::cout << "--------------------" << std::endl;
    Derived* b = new Derived;
    delete b;

}

int main() {
  //输出结果显示sharedA 能释放派生类对象， a不能释放派生类对象
  virtual_destructor();

  return 0;
}
