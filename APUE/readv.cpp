/*
  #include<sys/uio.h>
  struct iovec {
     // Starting address (内存起始地址
       void  *iov_base;   
     // Number of bytes to transfer（这块内存长度） 
       size_t iov_len;    
  };
ssize_t readv(int fd, const struct iovec *iov, int iovcnt);
  
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
  int fd = open("readv_test.txt", O_RDONLY);
  if (-1 == fd) {
    printf("open fail: %d\n", errno);
    return -1;
  }
  char buff1[100] = {0};
  char buff2[100] = {0};
  struct iovec vec[2];
  vec[0].iov_base = buff1;
  vec[0].iov_len = 5;
  vec[1].iov_base = buff2;
  vec[1].iov_len = 5;
  
  int size = readv(fd, vec, 2);
  if (-1 == size) {
    printf("readv fail:%d\n", errno);
    return -1;
  }
  printf("read buff1 = %s\n", buff1);
  printf("read buff2 = %s\n", buff2);

  close(fd);
  return 0;
}