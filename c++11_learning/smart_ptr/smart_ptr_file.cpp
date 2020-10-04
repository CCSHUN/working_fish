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