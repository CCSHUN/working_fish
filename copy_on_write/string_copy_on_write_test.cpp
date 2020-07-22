/*
  下面的例子体现了copy on write机制
  注意点：
    gcc 5.x.x之后std::string 就不在使用cow机制
    所以
*/

#include <cstdio>
#include <string>
#include <iostream>

using namespace std;
int main() {
  string str1("hello_world");
  string str2 = str1;
  
  //在c++11(可以用vc6.0运行)之前可以看到两个地址一样，说明指向同一内存空间
  //但是高版本的编译器不再使用cow机制
  print("shared the address\n");
  printf("str address: %x\n", str1.c_str());
  printf("str address: %x\n", str2.c_str());

  str2[0] = 'H'; //直到写时才赋值，此时str2的地址改变
  print("copy on write\n");
  printf("str address: %x\n", str1.c_str());
  printf("str address: %x\n", str2.c_str());
  return 0;
}