# c-11_learning

# 1.智能指针学习笔记

## 1.1使用智能指针管理FILE*对象，析构自动调用fclose

代码

```以下三种定义智能指针的方法皆可成功释放FILE资源
/*
  用智能指针关闭文件
*/

#include <memory>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>

//RAII写法
void OpenRAII() {
  //方法1：
  //std::unique_ptr<FILE, int (*)(FILE*)> ptr(::fopen("OpenRAII.txt", "w+"), ::fclose);
  //方法2：
  //std::unique_ptr<FILE, decltype(::fclose) *> ptr(::fopen("OpenRAII.txt", "w+"), ::fclose);
  //方法3：
  std::shared_ptr<FILE> ptr(::fopen("OpenRAII.txt", "w+"), ::fclose);
}

//泄漏资源写法
void Leak() {
  FILE* fp = ::fopen("OpenRAII.txt", "w+");
}

int main() {
   OpenRAII();
   //测试新打开文件描述符是否为3，如果是则证明文件自动释放
   int fd = open("OpenRAII.txt", O_RDONLY);
   std::cout<<fd<<std::endl;
   assert(fd == 3);
   ::close(fd);

  //泄漏资源写法
  Leak(); 
  fd = open("OpenRAII.txt", O_RDONLY);
  std::cout<<fd<<std::endl;
   assert(fd == 4);
}
```

# 2.模板学习

## 2.1可变参模板

### 2.11一个简单的可变模版参数函数

```
#include <iostream>
using namespace std;

template<class ... T>
void func(T ... args)
{
  cout<<"num is"<<sizeof ...(args)<<endl;
}

int main() {
  func();
  func(1, 2, 3);
  return 0;
}
```

输出结果如下：

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-20-09-45-03-image.png)

### 2.12 递归函数方式展开参数包

通过递归函数展开参数包，需要提供一个参数包展开的函数和一个递归终止函数，递归终止函数正是用来终止递归的，来看看下面的例子

```
#include <iostream>
using namespace std;
//递归终止函数
void print()
{
   cout << "empty" << endl;
}
//展开函数
template <class T, class ...Args>
void print(T head, Args... rest)
{
   cout << "parameter " << head << endl;
   print(rest...);
}


int main(void)
{
   print(1,2,3,4);
   return 0;
}
```

### 2.13模板参数求和

```
#include <iostream>
using namespace std;

int sum() {
   return 0;
}

template<class T, class ...args>
int sum(T head, args... rest) {
   return head + sum(rest...);
}

int main(void)
{
   cout<<sum(1,2,3,4)<<endl;
   return 0;
}
```

解释

其中T叫做模板参数包，args叫做函数参数包

省略号作用如下：

1）声明一个包含0到任意个模板参数的参数包

2）在模板定义得右边，可以将参数包展成一个个独立的参数

C++11可以使用递归函数的方式展开参数包，获得可变参数的每个值。通过递归函数展开参数包，需要提供一个参数包展开的函数和一个递归终止函数。例如：
