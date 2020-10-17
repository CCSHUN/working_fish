/*
  使用select 监控标准输入 select设置为阻塞模式
    注:
      int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
    timeout 设置为NULL表示阻塞模式，
    time.tv_sec 设置为0立刻返回
    man手册
    Note  that  the  timeout interval will be rounded up to the system clock
       granularity, and kernel scheduling delays mean that the blocking  inter-
       val may overrun by a small amount.  If both fields of the timeval struc-
       ture are zero, then select() returns immediately.  (This is  useful  for
       polling.)   If  timeout is NULL (no timeout), select() can block indefi-
       nitely
*/
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define STANDARD_INPUT 0

int main() {
  int maxfd = STANDARD_INPUT;

  while (true) {
    //使用阻塞模式 maxfd
    fd_set readset;
    FD_ZERO(&readset); //
    FD_SET(STANDARD_INPUT, &readset);
    timeval time;
    time.tv_sec = 0;
    time.tv_usec = 0;
    int ret = select(maxfd + 1, &readset, NULL, NULL, &time);
    //int ret = select(maxfd + 1, &readset, NULL, NULL, 0); 错误写法，这样又会变成阻塞模式
    if (ret == -1) 
    {
      if (errno != EINTR) 
        break;
    } 
    else if (ret == 0) 
    {
      continue; //timeout ，事实上是阻塞模式
    } 
    else 
    {
      if (FD_ISSET(STANDARD_INPUT, &readset)) 
      {
        //假设标准输入不超过63个字符
        char buf[64] = {0};
        int length = read(STANDARD_INPUT, buf, sizeof(buf));
        if (length <= 0 && errno != EINTR) 
        {
          printf("read data error");
        }
        else 
        {
          printf("read %s\n", buf);
        }
      }
    }
    
  }
  return 0;
}