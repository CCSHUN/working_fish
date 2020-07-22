/*
  介绍了四种分配二维数组的方式
  char *p1[10]; //指针数组，数组里面存放的是char *指针
  char (*p2)[5]; //数组指针 p2指向 char (*)[5]数组 也可以称为行指针
  char **p3;
  vector<vector<char>>
*/

#include <cstdio>
#include <iostream>
#include <cstring>
#include <vector>

int main() {
  char *p1[10]; //指针数组，数组里面存放的是char *指针
  char (*p2)[5]; //数组指针 p2指向 char (*)[5]数组 也可以称为行指针
  char **p3;

  //指针数组分配堆空间
  for (auto i = 0; i < 10; i++) {
    p1[i] = new char[5]; //假设每个char* 指针分配5个字节空间
    strcpy(p1[i], "abcd");  //都赋值为 abcd
    std::cout<<p1[i]<<std::endl;
  }
  //释放堆空间
  for (auto i = 0; i < 10; i++) {
    delete[] p1[i];
    p1[i] = nullptr;
  }

  //数组指针分配空间，申请一个10行5列的二维数组
  //p2 = (char (*)[10])malloc(sizeof(char) * 10 * 5);
  p2 = new char [10][5];
  for (auto i = 0; i < 10; i++) {
    strcpy(p2[i], "1234");
    std::cout<<p2[i]<<std::endl;
  }
  
  //释放数组指针堆空间 二维数组同样用delete[] 释放内存
  delete[] p2; 
 
  //二级指针分配空间
  p3 = new char*[10]; //等价于p3 = (char **)malloc(sizeof(char *) * 10);
  for (auto i = 0; i < 10; i++) {
    p3[i] = new char[5];
    strcpy(p3[i], "5678");
    std::cout<<p3[i]<<std::endl;
  }
  //释放空间 先按行释放，再释放整体
  for (auto i = 0; i < 10; i++) {
    delete[] p3[i];
    p3[i] = nullptr;
   }
   delete[] p3;
   p3 = nullptr;

  //用vector分配二维数组 ABCD
  std::vector<std::vector<char>> p4(10);
  for (auto& it : p4) {
    //it = "ABCD";
  }
  return 0;
}