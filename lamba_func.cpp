
#include<iostream>
#include <string>
#include <sstream>
#include <memory>
#include <functional>

typedef std::function<void(const std::string&)> Func;

void show(const Func& cb)
{   
    std::string data = "123";
    cb(data);
}

int main()
{
    auto func = [](const std::string data){
        std::cout<<data<<std::endl;
    };
    show(func);

    show([](const std::string data){
        std::cout<<data<<std::endl;
    }
    );
    return 0;
}