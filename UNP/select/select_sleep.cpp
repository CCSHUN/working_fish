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