#include <stdarg.h>
#include <stdio.h>
 
int testparams(int count, ...)
{
 va_list args;
 va_start(args, count);
 for (int i = 0; i < count; ++i)
 {
  int arg = va_arg(args, int);
  printf("arg %d = %d", i, arg);
 }
 va_end(args);
 return 0;
}
 
int main()
{
 testparams(3, 10, 11, 12);
 return 0;
}