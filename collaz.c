#include <stdio.h>

int steps(int start) {
  int result = start;
  int count = 0;
  while (result != 1) {
    if (result % 2 == 0) {
      result = result / 2;
    } else {
      result = result * 3 + 1;
    }

    printf("%d\n", result);
    count++;
  }
  printf("%d", count);
  return count;
}

int main() {
  steps(12);
  return 0;
}
