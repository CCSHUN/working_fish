/*
  time_point 如何装换成字符串
  参考
  https://zh.cppreference.com/w/cpp/io/manip/put_time
*/
#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

int main() {
  //打印到标准输出 写法1 
	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::cout  << std::put_time(std::localtime(&t), "%Y-%m-%d-%H-%M-%S") << std::endl;
 
  //写法二
  auto t1 = std::time(nullptr);
  std::cout  << std::put_time(std::localtime(&t1), "%Y-%m-%d-%H-%M-%S") << std::endl;

  std::chrono::system_clock::to_time_t(std::chrono::steady_clock::now());
	//转为字符串 写法3
	std::stringstream ss;
	ss << std::put_time(std::localtime(&t), "%F %T");
	std::string str = ss.str();
  std::cout<<str.c_str()<<std::endl;

  //稳定时间

	return 0;
}