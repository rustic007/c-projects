#include <stdio.h>

void increment(int *count) {  // Takes a pointer to int
    *x = *x + 1;          // Change the ACTUAL variable
}

int main() {
    int count = 5;
    increment(*count);     // Pass the ADDRESS
    printf("%d\n", count); // 6 ✓ Changed!
}
