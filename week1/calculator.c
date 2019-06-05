#include <stdio.h>
#include <stdlib.h>

/*
  Lập trình C++ với các yêu cầu sau:
  - Chương trình có 4 chương trình con: cộng, trừ, nhân, chia
  - Sử dụng 4 chương trình con cộng, trừ, nhân, chia và hai lệnh dịch bit (left shift, right shift) trong hàm main
*/

int plus(int x, int y) {
  return x + y;
}

int minus(int x, int y) {
  return x - y;
}

int mul(int x, int y) {
  return x * y;
}

int division(int x, int y) {
  if (y == 0) {
    return 0;
  }
  else {
    return x / y;
  }
}

int shiftLeft(int x, int y) {
  return x << y;
}

int shiftRight(int x, int y) {
  return x >> y;
}

int main(int argc, char** argv) {
  int x = 0;
  int y = 0;
  char op = 0;
  int result = 0;

  int s = 0;
  while (s != 3) {
    printf("NUM OP NUM\n");
#if defined(_WIN32) || defined(WIN32)
    s = scanf_s("%d %c %d", &x, &op, 1, &y);
#else
    s = scanf("%d %c %d", &x, &op, &y);
#endif
  }

  if (op == '+') {
    result = plus(x, y);
  }
  else if (op == '-') {
    result = minus(x, y);
  }
  else if (op == '*') {
    result = mul(x, y);
  }
  else if (op == '/') {
    result = division(x, y);
  }
  else if (op == '<') {
    result = shiftLeft(x, y);
  }
  else if (op == '>') {
    result = shiftRight(x, y);
  }

  printf("%d %c %d = %d\n", x, op, y, result);
  return 0;
}
