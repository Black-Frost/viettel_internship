#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <iostream>

/*
Lập trình chương trình với các yêu cầu sau:
- Khai báo 3 mảng tĩnh: Mảng char, short và int
- Tạo ngẫu nhiên các ký phần tử trong 3 mảng đó
- Sắp xếp 3 mảng
*/

template<class T>
void gen_random(T* arr, size_t len);

template<class T>
void sort(T* arr, size_t len);

template<class T>
void print_array(T* arr, size_t len);

using std::cout;
using std::cin;
using std::endl;

int main(int argc, char** argv) {
  srand(time(0));

  const size_t size = 3;
  // cout << "Enter the size of array: ";
  // cin >> size;

  char char_arr[size];
  short short_arr[size];
  int int_arr[size];

  gen_random(char_arr, size);
  gen_random(short_arr, size);
  gen_random(int_arr, size);

  cout << "After generation" << endl;
  print_array(char_arr, size);
  print_array(short_arr, size);
  print_array(int_arr, size);

  sort(char_arr, size);
  sort(short_arr, size);
  sort(int_arr, size);

  cout << "After sort" << endl;
  print_array(char_arr, size);
  print_array(short_arr, size);
  print_array(int_arr, size);

  return 0;
}

int randrange(int lower, int upper) {
  return (rand() % (upper - lower + 1)) + lower;
}

template<class T>
void gen_random(T* arr, size_t len) {
  for (size_t i = 0; i < len; i++) {
    arr[i] = (T)randrange(0, 100);
  }
}

template<class T>
void swap(T* x, T* y) {
  T temp = *x;
  *x = *y;
  *y = temp;
}

template<class T>
void sort(T* arr, size_t len) {
  if (len <= 1)
    return;
  if (len == 2) {
    if (arr[0] > arr[1])
      swap(&arr[0], &arr[1]);
    return;
  }

  T* left = arr;
  T* right = arr + len/2;
  size_t left_bound = len/2;
  size_t right_bound = len-len/2;

  sort(left, left_bound);
  sort(right, right_bound);

  // merge
  size_t left_cursor = 0;
  size_t right_cursor = 0;

  T* tmp = new T[len];
  size_t idx = 0;

  while (left_cursor < left_bound && right_cursor < right_bound) {
    if (left[left_cursor] > right[right_cursor]) {
      tmp[idx++] = right[right_cursor];
      right_cursor++;
    }
    else {
      tmp[idx++] = left[left_cursor];
      left_cursor++;
    }
  }

  for (;left_cursor < left_bound; left_cursor++)
    tmp[idx++] = left[left_cursor];

  for (;right_cursor < right_bound; right_cursor++)
    tmp[idx++] = right[right_cursor];

  for (idx = 0; idx < len; idx++)
    arr[idx] = tmp[idx];
}

template<class T>
void print_array(T* arr, size_t len) {
  cout << "[";
  for (size_t i = 0; i < len; i++) {
    cout << " " << (int)arr[i] << " ";
  }
  cout << "]" << endl;
}
