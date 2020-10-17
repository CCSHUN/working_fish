/*
  7字节压缩算法
*/
#include <iostream>
#include <string>

using namespace std;

void encode_7bit(uint32_t value, string& dst) {
  unsigned char c;
  while (value) {
    c = (unsigned char)(value & 0x7F); 
    if (value) {
      c |= 0x80;
    }
    dst.append(1, c);
    value >>= 7;
  }
}

void decode_7bit(const char *src, uint32_t len, uint32_t& dst) {
  dst = 0;
  int index = 0;
  int bit = 0;
  while (src[index] & 0x80) {
    uint32_t value = (uint32_t)(src[index] & 0x7F);
    value <<= bit;
    dst += value;
    bit += 7;
    index++;
  }
}

int main() {
  int value = 1234;
  string dst;
  dst.reserve(4);
  encode_7bit(value, dst);
  uint32_t value_dst;
  decode_7bit(dst.data(), dst.size(), value_dst);
  cout<<value_dst<<endl;
  return 0;
}