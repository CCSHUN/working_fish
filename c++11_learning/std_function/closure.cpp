#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>


class MyFunctor
{
public:
    MyFunctor(int temp): round(temp) {}
    int operator()(int temp) {return temp + round; }
private:
    int round;
};


void mytest()
{
    int round = 2;
    MyFunctor f(round);
    std::cout << "result: " << f(1) << std::endl; // operator()(int temp)

    return;
}

int main()
{
    mytest();
    return 0;
}