#include <stdio.h>
#include <string.h>

/* ============================================================
   PART 1: PASS BY VALUE vs PASS BY REFERENCE
   ============================================================ */

// EXAMPLE 1A: PASS BY VALUE (Wrong Way - Doesn't Work)
void increment_by_value(int n) {
    printf("  Inside function - Before: %d\n", n);
    n++;  // This only increments the COPY, not the original
    printf("  Inside function - After: %d\n", n);
}

// EXAMPLE 1B: PASS BY REFERENCE (Correct Way - Works!)
void main(int *n) {
    printf("  Inside function - Before: %d\n", *n);
    (*n)++;  // This increments the ORIGINAL variable
    printf("  Inside function - After: %d\n", *n);
}

