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