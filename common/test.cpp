#include <iostream>

class Test {
 public:
  static const int kMax = 3000;
  static int getMax () {
    return kMax;
  }
};

//const int Test::kMax;
int main() {

  std::cout<<Test::getMax()<<std::endl;
  return 0;
}