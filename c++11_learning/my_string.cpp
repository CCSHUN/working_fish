/*
  1.字符串操作，
  2.实现std::move语义 
  3.理解move noexcot修饰在stl resize时的作用
*/
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

class MyString {
 public:
  MyString(const char* str);                   //构造函数
  MyString(const MyString& other);            //拷贝构造函数
  MyString (MyString&& other) noexcept;      //右值引用，支持std::move noexcept 一定要加，因为stl容器在resize的时候会判断是否加了noexcept，没有的话会调用拷贝构造函数
  ~MyString();
  MyString& operator=(const MyString& other); //赋值函数
  MyString& operator=(MyString&& other) noexcept; //赋值函数 支持std::move
  char& operator[](int index);   //取下标
  friend std::ostream& operator<<(std::ostream& out, const MyString& str);
 private:
  int len_;
  char* str_;
};

MyString::MyString(const char* str) {
  std::cout<<"constructor"<<std::endl;
  if (!str)
    return;
  len_ = strlen(str) + 1;
  str_ = new char[len_];
  strcpy(str_, str);
  str_[len_ - 1] = '\0';
}

MyString::MyString(const MyString& other) {
  std::cout<<"copy constructor"<<std::endl;
  len_ = other.len_;
  str_ = new char[other.len_];
  strcpy(str_, other.str_);
}

MyString::MyString(MyString&& other) noexcept : len_(other.len_), str_(other.str_){
  std::cout<<"copy move"<<std::endl;
  //转移所有权
  other.str_ = nullptr;
}

MyString& MyString::operator=(const MyString& other) {
  std::cout<<"assign"<<std::endl;
  if (this == &other)  //自赋值直接返回
    return *this;

  delete[] str_;
  len_ = other.len_;
  str_ = new char[len_];
  strcpy(str_, other.str_);

  return *this;
}

MyString& MyString::operator=(MyString&& other) noexcept {
  std::cout<<"move assign"<<std::endl;
  if (this == &other)  //自赋值直接返回
    return *this;
  
  delete[] str_;
  len_ = other.len_;
  str_ = other.str_;
  other.str_ = nullptr;

  return *this;
}

//此处会越界，std::string下标访问亦是如此
char& MyString::operator[](int index) {
   return str_[index];
}

MyString::~MyString() {
  std::cout<<"destructor"<<std::endl;
  delete[] str_; 
}

//错误写法，友元函数不属于类成员函数
/*std::ostream& MyString::operator<<(std::ostream& out, const MyString& str) {
  out<<str.str_;
  return out;
}*/

//正确写法
std::ostream& operator<<(std::ostream& out, const MyString& str) {
  if (!str.str_) //如果不在这个判断，当str.str_为空时，程序直接挂掉，且不报段错误，十分诡异
    return out;
  out<<str.str_;
  return out;
}

int main() {
  MyString str("123");     //构造函数
  MyString str_copy(str); //拷贝构造函数
  MyString str_assign("1");//构造函数
  str_assign = str;        //赋值函数
  std::cout<<str<<std::endl; //输出123
  std::cout<<str_copy<<std::endl;//输出123
  std::cout<<str_assign<<std::endl;//输出123
  
  str[1] = 'a';
  std::cout<<str<<std::endl; //输出1a3

  MyString str_1("123");
  MyString str_move(std::move(str_1));
  std::cout<<str_move<<std::endl; //输出123
  std::cout<<str_1<<std::endl; //对象所有权转移，输出为空
  
  str_1 = std::move(str_move);
  std::cout<<str_1<<std::endl; //输出123
  std::cout<<str_move<<std::endl; //对象所有权转移，输出为空

  std::vector<MyString> vc;
  for (auto i = 0; i < 3; i++)
  {
    //这里只是测试是否在vetor 在resize时是否会调用拷贝构造函数
    //（如果MyString (MyString&& other) noexcept;）没有noexcept修饰则会调用
    //测试结果说明答案是否的，如果调用了很明显第二次程序会因为段错误而终止，因为第一次执行std::move的时候 
    //str_1已经被掏空，到了第二次push_back的时候，vector会resize
    //STL 为了保证容器类型的内存安全，在大多数情况下，只会调用被标记为不会抛出异常，即被标记为 
    //“noexcept” 或 “noexcept(true)” 的移动构造函数，否则，便会调用其拷贝构造函数来作为代替
    vc.push_back(std::move(str_1)); 
  }

  return 0;
}