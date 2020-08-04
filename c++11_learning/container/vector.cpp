#include <vector>
#include <iostream>
#include <cstdio>

int main(){
  std::vector<char> buff;
  buff.push_back('a');
  std::cout<<*buff.begin()<<std::endl;
  std::cout<<&*buff.begin()<<std::endl;
  printf("%p\n", &*buff.begin());
  printf("%p\n", buff.data()); //等价于&*buff.begin()
  return 0;
}