/*
 * ============================================================
 * EXERCISE 1: Pointer Basics
 * ============================================================
 * GOAL: Understand what a pointer IS — it's just a variable
 * that holds a memory address.
 *
 * BEFORE YOU RUN THIS: predict what each printf will output.
 * Write your predictions on paper, then run it and compare.
 * ============================================================
 */

#include <stdio.h>

int main() {
    int x = 42;
    int *p = &x;  // p holds the ADDRESS of x
    int *q = &x;

    float var1 = 23323.44f;
    float *var1pt = &var1;

    char var2 = 'd';
    char *var2pt = &var2;

    double var3 = 99999 * 999;
    double *var3pt = &var3;

    printf("Value of x:            %d\n", x);         // 42
    printf("Address of x (&x):     %p\n", (void*)&x); // 0x16f2e26f8           &x is the memory address where the integer x is stored
    printf("Value of p (= &x):     %p\n", (void*)p);  // 0x16f2e26f8
    printf("Dereferenced *p:       %d\n", *p);        // 42
    printf("Address of p itself:   %p\n", (void*)&p); // 0x16f2e26f0           &p is where the pointer p is stored

    // Modify x through the pointer
    // Before change
    *p = 99;
    printf("\nAfter *p = 99:\n");
    printf("x is now:              %d\n", x);
    printf("*p is now:             %d\n", *p);

    // After changes
    *q = 77;
    printf("\nAfter *q = 77:\n");
    printf("x is now:              %d\n", x);
    printf("*q is now:             %d\n", *q);

    // this prints the size of the pointers itself
    printf("\nThe size of var1 is %zu\n", sizeof(var1pt));
    printf("\nThe size of var2 is %zu\n", sizeof(var2pt));
    printf("\nThe size of var3 is %zu\n", sizeof(var3pt));
    
    // this pring the size of the things its pointing to
    printf("\nThe size of var1pt is %zu\n", sizeof(*var1pt));
    printf("\nThe size of var2pt is %zu\n", sizeof(*var2pt));
    printf("\nThe size of var3pt is %zu\n", sizeof(*var3pt));
    

    return 0;
}

/*
 * ============================================================
 * YOUR TASK 1a:
 * Add a second pointer q that also points to x.
 * Change x through q and verify that *p also changed.
 * Why does this happen? Write a comment explaining.
 * ============================================================
 *
 * YOUR TASK 1b:
 * Declare a float, a char, and a double.
 * Create pointers to each. Print sizeof() for each pointer.
 * Are the pointer sizes the same or different? Why?
 * (Hint: on a 64-bit system, ALL pointers are 8 bytes.
 *  On a 32-bit embedded system, they'd all be 4 bytes.)
 * ============================================================
 */
