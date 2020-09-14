/*
  https://www.cnblogs.com/ailumiyana/p/9942989.html
*/
#include <sys/timerfd.h>
#include <sys/select.h>
#include <sys/epoll.h>
/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <iostream>


int main()
{

  int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK |TFD_CLOEXEC);

  struct itimerspec howlong;
  bzero(&howlong, sizeof howlong);
  howlong.it_value.tv_sec = 3;
  //timerfd_settime(timerfd, 0, &howlong, NULL);
  timerfd_settime(timerfd, 0, &howlong, &howlong);

      //创建epollfd
    int epollfd = epoll_create(1);
    if (epollfd == -1)
    {
        std::cout << "create epollfd error." << std::endl;
        close(timerfd);
        return -1;
    }

    epoll_event timer_fd_event;
    timer_fd_event.data.fd = timerfd;
    timer_fd_event.events = EPOLLIN;

    //将监听sokcet绑定到epollfd上去
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, timerfd, &timer_fd_event) == -1)
    {
        std::cout << "epoll_ctl error" << std::endl;
        close(timerfd);
        return -1;
    }

  while(1)
  {
    epoll_event epoll_events[1024];
    int n = epoll_wait(epollfd, epoll_events, 1024, 1000);
    if (n < 0)
    {
      //被信号中断
      if (errno == EINTR) 
          continue;

      //出错,退出
      break;
    }
    else if (n == 0)
    {
      //超时,继续
      continue;
    }
    else
    {
      for (size_t i = 0; i < n; ++i)
      {
        if (epoll_events[i].events & EPOLLIN)
        {
          std::cout << " timer happend\n";
          uint64_t howmany;
          ssize_t size = ::read(timerfd, &howmany, sizeof howmany);
          if (size != sizeof howmany)
          {
            std::cout << "TimerQueue::handleRead() reads " << size << " bytes instead of 8" << std::endl;
          }
        }
      }
    }
  }


  close(timerfd);

  return 0;
}
