#include <iostream>

int AdjustArray(int array[], int l, int r) {
  int i = l;
  int j = r;
  int x =array[i];

  while (i < j) {
    //x = array[i]; 基准值只需要在开始的时候赋值一次
    while (i < j && array[j] > x)
      j--;
    if (i < j) 
      array[i++] = array[j];

    while (i < j && array[i] < x)
      i++;
    if (i < j)
      array[j--] = array[i];
  }
  array[i] = x;

  return i;
}

void QuickSort(int array[], int l, int r) {
  if (l < r) {
    int i = AdjustArray(array, l, r);
    QuickSort(array, l, i - 1);
    QuickSort(array, i + 1, r);
  }
}

int main() {
  int array[10] = {72, 6, 57, 88, 60, 42, 83, 73, 48, 85};
  QuickSort(array, 0, 9);
  for (int i = 0; i < 10; i++) {
    std::cout<<array[i]<<std::endl;
  }
  return 0;
}