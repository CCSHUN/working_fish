/*
  struct hack编程技巧
    由输出结果可见struct_hack1、struct_hack2、struct_hack3效果几乎等价
    其中 struct_hack1、struct_hack2结构体占用空间最小，都为4个字节
    struct_hack3结构体占用16字节，虽然struct_hack3结构体占用空间最大，
    但是业界更多地采用了struct_hack3的做法，原因是data[0] 和data[]在
    旧标准不支持
  最后：
    1.struct_hack0被弃用，原因是申请时需要分配两次空间，先申请结构体的空间，
    然后再申请data的空间，释放的时候先释放data指向的空间，然后释放结构体的空间
    增加了操作的复杂度，此外，这样的做法造成了空间的不连续，不易于管理
    
    2.struct_hack4的做法在旧标准的std::string cow策略就是这样用的
  
  总结：
    struct_hack1、struct_hack2、struct_hack3的做法操作最简单，struct_hack4最复杂，struct_hack0不可取
    建议采用struct_hack3的做法
*/

#include <cstdio>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>

/*
  以下定义方式编译均可通过
*/
struct struct_hack0 { 
  int size;
  char* data;
};
struct struct_hack1 {
  int size;
  char data[];
};

struct struct_hack2 {
  int size;
  char data[0];
};

struct struct_hack3 {
  int size;
  char data[1];
};

struct struct_hack4 {
  int size;
  char* data[1]; 
};

/*
  假设代码在64位机器上运行
 */
int main() {
  std::cout<<sizeof(int)<<std::endl;//4
  std::cout<<sizeof(void*)<<std::endl;//8
  std::cout<<sizeof(struct_hack0)<<std::endl;//16
  std::cout<<sizeof(struct_hack1)<<std::endl;//4
  std::cout<<sizeof(struct_hack2)<<std::endl;//4
  std::cout<<sizeof(struct_hack3)<<std::endl;//8
  std::cout<<sizeof(struct_hack4)<<std::endl;//16

  int len = 11;
  char str[] = "hello_world";

  printf("--------begin hack0 analyse-------\n");
  struct_hack0* hack0 = (struct_hack0*)malloc(sizeof(struct_hack0) + len + 1);
  //strcpy(&hack0->data[0], str);//段错误
  printf("hack0 address=%p\n", hack0); //堆空间地址
  printf("hack0->data address=%p\n", hack0->data); //nil
  printf("&hack0->data address=%p\n", &hack0->data);//hack0地址 + 8
  printf("&hack0->data + 1 address=%p\n", &hack0->data + 1);//hack0地址 + 16
  printf("--------end hack0 analyse----------\n\n");

  printf("--------begin hack1 analyse-------\n");
  struct_hack1* hack1 = (struct_hack1*)malloc(sizeof(struct_hack1) + len + 1);
  memcpy(hack1->data, str, len);
  printf("hack1 address=%p\n", hack1); //堆空间地址
  printf("hack1->data address=%p\n", hack1->data); //hack + 4
  printf("hack1->data address=%p\n", hack1->data + 1);//hack + 5
  printf("hack1->data =%s\n", hack1->data);
  printf("--------end hack1 analyse----------\n\n");

  printf("--------begin hack2 analyse-------\n");
  struct_hack2* hack2 = (struct_hack2*)malloc(sizeof(struct_hack2) + len + 1);
  memcpy(hack2->data, str, len);
  printf("hack2 address=%p\n", hack2); //堆空间地址
  printf("hack2->data address=%p\n", hack2->data); //hack + 4
  printf("hack2->data address=%p\n", hack2->data + 1);//hack + 5
  printf("hack2->data =%s\n", hack2->data);
  printf("--------end hack2 analyse----------\n\n");

  printf("--------begin hack3 analyse-------\n");
  struct_hack3* hack3 = (struct_hack3*)malloc(sizeof(struct_hack3) + len + 1);
  memcpy(hack3->data, str, len);
  printf("hack3 address=%p\n", hack3); //堆空间地址
  printf("hack3->data address=%p\n", hack3->data); //hack + 4
  printf("hack3->data address=%p\n", hack3->data + 1);//hack + 5
  printf("hack3->data =%s\n", hack3->data);
  printf("--------end hack3 analyse----------\n\n");

  printf("--------begin hack4 analyse-------\n");
  struct_hack4* hack4 = (struct_hack4*)malloc(sizeof(struct_hack4) + len + 1);
  memcpy(&hack4->data, str, len);
  printf("hack4 address=%p\n", hack4); //堆空间地址
  printf("hack4->data address=%p\n", hack4->data); //hack + 8
  printf("hack4->data address=%p\n", hack4->data + 1);//hack + 16
  printf("&hack4->data=%s\n", &hack4->data[0]);//这样才能真正取出data数据
  //printf("&hack4->data=%s\n", hack4->data[0]); //段错误 自己画内存分布图体验
  printf("--------end hack4 analyse-------\n\n");

  return 0;
}