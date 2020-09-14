//long int strtol (const char* str, char** endptr, int base);
/*
  下面我们来看下每个参数的意义：

str是要转换的字符
enptr是指向第一个不可转换的字符位置的指针
base的基数，表示转换成为几进制的数
*/
#include <stdio.h>
#include <stdlib.h>

int main()
{
   char str[30] = "2030300 This is test";
   char *ptr;
   long ret;

   ret = strtol(str, &ptr, 10);
   printf("数字（无符号长整数）是 %ld\n", ret);
   printf("字符串部分是 |%s|", ptr);

   return(0);
}