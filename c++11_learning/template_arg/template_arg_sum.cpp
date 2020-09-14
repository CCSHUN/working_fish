#include <iostream>
using namespace std;

//递归结束函数写法1
/*
int sum() {
   return 0;
}*/

//递归结束函数写法2
template<class T>
T sum(T t) {
   return t;
}


template<class T, class ...args>
T sum(T head, args... rest) {
   return head + sum(rest...);
}

int main(void)
{
   cout<<sum(1,2,3,4)<<endl;
   return 0;
}