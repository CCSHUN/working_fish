/*
  参考https://blog.csdn.net/m0_37806112/article/details/81252151?utm_medium=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.add_param_isCf&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.add_param_isCf
*/
#include <iostream>

int main() {
  const char *p0 = "hello";
  char const* p1 = "hello";
  char *const p2 = "hello";
  const char *const p3 = "hello";

  p0 = "123"; //允许
  //p0[0] = 'a'; //编译出错，p0指向的内容不允许被改变
  p1 = "456"; //允许，等价于p1
  //p1[0] = 'a'; //编译出错，p1指向的内容不允许被改变

  p2[0] = 'A';//允许
  //p2 = p0; //编译出错，p2指针不能被改变

  //p3[0] = 'A'; //编译出错，p1指向的内容不允许被改变
  //p3 = p0;     //编译出错，p2指针不能被改变
  return 0;
}