#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

struct MyData 
{
    int nLen;
    char data[0];
};
//但是很多开源工程定义为以下格式
//原因C89 char data[] 和char data[0]不合法，编译无法通过
//作用等价于上述定义，除了sizeof(MyData)不等以外
//原因char data[0]不占用内存空间，char data[1]占用1个字节，
//然后整个结构体需要对齐再需要加上对齐空间
/*
struct MyData 
{
    int nLen;
    char data[1];
};*/

int main()
{
    int nLen = 10;
    char str[10] = "123456789";

    cout << "Size of MyData: " << sizeof(MyData) << endl;

    MyData *myData = (MyData*)malloc(sizeof(MyData) + nLen);
    memcpy(myData->data, str, 10);

    cout << "myData's Data is: " << myData->data << endl;

    free(myData);

    return 0;
}