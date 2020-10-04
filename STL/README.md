# STL学习笔记

## 1.O(1)算法时间复杂段删除vector元素

### 思路：

    保存vector元素的索引，当删除是用stl算法库，std::iter_swap函数交换需要删除元素和末尾元素的位置，然后再pop_back()删除，整个过程时间复杂度为O(1)

   扩展：

       用map存储vector元素索引，可以删除时快速定位到vector指定元素

### 代码：

```
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
```

# 2.vector使用std::sort排序

包括自定义类排序使用sort排序、vector_vector排序

```
/*
  vector排序
*/
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <functional>

using namespace std;

class Test {
 public:
  bool operator<(const Test& other) { //从小到大排序
    return key_ < other.key_;
  }
 public:
   explicit Test(int key) : key_(key) {}  
 public:
   int key_;
};

int main() {
  vector<vector<string>> intervals;
  vector<string> A = {"HELLO", "WORLD"};
  vector<string> B = {"CHEN", "SHUN"};
  vector<string> C = {"A", "B"};
  intervals.push_back(A);
  intervals.push_back(B);
  intervals.push_back(C);
  sort(intervals.begin(), intervals.end());

  auto print = [](const vector<string>& vc_s) {
    for (const auto& iter : vc_s) {
      cout<<iter<<endl;
    }
  };
  std::for_each(intervals.begin(), intervals.end(), print); 
    
  //自定义类排序
  vector<Test> vc{Test(3), Test(2), Test(4)};
  auto print_test = [](const Test& test) {
    std::cout<<test.key_<<std::endl;
  };
  sort(vc.begin(), vc.end());
  std::for_each(vc.begin(), vc.end(), print_test);

  //自定义类排序 vector_vector
  auto print_vc = [](const vector<Test>& vc) {
    for (auto const& iter : vc)
      std::cout<<iter.key_<<std::endl;
  };
  vector<vector<Test>> vc_vc;
  vc_vc.push_back(vc);
  std::for_each(vc_vc.begin(), vc_vc.end(), print_vc);

  return 0;
}
```
