/**
 * 获取发送接收缓冲区大小
 * 函数原型：
int getsockopt(int sockfd, int level, int optname,
                      void *optval, socklen_t *optlen);
*/
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <iostream>

int main() {
  int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    printf("sockets::createNonblockingOrDie\n");
    close(sockfd);
    return -1;
  }
  
  /*struct sockaddr_in bindaddr;
  bindaddr.sin_family = AF_INET;
  bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  bindaddr.sin_port = htons(3000);
  if (bind(sockfd, (struct sockaddr *)&bindaddr, sizeof(bindaddr)) == -1)
  {
      std::cout << "bind listen socket error." << std::endl;
      close(sockfd);
      return -1;
  }*/

  int rcvbuf;
  int sndbuf;
  socklen_t len = static_cast<socklen_t>(sizeof(rcvbuf));
  int ret = ::getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (void *)&rcvbuf, &len);
  if (ret == -1) {
    printf("sockets::SO_RCVBUF get fail\n");
    close(sockfd);
    return -1;
  }
  printf("SO_RCVBUF = %d\n", rcvbuf);

  ret = ::getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (void *)&sndbuf, &len);
  if (ret == -1) {
    printf("sockets::SO_SNDBUF get fail\n");
    close(sockfd);
    return -1;
  }
  printf("SO_SNDBUF = %d\n", sndbuf);

  return 0;
}