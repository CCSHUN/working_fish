/*
  #include<sys/uio.h>
  struct iovec {
     // Starting address (内存起始地址
       void  *iov_base;   
     // Number of bytes to transfer（这块内存长度） 
       size_t iov_len;    
  };
  汇聚写和散布读

  ssize_t readv(int fd, const struct iovec *iov, int iovcnt);
  ssize_t writev(int fd, const struct iovec *iov, int iovcnt);
  
  参考：https://blog.csdn.net/qq_28090573/article/details/89452379
*/
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  int fd = open("writev_test.txt", O_CREAT|O_RDWR|O_TRUNC);
  if (-1 == fd) {
    printf("open fail: %d\n", errno);
    return -1;
  }
  char buff1[100] = "123456\n";
  char buff2[100] = "abcdef\n";
  struct iovec vec[2];
  vec[0].iov_base = buff1;
  vec[0].iov_len = 7;
  vec[1].iov_base = buff2;
  vec[1].iov_len = 7;
  
  int size = writev(fd, vec, 2);
  if (-1 == size) {
    printf("writev fail:%d\n", errno);
    return -1;
  }

  close(fd);
  return 0;
}