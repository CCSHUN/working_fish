#include <iostream>
using namespace std;

template<class ... T>
void func(T ... args)
{
  cout<<"num is"<<sizeof ...(args)<<endl;
}

int main() {
  func();
  func(1, 2, 3);
  return 0;
}