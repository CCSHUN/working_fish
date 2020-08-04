/*
  认真理解智能指针转移对象所有权
  swap
*/
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>

typedef std::vector<int> EntryList;
typedef std::shared_ptr<EntryList> EntryListPtr;

int main() {
  //（1）先看vector内容的拷贝
  EntryList list1{4, 5, 6};
  EntryList list2(list1);
  auto print = [](int n){
    std::cout<<n<<std::endl;
  };
  std::for_each(list1.begin(), list1.end(), print); //输出4，5，6
  std::for_each(list2.begin(), list2.end(), print);//输出4，5，6
  
  //（2）再看vector的交换
  EntryList* list3 = new EntryList();
  list3->swap(list2); //vector内容被交换
  std::for_each(list2.begin(), list2.end(), print);
  std::cout<<std::endl;
  std::for_each(list3->begin(), list3->end(), print);
  delete list3;

  //使用智能智能替代上述操作
  EntryListPtr entry_list_ptr = std::make_shared<EntryList>(); 
  entry_list_ptr->push_back(1);
  entry_list_ptr->push_back(2);
  entry_list_ptr->push_back(3);

  //拷贝指针指针内容
  EntryListPtr entry_list_ptr_copy(new EntryList(*entry_list_ptr));
  //输出1由于是拷贝智能指针管理的对象内容，所以引用并不会增加 以下四行都输出1
  std::cout<<"entry_list_ptr ref = "<<entry_list_ptr.use_count()<<std::endl; 
  std::cout<<"entry_list_ptr unique = "<<entry_list_ptr.unique()<<std::endl; 
  std::cout<<"entry_list_ptr_copy ref = "<<entry_list_ptr_copy.use_count()<<std::endl; 
  std::cout<<"entry_list_ptr_copy unique = "<<entry_list_ptr_copy.unique()<<std::endl; 

  //交换entry_list_ptr 和 entry_list_ptr_copy管理的对象 
  entry_list_ptr.swap(entry_list_ptr_copy);
  //改变entry_list_ptr内容
  entry_list_ptr->at(0) = 0;
  std::for_each(entry_list_ptr->begin(), entry_list_ptr->end(), print); //0,2,3
  std::for_each(entry_list_ptr_copy->begin(), entry_list_ptr_copy->end(), print);//1,2,3
  return 0;
}