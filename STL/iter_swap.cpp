/*
  知道索引的情况可以O(1)时间复杂度删除vector元素
*/
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

class Test {
 public:
  Test(int index) : index_(index) {}
  int index_;  
};

int main() {
  std::vector<Test> test_list = {Test(0), Test(1), Test(2),Test(3)};
  //遍历
  auto print = [](const Test& n){
    std::cout<<n.index_<<std::endl;
  };
  std::for_each(test_list.begin(), test_list.end(), print);  //0,1,2,3

  //知道索引的情况下可以O(1)时间删除元素,现在删除 2
  std::iter_swap(test_list.begin() + 2, test_list.end() - 1);
  test_list.pop_back();
  std::for_each(test_list.begin(), test_list.end(), print);
  return 0;
}