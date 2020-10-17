/**
 * 还原SIGPIPE过程
 * zhangyl 2018.12.17
 */
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT     3000
#define SEND_DATA       "1"

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

    int optval = 1;
    int optlen = sizeof(optval); 
    //关闭NAGLE算法
    int ret = setsockopt(clientfd, IPPROTO_TCP, TCP_NODELAY, (void*)&optval, optlen);
    if (ret == -1)
    {
        std::cout<<"set TCP_NODELAY fail:"<<errno<<std::endl;
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
        int ret = send(clientfd, SEND_DATA, 1, 0); //发送一个字节
        //测试打印错误码
        /*int err;
        socklen_t len = static_cast<socklen_t>(sizeof err);
        if (::getsockopt(clientfd, SOL_SOCKET, SO_ERROR, &err, &len) < 0);
        printf("errno=%d, SO_ERROR=%d\n", errno, err);
        */
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
        sleep(30);
    }

    //5. 关闭socket
    close(clientfd);

    return 0;
}