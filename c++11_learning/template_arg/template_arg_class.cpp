#include <iostream>
using namespace std;

//前向声明
template<typename First, typename... Args>
struct Sum;

//基本定义
template<typename First, typename... Rest>
struct Sum<First, Rest...> : std::integral_constant<int, Sum<First>::value + Sum<Rest...>::value>
{
};

//递归终止
template<typename Last>
struct Sum<Last> : std::integral_constant<int, sizeof(Last)>
{
};
int main(void)
{
   Sum<int,double,short>::value;//值为14
   //cout<<sum(1,2,3,4)<<endl;
   return 0;
}