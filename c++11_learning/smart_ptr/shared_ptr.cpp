/*
  智能指针的基本操作
*/
#include <memory>
#include <vector>
#include <iostream>

typedef std::vector<int> EntryList;
typedef std::shared_ptr<EntryList> EntryListPtr;

int main() {
  EntryListPtr entry_list_ptr = std::make_shared<EntryList>(); 
  std::cout<<entry_list_ptr.use_count()<<":"<<entry_list_ptr.unique()<<std::endl;  //输出1:1
  EntryListPtr entry_list_ptr_copy = entry_list_ptr; 
  std::cout<<entry_list_ptr.use_count()<<":"<<entry_list_ptr.unique()<<std::endl; //输出2:0

  //使用智能指针访问对象 , 可见entry_list_ptr_copy和entry_list_ptr共享一个对象
  //对其中任何一个指针操作，都是对同一对象操作
  entry_list_ptr_copy->push_back(3);
  entry_list_ptr->push_back(4);
  for (auto i = 0; i < entry_list_ptr_copy->size(); i++) {
    std::cout<<entry_list_ptr_copy->at(i)<<std::endl;
  }

  //清空 可以看到entry_list_ptr 和 entry_list_ptr都被清空了
  entry_list_ptr.reset();
  std::cout<<entry_list_ptr.use_count()<<std::endl;
  std::cout<<entry_list_ptr.use_count()<<std::endl; 

  //
  EntryListPtr ptr1;
  std::cout<<ptr1.unique()<<std::endl; //未初始化输出0
  ptr1.reset(new EntryList);
  std::cout<<ptr1.unique()<<std::endl; //输出1
  // = std::make_shared<EntryList>();

  return 0;
}