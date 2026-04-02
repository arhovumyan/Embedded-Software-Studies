#include <stdio.h>
#include <stdlib.h>

typedef struct { // this is how you define a custom type 
    int a;  // 4
    double b; // 8
    int c;   // 4
    long long int d;  // 8
} MYSTRUCT; // we define a custom type with this name with 4 memebers. 
            // The size of MYSTRUCT will be not 24, but 32 due to padding

#define COUNT 4 // anywhere you see COUNT, it will be represented as the number 4.

static int a[]{11, 21, 31, 41, 51, 61}; // file scope static array with 6 elements in it. 

void address1() {
    // declaring pointers
    int *arrayptr;
    int *p;

    arrayptr = a; // points to the zero'th element of the array -> a. so 11.
    p = a; // same here

    // NOTE: When we add 1 or 2 or whateer, it means we are adding 1* the amount
    // of bytes the type of pointer holds. In this case we are adding 1*4 an integer is held in 4 bytes. 
    p = p + 1; // this now moves the pointer by one point, now pointing at a[1]. so 21. 
    p++; // moves again a[2] now to 31
    p = p + 2; // moves by 2 so now its at a[4], so 51

    printf("1st int = %d, 5th int (*p) = %d\n", *arrayptr, *p);
    printf("5th int a[4] = %d\n", a[4]);
}

void address2() {

}