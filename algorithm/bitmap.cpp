/*
  简单使用，没做边界处理
*/
#include <iostream>

//8*8*16 = 1024个数
template<int SIZE = 16>
class BitMap {
 public:
  BitMap(){}
  void set(int value);
  void clear(int value);
  void clear();
  bool isSet(int value);
 private:
  long int bit_[SIZE]; 
};

template<int SIZE>
void BitMap<SIZE>::set(int value) {
 bit_[value / 64] |= (1<<(value % 64)); //置1
}

template<int SIZE>
void BitMap<SIZE>::clear(int value) {
  bit_[value / 64] &= ~(1<<(value % 64)); //置零
} 

template<int SIZE>
void BitMap<SIZE>::clear() {
  bzero(bit_, sizeof(bit_));
} 

template<int SIZE>
bool BitMap<SIZE>::isSet(int value) {
  return (bit_[value / 64] & (1<<(value % 64)))? true : false;
}

int main() {
  BitMap<16> bit_map;
  bit_map.set(64);
  bit_map.set(65);
  std::cout<<bit_map.isSet(64)<<std::endl; //1
  std::cout<<bit_map.isSet(65)<<std::endl; //1
  std::cout<<bit_map.isSet(66)<<std::endl; //0

  //清除
  bit_map.clear(65);
  std::cout<<bit_map.isSet(64)<<std::endl; //1
  std::cout<<bit_map.isSet(65)<<std::endl; //0
  std::cout<<bit_map.isSet(66)<<std::endl; //0

  bit_map.clear();
  std::cout<<bit_map.isSet(64)<<std::endl; //0
  return 0;
}
