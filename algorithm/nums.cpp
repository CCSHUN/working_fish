//给定一个包含n个整数的数组nums 和一个目标值target. 找出nums 中的三个整数，使得它们的和与target最接近。 并返回这三个数的和。 假定每组输入值存在唯一答案。
//1, 7, 9 ,13, 15
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

int find_sum(std::vector<int>& src, int target) {
  assert(src.size() > 0);

  std::vector<int> help;
  int len = src.size();
  for (int i = 0; i < len; i++) {
    int sums = src[i];
    for (int j = i + 1; j < len; j++) {
      sums += src[j];
      for (int k = j + 1; k < len; k++){
        sums += src[k];
        help.emplace_back(sums);
        sums -= src[k];
      }
      sums -= src[j];
    }
  }

  //求和target最接近值
  int result = 0;
  int temp = abs(help[0] - target); //差值
  for (auto it : help) {
    int sub = abs(it - target);
    if (sub < temp) {
      temp = sub;
      result = it;
    } 
  }
  return result;
}

int main() {
  int target = 32;
  std::vector<int> src= {1, 7, 9, 10};
  std::cout<<find_sum(src, target)<<std::endl;
  return 0;
}