//将两个有序链表合并成一个新的有序链表并返回
#include <vector>
#include <iostream>

void merge(std::vector<int>& dst, const std::vector<int>& left, const std::vector<int>& right)
{
  int left_len = left.size();
  int right_len = right.size();
  int left_index = 0;
  int right_index = 0;

  while (left_index < left_len && right_index < right_len) {
    if (left[left_index] < right[right_index]) {
      dst.emplace_back(left[left_index++]);
    } else {
      dst.emplace_back(right[right_index++]);
    } 
  }
  if (left_index < left_len) {
    dst.insert(dst.end(), left.begin() + left_index, left.end());
  } else {
    dst.insert(dst.end(), right.begin() + right_index, right.end());
  }
}

int main() {
  std::vector<int> dst;
  std::vector<int> left = {1, 3, 4, 7};
  std::vector<int> right = {1, 4, 8, 9};
  merge(dst, left, right);

  for (auto i : dst) {
    std::cout<<i<<std::endl;
  }

  return 0;
}