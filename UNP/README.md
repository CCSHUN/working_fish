# 1.SELECT函数

参考：

http://47.97.181.98/zsxq2/articles/net_programming/4-4-select-%E5%87%BD%E6%95%B0%E7%94%A8%E6%B3%95%E5%92%8C%E5%8E%9F%E7%90%86.html

关于select函数中timeval和fd_set重新设置的问题

[关于select函数中timeval和fd_set重新设置的问题_星星之火的Blog-CSDN博客_select函数执行中更改fd_set](https://blog.csdn.net/starflame/article/details/7860091)

## 1.1使用SELECT作为延时函数

```
/*
  使用SELECT做延时函数
    测试延时3.6秒
*/
#include <sys/select.h>
#include <cstdio>
#include <chrono>
#include <iostream>

void sleep_ms(size_t ms) {
  struct timeval time;
  time.tv_sec = ms / 1000;
  time.tv_usec = (ms % 1000) * 1000;
  ::select(0, NULL, NULL, NULL, &time);
}

int main() {
  auto start = std::chrono::steady_clock::now();
  sleep_ms(3600);
  auto end = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout<<duration.count()<<std::endl;
  return 0;
}
```

运行效果

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-04-13-56-20-image.png)

## 1.2使用select函数读取标准输入

### 1.2.1 select阻塞模式

```
 /*
  使用select 监控标准输入 select设置为阻塞模式
    注:
      int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
    timeout 设置为NULL表示阻塞模式，设置为0立刻返回
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
    int ret = select(maxfd + 1, &readset, NULL, NULL, NULL);
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
```

运行结果：

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-04-17-32-45-image.png)

### 1.2.2select非阻塞模式

#### 注意点:

int ret = select(maxfd + 1, &readset, NULL, NULL, 0); 错误写法，这样又会变成阻塞模式

正确写法是将0的时间值作为参数传进去

timeval time;
 time.tv_sec = 0;
 time.tv_usec = 0;
 int ret = select(maxfd + 1, &readset, NULL, NULL, &time);    

```
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
```

输出结果和阻塞模式一致

但非阻塞模式几乎会把CPU占满

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-04-17-44-32-image.png)

# 2.netstat命令

netstat命令使用参考

http://47.97.181.98/zsxq2/articles/net_cmds/%E7%AC%AC%E5%9B%9B%E8%8A%82-netstat.html

## 2.1记一次netstat调试send、recv阻塞的过程

### 实验场景：

    服务端阻塞模式下只accept客户端连接，不recv客户端数据

    客户端阻塞模式下发送

### 先上结论：

    客户端发送缓冲区满了之后，客户端阻塞，netstat查看send-Q非0

    服务端不调用recv接受，服务端recv-Q非0

服务端代码：

```
/**
 * 验证阻塞模式下send函数的行为，server端
 * zhangyl 2018.12.17
 */
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

int main(int argc, char* argv[])
{
    //1.创建一个侦听socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        std::cout << "create listen socket error." << std::endl;
        return -1;
    }

    //2.初始化服务器地址
    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindaddr.sin_port = htons(3000);
    if (bind(listenfd, (struct sockaddr *)&bindaddr, sizeof(bindaddr)) == -1)
    {
        std::cout << "bind listen socket error." << std::endl;
        close(listenfd);
        return -1;
    }

    //3.启动侦听
    if (listen(listenfd, SOMAXCONN) == -1)
    {
        std::cout << "listen error." << std::endl;
        close(listenfd);
        return -1;
    }

    while (true)
    {
        struct sockaddr_in clientaddr;
        socklen_t clientaddrlen = sizeof(clientaddr);
        //4. 接受客户端连接
        int clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientaddrlen);
        if (clientfd != -1)
        {             
            //只接受连接，不调用recv收取任何数据
            std:: cout << "accept a client connection." << std::endl;
        }
    }

    //7.关闭侦听socket
    close(listenfd);

    return 0;
}
```

客户端代码：

```
/**
 * 验证阻塞模式下send函数的行为，client端
 * zhangyl 2018.12.17
 */
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT     3000
#define SEND_DATA       "helloworld"

int main(int argc, char* argv[])
{
    //1.创建一个socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1)
    {
        std::cout << "create client socket error." << std::endl;
        close(clientfd);
        return -1;
    }

    //2.连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serveraddr.sin_port = htons(SERVER_PORT);
    if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        std::cout << "connect socket error." << std::endl;
        return -1;
    }

    //3. 不断向服务器发送数据，或者出错退出
    int count = 0;
    while (true)
    {
        int ret = send(clientfd, SEND_DATA, strlen(SEND_DATA), 0);
        if (ret != strlen(SEND_DATA))
        {
            std::cout << "send data error." << std::endl;
            break;
        } 
        else
        {
            count ++;
            std::cout << "send data successfully, count = " << count << std::endl;
        }
    }

    //5. 关闭socket
    close(clientfd);

    return 0;
}
```

netstat -anp查看效果

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-05-09-34-19-image.png)

### recv-Q和send-Q原理介绍

recv-Q 表示网络接收队列  
表示收到的数据已经在本地接收缓冲，但是还有多少没有被进程取走，recv()  
如果接收队列Recv-Q一直处于阻塞状态，可能是遭受了拒绝服务 denial-of-service 攻击。

send-Q 表示网路发送队列  
对方没有收到的数据或者说没有Ack的,还是本地缓冲区.  
如果发送队列Send-Q不能很快的清零，可能是有应用向外发送数据包过快，或者是对方接收数据包不够快。

这两个值通常应该为0，如果不为0可能是有问题的。packets在两个队列里都不应该有堆积状态。可接受短暂的非0情况。

# 3.常用网络调试

## 还原客户端send 0字节，服务端主动关闭后，客户端send 仍然返回0的全过程

<mark>注：客户端和服务端都是非阻塞模式</mark>

### 四次挥手过程图示（客户端和服务器都可以主动发出断开连接）

参考：

[TCP三次握手和四次挥手及wireshark抓取 - elephantcc - 博客园](https://www.cnblogs.com/bencai/p/11561856.html)

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-05-10-22-12-image.png)

### 服务端代码：

```
/**
 * 验证recv函数接受0字节的行为，server端，server_recv_zero_bytes.cpp
 * zhangyl 2018.12.17
 */
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <vector>

int main(int argc, char* argv[])
{
    //1.创建一个侦听socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        std::cout << "create listen socket error." << std::endl;
        return -1;
    }

    //2.初始化服务器地址
    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindaddr.sin_port = htons(3000);
    if (bind(listenfd, (struct sockaddr *)&bindaddr, sizeof(bindaddr)) == -1)
    {
        std::cout << "bind listen socket error." << std::endl;
        close(listenfd);
        return -1;
    }

    //3.启动侦听
    if (listen(listenfd, SOMAXCONN) == -1)
    {
        std::cout << "listen error." << std::endl;
        close(listenfd);
        return -1;
    }

    int clientfd;

    struct sockaddr_in clientaddr;
    socklen_t clientaddrlen = sizeof(clientaddr);
    //4. 接受客户端连接
    clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientaddrlen);
    if (clientfd != -1)
    {             
        while (true)
        {
            char recvBuf[32] = {0};
            //5. 从客户端接受数据,客户端没有数据来的时候会在 recv 函数处阻塞
            int ret = recv(clientfd, recvBuf, 32, 0);
            if (ret > 0) 
            {
                std::cout << "recv data from client, data: " << recvBuf << std::endl;                
            } 
            else if (ret == 0)
            {
                //“假设recv返回值为0时是收到了0个字节”
                std::cout << "recv 0 byte data." << std::endl;
                continue;
            } 
            else
            {
                //出错
                std::cout << "recv data error." << std::endl;
                break;
            }
        }                
    }


    //关闭客户端socket
    close(clientfd);
    //7.关闭侦听socket
    close(listenfd);

    return 0;
}
```

### 客户端代码：

```
/**
 * 验证非阻塞模式下send函数发送0字节的行为，client端，nonblocking_client_send_zero_bytes.cpp
 * zhangyl 2018.12.17
 */
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT     3000
#define SEND_DATA       ""

int main(int argc, char* argv[])
{
    signal(SIGPIPE,SIG_IGN);
    //1.创建一个socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1)
    {
        std::cout << "create client socket error." << std::endl;
        return -1;
    }

    //2.连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serveraddr.sin_port = htons(SERVER_PORT);
    if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        std::cout << "connect socket error." << std::endl;
        close(clientfd);
        return -1;
    }

    //连接成功以后，我们再将 clientfd 设置成非阻塞模式，
    //不能在创建时就设置，这样会影响到 connect 函数的行为
    int oldSocketFlag = fcntl(clientfd, F_GETFL, 0);
    int newSocketFlag = oldSocketFlag | O_NONBLOCK;
    if (fcntl(clientfd, F_SETFL,  newSocketFlag) == -1)
    {
        close(clientfd);
        std::cout << "set socket to nonblock error." << std::endl;
        return -1;
    }

    //3. 不断向服务器发送数据，或者出错退出
    int count = 0;
    while (true)
    {
        //发送 0 字节的数据
        int ret = send(clientfd, SEND_DATA, 0, 0);
        if (ret == -1) 
        {
            //非阻塞模式下send函数由于TCP窗口太小发不出去数据，错误码是EWOULDBLOCK
            if (errno == EWOULDBLOCK)
            {
                std::cout << "send data error as TCP Window size is too small." << std::endl;
                continue;
            } 
            else if (errno == EINTR)
            {
                //如果被信号中断，我们继续重试
                std::cout << "sending data interrupted by signal." << std::endl;
                continue;
            } 
            else 
            {
                std::cout << "send data error." << std::endl;
                break;
            }
        }
        else if (ret == 0)
        {
            //发送了0字节
            std::cout << "send 0 byte data." << std::endl;
        } 
        else
        {
            count ++;
            std::cout << "send data successfully, count = " << count << std::endl;
        }

        //每三秒发一次
        sleep(3);
    }

    //5. 关闭socket
    close(clientfd);

    return 0;
}
```

### 1.启动服务端程序：

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-05-09-59-05-image.png)

### 2.启动客户端程序：

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-05-09-59-53-image.png)

### 3.此时启动tcpdump抓包并写入wirefile文件，用于观察服务端主动关闭后<mark>4次握手情况</mark>

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-05-10-01-23-image.png)

### 4.这时候<mark>ctrl+C关闭服务端程序，客户端保持运行</mark>，然后查看用wireshark打开tcpdump抓取的wirefile，如下

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-05-10-06-24-image.png)

### 5.由上图，可以看到服务端主动关闭发送FIN，然后客户端回复ACK，此时客户端进入<mark>CLOSE_WAIT</mark>状态，服务端进入<mark>FIN_WAIT2</mark>状态，用netstat可以看到这个状态

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-05-10-10-22-image.png)

### 6.此时查看客户端运行情况，可以看到客户端send函数依然返回0，但是客户端收到对端关闭没有收到调用close，所以客户端一直是CLOSE_WAIT状态

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-05-10-20-02-image.png)

### 7.ctrl+C关闭客户端程序，程序退出会调用close，然后tcpdump抓取客户端关闭动作

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-05-10-41-04-image.png)

### 8.wireshark打开抓取的<mark>closefile</mark>可以看到客户端重新发起了四次挥手动作，由于服务早已关闭，所以收到了RST

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-05-10-39-12-image.png)

### 9.结论

    这时候仍然没有结论，为什么会SEND会返回0，这时候需要结合SIGPIPE原理来分析，下面来还原下SIGPIPE信号产生的过程。

## SIGPIPE信号的产生过程

### SIGPIPE的产生原因参考：

[Linux SIGPIPE信号产生原因与解决方法_自己的学习笔记-CSDN博客_sigpipe](https://blog.csdn.net/u010821666/article/details/81841755?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase)

### 代码

服务端代码不变

客户端代码send(clientfd, SEND_DATA, 0, 0); 改为<mark>send(clientfd, SEND_DATA, 1, 0); </mark>

因为要还原SIGPIPE过程，所以在服务端断开的时候，客户端需要发送数据，才能产生SIGPIPE

### 1.启动服务器

   ![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-05-11-12-06-image.png)

### 2.用gdb启动客户端

然后ctrl+c中断客户端，

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-05-11-17-09-image.png)

### 3.ctrl+C关闭服务端，然后单步跟踪客户端的运行情况

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-05-11-18-00-image.png)

### 4.客户端单步跟踪 可以看到客户端在<mark>第二次</mark>send的时候产生SIGPIPE

服务端关闭后，客户端<mark>第一次send </mark>,可以看到ret=1,errno=0,可以看到成功发送

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-05-11-29-03-image.png)

<mark>第二次send</mark>,可以看到ret返回-1，errno=32 ,32为SIGPIPE对应的错误码

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-05-11-31-21-image.png)

注：     ![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-05-11-24-40-image.png)

## 综合思考：

### 实验效果

    SEND 1 代表客户端发送1字节代码（对应SEND 0字节实验）

    SEND 0 代表客户端发送0字节代码（对应SIGPIPE实验）

----------------------------------------------------------------------------------------------

    为什么同样服务端关闭，send 0字节和send 1字节，客户端运行效果截然不同

    --》SEND 1在服务端关闭后，第二次调用send函数会产生SIGPIPE

    --》SEND 0在服务端关闭后，send 函数依然返回0

### 群主结论：

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-05-11-56-52-image.png)

### 综合结论：

是否可以换一种结论，SEND 0客户端在服务端关闭后，底层并没有真正发送出去（<mark>这点已用tcpdump验证</mark>），所以并不会产生SIGPIPE信号，<mark>而此时 SEND 0仍然只是SEND 0的0发送字节的返回值，并不是代表对端关闭</mark>，通常我们都是用recv 0代表对端关闭。

下面我们打印下SEND 0服务端关闭后send返回0时的错误码,

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-05-11-51-53-image.png)可以看到errno = 0，说明了和服务端并没有交互，有交互必然会收到RST，是否间接验证了以下结论并不严谨 

# 4.异步connect如何判断是否connect成功

## man手册

 **EINPROGRESS**
              The socket is nonblocking and the connection cannot be
              completed immediately.  (UNIX domain sockets failed with
              **EAGAIN** instead.)  It is possible to [select(2)](https://man7.org/linux/man-pages/man2/select.2.html) or [poll(2)](https://man7.org/linux/man-pages/man2/poll.2.html) for
              completion by selecting the socket for writing.  After
              [select(2)](https://man7.org/linux/man-pages/man2/select.2.html) indicates writability, use [getsockopt(2)](https://man7.org/linux/man-pages/man2/getsockopt.2.html) to read the
              **SO_ERROR** option at level **SOL_SOCKET** to determine whether
              **connect**() completed successfully (**SO_ERROR** is zero) or
              unsuccessfully (**SO_ERROR** is one of the usual error codes
              listed here, explaining the reason for the failure).

可以清楚地看到非阻塞connect会立即返回，当错误码为EINPROGRESS时，可以用select或则poll检测写事件，当select/poll返回可写，使用getsockopt获取错误码，如果错误码==0代表连接成功，如果错误码非0代表连接失败

# 5.time_wait产生原因以及实验

time_wait产生原因参考

[time_wait状态产生的原因，危害，如何避免_huangyimo的专栏-CSDN博客](https://blog.csdn.net/huangyimo/article/details/81505558)

## 实验过程：

        服务端在客户端建立起连接后ctrl+c关闭

服务端代码：

```
/*
  记一次time_wait的实验过程，服务端在客户端建立起连接后ctrl+c关闭
*/
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

#define TRUE 1 
#define FALSE 0 

int main(int argc, char *argv[])
{ 
    //1.创建一个侦听socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        std::cout << "create listen socket error." << std::endl;
        return -1;
    }

    /* 
    optlen=sizeof(option);
    option=TRUE; 
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (void*)&option> optlen); 
    */

    //2.初始化服务器地址
    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindaddr.sin_port = htons(3000);
    if (bind(listenfd, (struct sockaddr *)&bindaddr, sizeof(bindaddr)) == -1)
    {
        std::cout << "bind listen socket error." << std::endl;
        close(listenfd);
        return -1;
    }

    //3.启动侦听
    if (listen(listenfd, SOMAXCONN) == -1)
    {
        std::cout << "listen error." << std::endl;
        close(listenfd);
        return -1;
    }

    while (true)
    {
        struct sockaddr_in clientaddr;
        socklen_t clientaddrlen = sizeof(clientaddr);
        //4. 接受客户端连接
        int clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientaddrlen);
        if (clientfd != -1)
        {             
            //只接受连接，不调用recv收取任何数据
            std:: cout << "accept a client connection." << std::endl;
        }
    }

    //7.关闭侦听socket
    close(listenfd);
 return 0; 
}    
```

客户端代码：

```
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT     3000
#define SEND_DATA       "helloworld"

int main(int argc, char* argv[])
{
    //1.创建一个socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1)
    {
        std::cout << "create client socket error." << std::endl;
        close(clientfd);
        return -1;
    }

    //2.连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serveraddr.sin_port = htons(SERVER_PORT);
    if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        std::cout << "connect socket error." << std::endl;
        return -1;
    }

    while (true)
    {
        char buf[100] = {0};
        int ret = recv(clientfd, buf, 100, 0);
        if (ret == 0)
        {
            std::cout << "peer close" << std::endl;
            sleep(1);
            break;
        } 
        else if (ret == -1)
        {
            std::cout << " recv data error" << std::endl;
        }
    }

    //5. 关闭socket
    close(clientfd);

    return 0;
}
```

实验效果：

    在服务端ctrl+c关闭后![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-06-16-44-38-image.png)

tcpdump 抓包wireshark分析

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-06-16-46-34-image.png)

1-3 行是建立连接三次握手，第四行是服务端已经关闭时close,第五行是客户端close,

    此时，服务端进入FIN_WAIT2半连接状态，客户端处于CLOSE_WAIT状态

    然后客户端recv返回0，调用close会给服务端发送FIN,进入LAST_ACK状态，等待服务端回复ACK，

    这时候处于FIN_WAIT2的服务端回复ACK，然后进入TIME_WAIT状态，

    netstat查看

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-06-17-33-54-image.png)

# 6.关于FIN_WAIT2、FIN_WAIT1状态

主动关闭一方，是进入FIN_WAIT1还是FIN_WAIT2状态呢

## 6.1FIN_WAIT1

## 实验过程：

    客户端发送缓冲尚有数据，服务端不接收数据，客户端关闭，客户端进入FIN_WAIT1状态，因为close会等待客户端缓冲区数据发送才发送FIN

服务端代码：

```
/**
 *  服务端只接受连接不接受数据，
 * 测试对端关闭时是否经过四次握手，
 */
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <sys/time.h>
#include <vector>
#include <errno.h>

//自定义代表无效fd的值
#define INVALID_FD -1

int main(int argc, char* argv[])
{
    //创建一个侦听socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        std::cout << "create listen socket error." << std::endl;
        return -1;
    }
    //初始化服务器地址
    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindaddr.sin_port = htons(3000);
    if (bind(listenfd, (struct sockaddr *)&bindaddr, sizeof(bindaddr)) == -1)
    {
        std::cout << "bind listen socket error." << std::endl;
        close(listenfd);
        return -1;
    }

    //启动侦听
    if (listen(listenfd, SOMAXCONN) == -1)
    {
        std::cout << "listen error." << std::endl;
        close(listenfd);
        return -1;
    }

    //存储客户端socket的数组
    std::vector<int> clientfds;
    int maxfd = listenfd;

    while (true) 
    {    
        fd_set readset;
        FD_ZERO(&readset);

        //将侦听socket加入到待检测的可读事件中去
        FD_SET(listenfd, &readset);

        //将客户端fd加入到待检测的可读事件中去
        int clientfdslength = clientfds.size();
        for (int i = 0; i < clientfdslength; ++i)
        {
            if (clientfds[i] != INVALID_FD)
            {
                FD_SET(clientfds[i], &readset);
            }
        }

        timeval tm;
        tm.tv_sec = 1;
        tm.tv_usec = 0;
        //暂且只检测可读事件，不检测可写和异常事件
        int ret = select(maxfd + 1, &readset, NULL, NULL, &tm);
        if (ret == -1)
        {
            //出错，退出程序。
            if (errno != EINTR)
                break;
        }
        else if (ret == 0)
        {
            //select 函数超时，下次继续
            continue;
        } 
        else
        {
            //检测到某个socket有事件
            if (FD_ISSET(listenfd, &readset))
            {
                //侦听socket的可读事件，则表明有新的连接到来
                struct sockaddr_in clientaddr;
                socklen_t clientaddrlen = sizeof(clientaddr);
                //4. 接受客户端连接
                int clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientaddrlen);
                if (clientfd == -1)                    
                {             
                    //接受连接出错，退出程序
                    break;
                }

                //只接受连接，不调用recv收取任何数据
                std:: cout << "accept a client connection, fd: " << clientfd << std::endl;
                clientfds.push_back(clientfd);
                //记录一下最新的最大fd值，以便作为下一轮循环中select的第一个参数
                if (clientfd > maxfd)
                    maxfd = clientfd;
            } 
            else 
            {

            }
        }
    }

    //关闭所有客户端socket
    int clientfdslength = clientfds.size();
    for (int i = 0; i < clientfdslength; ++i)
    {
        if (clientfds[i] != INVALID_FD)
        {
            close(clientfds[i]);
        }
    }

    //关闭侦听socket
    close(listenfd);

    return 0;
}
```

客户端代码：

```
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT     3000
#define SEND_DATA       "helloworld"

int main(int argc, char* argv[])
{
    //1.创建一个socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1)
    {
        std::cout << "create client socket error." << std::endl;
        close(clientfd);
        return -1;
    }

    //2.连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serveraddr.sin_port = htons(SERVER_PORT);
    if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        std::cout << "connect socket error." << std::endl;
        return -1;
    }

    //3. 不断向服务器发送数据，或者出错退出
    int count = 0;
    while (true)
    {
        int ret = send(clientfd, SEND_DATA, strlen(SEND_DATA), 0);
        if (ret != strlen(SEND_DATA))
        {
            std::cout << "send data error." << std::endl;
            break;
        } 
        else
        {
            count ++;
            std::cout << "send data successfully, count = " << count << std::endl;
        }
    }

    //5. 关闭socket
    close(clientfd);

    return 0;
}
```

## 实验效果

启动服务端：

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-08-10-39-08-image.png)

启动客户端：

   可见客户端不断发送数据直到缓冲区满![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-08-10-39-40-image.png)

此时打开tcpdump抓取客户端关闭时，数据包情况

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-08-10-42-03-image.png)

打开netstat，监控客户端连接状态

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-08-10-43-34-image.png)

<mark>此时ctrl+c关闭客户端</mark>

tcpdump抓包如下，可以看到FIN包并没有发送出去，而是一直尝试重传数据

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-08-10-47-16-image.png)

用wireshark查看数据包，可以看到发送端0窗口，数据一直没法送出去

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-08-11-07-00-image.png)

netstat查看客户端状态,一直处于FIN_WAIT1状态

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-08-11-09-18-image.png)

过了很长的一段时间netstat再看

FIN_WAIT1状态消失，这是因为内核定时器自动释放

## 6.2FIN_WAIT2

参考：

[23-tcp协议——TIME_WAIT状态和FIN_WAIT2状态_专注于linux，网络，渗透测试红队方向-CSDN博客_fin_wait](https://blog.csdn.net/qq_35733751/article/details/80146161)

思考：通常主动一方发起FIN报文，只要对端发送ACK确认后主动方就会处于FIN_WAIT2状态，然后等待对端发送FIN报文，如果一直没有发送FIN报文（就会一直处于CLOSE_WAIT状态，还有数据要发送，等等再关闭），那么主动一方就可能永远处于FIN_WAIT2状态

答案：不会

原因：

     为了解决主动一方永远处于FIN_WAIT2状态，操作系统设置了一个定时器，如果这个连接空闲时间超时了，那么该连接将进入CLOSED状态。

### 实验过程：

    服务端主动关闭，客户端不关闭，验证效果

服务端代码

```
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

#define TRUE 1 
#define FALSE 0 

int main(int argc, char *argv[])
{ 
    //1.创建一个侦听socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        std::cout << "create listen socket error." << std::endl;
        return -1;
    }

    /* 
    optlen=sizeof(option);
    option=TRUE; 
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (void*)&option> optlen); 
    */

    //2.初始化服务器地址
    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindaddr.sin_port = htons(3000);
    if (bind(listenfd, (struct sockaddr *)&bindaddr, sizeof(bindaddr)) == -1)
    {
        std::cout << "bind listen socket error." << std::endl;
        close(listenfd);
        return -1;
    }

    //3.启动侦听
    if (listen(listenfd, SOMAXCONN) == -1)
    {
        std::cout << "listen error." << std::endl;
        close(listenfd);
        return -1;
    }

    while (true)
    {
        struct sockaddr_in clientaddr;
        socklen_t clientaddrlen = sizeof(clientaddr);
        //4. 接受客户端连接
        int clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientaddrlen);
        if (clientfd != -1)
        {             
            //只接受连接，不调用recv收取任何数据
            std:: cout << "accept a client connection." << std::endl;
        }
    }

    //7.关闭侦听socket
    close(listenfd);
 return 0; 
}
```

客户端代码：

```
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT     3000
#define SEND_DATA       "helloworld"

int main(int argc, char* argv[])
{
    //1.创建一个socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1)
    {
        std::cout << "create client socket error." << std::endl;
        close(clientfd);
        return -1;
    }

    //2.连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serveraddr.sin_port = htons(SERVER_PORT);
    if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        std::cout << "connect socket error." << std::endl;
        return -1;
    }

    while (true)
    {
        char buf[100] = {0};
        int ret = recv(clientfd, buf, 100, 0);
        if (ret == 0)
        {
            std::cout << "peer close" << std::endl;
            sleep(1);
            //break;
        } 
        else if (ret == -1)
        {
            std::cout << " recv data error" << std::endl;
        }
    }

    //5. 关闭socket
    close(clientfd);

    return 0;
}
```

### 实验效果：

    服务端主动关闭，客户端不调用close，服务端一直处于FIN_WAIT2状态![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-06-17-57-51-image.png)

过了一段时间，客户端仍处于CLOSE_WAIT，但是服务端FIN_WAIT2已经消失，进入CLOSED状态

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-06-17-59-09-image.png)

# 7.RST和四次握手

[TCP协议RST：RST介绍、什么时候发送RST包_kanguolaikanguolaik的专栏-CSDN博客_rst ack 是什么包](https://blog.csdn.net/guowenyan001/article/details/11766929)

[几种TCP连接中出现RST的情况 - costaxu的个人页面 - OSCHINA](https://my.oschina.net/costaxu/blog/127394)

## 实验1：

    服务端接受缓冲区仍有数据，服务端先关闭，客户端会收到RST

服务端代码：

```
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <sys/time.h>
#include <vector>
#include <errno.h>

//自定义代表无效fd的值
#define INVALID_FD -1

int main(int argc, char* argv[])
{
    //创建一个侦听socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        std::cout << "create listen socket error." << std::endl;
        return -1;
    }

    //初始化服务器地址
    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindaddr.sin_port = htons(3000);
    if (bind(listenfd, (struct sockaddr *)&bindaddr, sizeof(bindaddr)) == -1)
    {
        std::cout << "bind listen socket error." << std::endl;
        close(listenfd);
        return -1;
    }

    //启动侦听
    if (listen(listenfd, SOMAXCONN) == -1)
    {
        std::cout << "listen error." << std::endl;
        close(listenfd);
        return -1;
    }

    //存储客户端socket的数组
    std::vector<int> clientfds;
    int maxfd = listenfd;

    while (true) 
    {    
        fd_set readset;
        FD_ZERO(&readset);

        //将侦听socket加入到待检测的可读事件中去
        FD_SET(listenfd, &readset);

        //将客户端fd加入到待检测的可读事件中去
        int clientfdslength = clientfds.size();
        for (int i = 0; i < clientfdslength; ++i)
        {
            if (clientfds[i] != INVALID_FD)
            {
                FD_SET(clientfds[i], &readset);
            }
        }

        timeval tm;
        tm.tv_sec = 1;
        tm.tv_usec = 0;
        //暂且只检测可读事件，不检测可写和异常事件
        int ret = select(maxfd + 1, &readset, NULL, NULL, &tm);
        if (ret == -1)
        {
            //出错，退出程序。
            if (errno != EINTR)
                break;
        }
        else if (ret == 0)
        {
            //select 函数超时，下次继续
            continue;
        } 
        else
        {
            //检测到某个socket有事件
            if (FD_ISSET(listenfd, &readset))
            {
                //侦听socket的可读事件，则表明有新的连接到来
                struct sockaddr_in clientaddr;
                socklen_t clientaddrlen = sizeof(clientaddr);
                //4. 接受客户端连接
                int clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientaddrlen);
                if (clientfd == -1)                    
                {             
                    //接受连接出错，退出程序
                    break;
                }

                //只接受连接，不调用recv收取任何数据
                std:: cout << "accept a client connection, fd: " << clientfd << std::endl;
                clientfds.push_back(clientfd);
                //记录一下最新的最大fd值，以便作为下一轮循环中select的第一个参数
                if (clientfd > maxfd)
                    maxfd = clientfd;
            } 
            else 
            {
            }
        }
    }

    //关闭所有客户端socket
    int clientfdslength = clientfds.size();
    for (int i = 0; i < clientfdslength; ++i)
    {
        if (clientfds[i] != INVALID_FD)
        {
            close(clientfds[i]);
        }
    }

    //关闭侦听socket
    close(listenfd);

    return 0;
}
```

客户端代码：

```
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT     3000
#define SEND_DATA       "helloworld"

int main(int argc, char* argv[])
{
    //1.创建一个socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1)
    {
        std::cout << "create client socket error." << std::endl;
        close(clientfd);
        return -1;
    }

    //2.连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serveraddr.sin_port = htons(SERVER_PORT);
    if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        std::cout << "connect socket error." << std::endl;
        return -1;
    }

    //3. 不断向服务器发送数据，或者出错退出
    int count = 0;
    while (true)
    {
        int ret = send(clientfd, SEND_DATA, strlen(SEND_DATA), 0);
        if (ret != strlen(SEND_DATA))
        {
            std::cout << "send data error." << std::endl;
            break;
        } 
        else
        {
            count ++;
            std::cout << "send data successfully, count = " << count << std::endl;
        }
    }

    //5. 关闭socket
    close(clientfd);

    return 0;
}
```

以上代码验证了这个实验，服务端代码只接受连接，不接受数据，客户端不断发送，直接服务端接受缓冲区满或者本端发送缓冲区满

启动服务端、启动客户端，然后ctrl+c关闭服务端，最后tcpdump抓包可以看到服务端发送RST给客户端，并没有四次挥手过程

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-08-11-57-26-image.png)

<mark>注：以上行为，服务端、客户端反过来结果一样（已验证）</mark>



## 实验2：

     同样是实验1的代码，先关闭客户端，服务端保存运行

运行结果：

tcpdump没有抓到客户端的FIN包

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-08-14-05-42-image.png)

  netstat可以看到客户端发送了FIN包，进入FIN_WAIT1状态，客户端send-Q字节数不变

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-08-14-08-45-image.png)

为什么会有这种情况，原因是客户端发送缓冲区有数据  ，<mark>那么系统会发送这些数据，然后再发送FIN</mark>



## 实验3：

    服务端发送、接受缓冲区没有数据，客户端接受缓冲区有数据，先关闭服务端，服务端会发起四次挥手

服务端代码：

```
/**
 * 验证阻塞模式下send函数的行为，server端
 * zhangyl 2018.12.17
 */
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

int main(int argc, char* argv[])
{
    //1.创建一个侦听socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        std::cout << "create listen socket error." << std::endl;
        return -1;
    }

    //2.初始化服务器地址
    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindaddr.sin_port = htons(3000);
    if (bind(listenfd, (struct sockaddr *)&bindaddr, sizeof(bindaddr)) == -1)
    {
        std::cout << "bind listen socket error." << std::endl;
        close(listenfd);
        return -1;
    }

    //3.启动侦听
    if (listen(listenfd, SOMAXCONN) == -1)
    {
        std::cout << "listen error." << std::endl;
        close(listenfd);
        return -1;
    }

    while (true)
    {
        struct sockaddr_in clientaddr;
        socklen_t clientaddrlen = sizeof(clientaddr);
        //4. 接受客户端连接
        int clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientaddrlen);
        if (clientfd != -1)
        {             
            std:: cout << "accept a client connection." << std::endl;
            //发送数据给客户端
            int ret = send(clientfd, "123", 3, 0);
            if (ret != 3)
            {
                std::cout << "send data error." << std::endl;
                break;
            } 
        }
    }

    //7.关闭侦听socket
    close(listenfd);

    return 0;
}
```

    客户端代码：

```
/**
 * 
 */
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT     3000
#define SEND_DATA       "helloworld"

int main(int argc, char* argv[])
{
    //1.创建一个socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1)
    {
        std::cout << "create client socket error." << std::endl;
        close(clientfd);
        return -1;
    }

    //2.连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serveraddr.sin_port = htons(SERVER_PORT);
    if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        std::cout << "connect socket error." << std::endl;
        return -1;
    }

    while (true)
    {
        sleep(1);
    }

    //5. 关闭socket
    close(clientfd);

    return 0;
}    
```

上面两个代码，服务端接受到客户端连接就给客户端发送123这三个字节的数据，客户端只建立起连接不接受数据

运行过程：

    1.先启动服务器，再启动客户端，然后关闭服务器

  运行效果：

    关闭服务器，tcpdump可以看到，服务端发起四次握手发送FIN,客户端回复ACK

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-08-13-32-08-image.png)

netstat 可以看到

1.服务端此时的状态为FIN_WAIT2

2.客户端为CLOSE_WAIT

3.服务端发送了FIN,客户端接受缓冲区收到FIN，recv-Q字节数从3变成4

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-08-13-33-41-image.png)





# 8.SO_LINGER选项

### 8.1SO_LINGER设置关闭发送RST给对端

   <mark> 设置方法入红框所示</mark>

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-08-15-10-41-image.png)

服务端代码：

```
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

#define TRUE 1 
#define FALSE 0 

int main(int argc, char *argv[])
{ 
    //1.创建一个侦听socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        std::cout << "create listen socket error." << std::endl;
        return -1;
    }

    /* 
    optlen=sizeof(option);
    option=TRUE; 
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (void*)&option> optlen); 
    */

    //2.初始化服务器地址
    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindaddr.sin_port = htons(3000);
    if (bind(listenfd, (struct sockaddr *)&bindaddr, sizeof(bindaddr)) == -1)
    {
        std::cout << "bind listen socket error." << std::endl;
        close(listenfd);
        return -1;
    }

    //3.启动侦听
    if (listen(listenfd, SOMAXCONN) == -1)
    {
        std::cout << "listen error." << std::endl;
        close(listenfd);
        return -1;
    }

    while (true)
    {
        struct sockaddr_in clientaddr;
        socklen_t clientaddrlen = sizeof(clientaddr);
        //4. 接受客户端连接
        int clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientaddrlen);
        if (clientfd != -1)
        {             
            //只接受连接，不调用recv收取任何数据
            std:: cout << "accept a client connection." << std::endl;
        }
    }

    //7.关闭侦听socket
    close(listenfd);
 return 0; 
}
```

客户端代码

```
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <errno.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT     3000
#define SEND_DATA       "helloworld"

int main(int argc, char* argv[])
{
    //1.创建一个socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1)
    {
        std::cout << "create client socket error." << std::endl;
        close(clientfd);
        return -1;
    }

    struct linger lin;
    lin.l_onoff = 1;
    lin.l_linger = 0;
    socklen_t size = sizeof(struct linger);
    int ret = setsockopt(clientfd, SOL_SOCKET, SO_LINGER, (void *)&lin, size);
    if (ret == -1) 
    {
        std::cout << "set SO_LINGER error." << std::endl;
        close(clientfd);
        return -1;
    }

    //2.连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serveraddr.sin_port = htons(SERVER_PORT);
    if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        std::cout << "connect socket error." << std::endl;
        return -1;
    }

    //3. 不断向服务器发送数据，或者出错退出
    int count = 0;
    while (true)
    {
        int ret = send(clientfd, SEND_DATA, strlen(SEND_DATA), 0);
        if (ret <= 0)
        {
            std::cout << "send data error" << std::endl;
        } 
        else
        {
            count ++;
            std::cout << "send data successfully, count = " << count << std::endl;
        }
    }

    //5. 关闭socket
    close(clientfd);

    return 0;
}
```

上面代码，客户端不断发送数据给服务端，服务端只接受连接，不接受数据

#### 实现过程：

    先启动服务端，再启动客户端，然后关闭客户端，上面的代码如果没设置SO_LINGER选项，<mark>在7.2实验已经验证过</mark>，客户端关闭时，会先发送缓冲区数据，然后发送FIN

    但是按上面的设置了SO_LINGER选项，客户端关闭时会直接发送RST给服务端

tcpdump抓包如下，可以看到客户端关闭时发送RST给服务端

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-08-15-23-59-image.png)

netstat查看客户端网络状态

    1.跳过了四次握手环节

    2.缓冲区被清空

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-08-15-25-35-image.png)  

# 9.shutshow优雅关闭

参考UNP 6.6节

服务端代码：

```
/**
 * select函数示例，server端, select_server.cpp
 * zhangyl 2018.12.24
 */
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <sys/time.h>
#include <vector>
#include <errno.h>

//自定义代表无效fd的值
#define INVALID_FD -1

int main(int argc, char* argv[])
{
    //创建一个侦听socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        std::cout << "create listen socket error." << std::endl;
        return -1;
    }

    //初始化服务器地址
    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindaddr.sin_port = htons(3000);
    if (bind(listenfd, (struct sockaddr *)&bindaddr, sizeof(bindaddr)) == -1)
    {
        std::cout << "bind listen socket error." << std::endl;
        close(listenfd);
        return -1;
    }

    //启动侦听
    if (listen(listenfd, SOMAXCONN) == -1)
    {
        std::cout << "listen error." << std::endl;
        close(listenfd);
        return -1;
    }

    //存储客户端socket的数组
    std::vector<int> clientfds;
    int maxfd = listenfd;

    while (true) 
    {    
        fd_set readset;
        FD_ZERO(&readset);

        //将侦听socket加入到待检测的可读事件中去
        FD_SET(listenfd, &readset);

        //将客户端fd加入到待检测的可读事件中去
        int clientfdslength = clientfds.size();
        for (int i = 0; i < clientfdslength; ++i)
        {
            if (clientfds[i] != INVALID_FD)
            {
                FD_SET(clientfds[i], &readset);
            }
        }

        timeval tm;
        tm.tv_sec = 1;
        tm.tv_usec = 0;
        //暂且只检测可读事件，不检测可写和异常事件
        int ret = select(maxfd + 1, &readset, NULL, NULL, &tm);
        if (ret == -1)
        {
            //出错，退出程序。
            if (errno != EINTR)
                break;
        }
        else if (ret == 0)
        {
            //select 函数超时，下次继续
            continue;
        } 
        else
        {
            //检测到某个socket有事件
            if (FD_ISSET(listenfd, &readset))
            {
                //侦听socket的可读事件，则表明有新的连接到来
                struct sockaddr_in clientaddr;
                socklen_t clientaddrlen = sizeof(clientaddr);
                //4. 接受客户端连接
                int clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientaddrlen);
                if (clientfd == -1)                    
                {             
                    //接受连接出错，退出程序
                    break;
                }

                //只接受连接，不调用recv收取任何数据
                std:: cout << "accept a client connection, fd: " << clientfd << std::endl;
                clientfds.push_back(clientfd);
                //记录一下最新的最大fd值，以便作为下一轮循环中select的第一个参数
                if (clientfd > maxfd)
                    maxfd = clientfd;
                
                //接受连接后设置延时
                sleep(30);
            } 
            else 
            {
                //假设对端发来的数据长度不超过63个字符
                char recvbuf[64];
                int clientfdslength = clientfds.size();
                for (int i = 0; i < clientfdslength; ++i)
                {
                    if (clientfds[i] != -1 && FD_ISSET(clientfds[i], &readset))
                    {                
                        memset(recvbuf, 0, sizeof(recvbuf));
                        //非侦听socket，则接收数据
                        int length = recv(clientfds[i], recvbuf, 64, 0);
                        if (length <= 0 && errno != EINTR)
                        {
                            //收取数据出错了
                            std::cout << "recv data error, clientfd: " << clientfds[i] << std::endl;                            
                            close(clientfds[i]);
                            //不直接删除该元素，将该位置的元素置位-1
                            clientfds[i] = INVALID_FD;
                            continue;
                        }

                        std::cout << "clientfd: " << clientfds[i] << ", recv data: " << recvbuf << std::endl;                    
                    }
                }

            }
        }
    }

    //关闭所有客户端socket
    int clientfdslength = clientfds.size();
    for (int i = 0; i < clientfdslength; ++i)
    {
        if (clientfds[i] != INVALID_FD)
        {
            close(clientfds[i]);
        }
    }

    //关闭侦听socket
    close(listenfd);

    return 0;
}
```

客户端代码：

```
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <errno.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT     3000
#define SEND_DATA       "helloworld"

int main(int argc, char* argv[])
{
    //1.创建一个socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1)
    {
        std::cout << "create client socket error." << std::endl;
        close(clientfd);
        return -1;
    }

    //2.连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    serveraddr.sin_port = htons(SERVER_PORT);
    if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        std::cout << "connect socket error." << std::endl;
        return -1;
    }

    //3. 不断向服务器发送数据，或者出错退出
    int count = 0;
    while (true)
    {
        int ret = send(clientfd, SEND_DATA, strlen(SEND_DATA), 0);
        if (count > 1000)
        {
            shutdown(clientfd, SHUT_WR); //关闭写
            std::cout<<"wait for peer close.."<<std::endl;
            char buf[100] = {0};
            ret = recv(clientfd, buf, sizeof(buf), 0);
            if (ret == 0) {
                std::cout<<"peer has closed"<<std::endl;
                break;
            }
        } 
        else
        {
            count ++;
            std::cout << "send data successfully, count = " << count << std::endl;
        }
    }

    //5. 关闭socket
    close(clientfd);

    return 0;
}
```

## 实验过程：

    服务端代码：

         客户端连接成功便延时30秒,目的为了观察客户端shutdown后数据发送情况

    客户端代码：

        发送1000次便调用shutdown,然后等待recv返回0，最后close

   实验效果：

客户send 1000次后shutdown写端，等待服务端关闭           

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-08-17-04-08-image.png)

netstat可以看到客户端进入FIN_WAIT2状态

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-08-17-05-18-image.png)

大约30秒后，服务端开始接受数据，然后close客户端连接，

tcpdump可以看到最后两次挥手，服务端发送 

![](/Users/chen/Library/Application%20Support/marktext/images/2020-08-08-17-07-58-image.png)
