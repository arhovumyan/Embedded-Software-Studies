#include <stdio.h>
#include <stdlib.h>

typedef struct { // this is how you define a custom type 
    int a;  // 4
    double b; // 8
    int c;   // 4
    long long int d;  // 8
} MYSTRUCT; // we define a custom type with this name with 4 memebers. 
            // The size of MYSTRUCT will be not 24, but 32. it adds 4 to both a and c to match b and d

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
    int *p;
    // %p = hexidecimal , %d = decimal
    p = a;
    printf("Address of a is %p; value of p is %p [%d]\nvalue pointer to by p is %d\n", a, p, p, *p);

    p =  p + 1;
    printf("Address of a is %p; value of p is %p [%d]\nvalue pointer to by p is %d\n", a, p, p, *p);

    p++;
    printf("Address of a is %p; value of p is %p [%d]\nvalue pointer to by p is %d\n", a, p, p, *p);

    p = p + 2;
    printf("Address of a is %p; value of p is %p [%d]\nvalue pointer to by p is %d\n", a, p, p, *p);
}

void address3() {
    MYSTRUCT *p, *q, *arrayadd;
    void *v;

    printf("size of MYSTRUCT = %d\n", sizeof(MYSTRUCT));
    p = (MYSTRUCT*)calloc(COUNT, sizeof(MYSTRUCT)); // allocated contigous block of memory for 4 structs,
    // p is basically creating an array of 4 MYSTRUCT on the heap. 
    for (int i = 0; i < COUNT; i++) {
        p[i].a = i;
        p[i].b = 10000000000.0 + i;
        p[i].c = i * 20;
        p[i].d = 4294967296 + i;
    }

    q = p;
    arrayadd = p;
    printf("[0] values:\na is %d\nb is %f\nc is %d\nd is %lld\naddress=%p : %d\n\n", q->a, q->b, q->c, q->d, q, q);
    q = p + 1;
    printf("[1] values:\na is %d\nb is %f\nc is %d\nd is %lld\naddress=%p : %d\n\n", q->a, q->b, q->c, q->d, q, q);
    q = q + 2; // use address arithmetic to get the address of the fourth element (the array is zero based remember)
    printf("[3] values:\na is %d\nb is %f\nc is %d\nd is %lld\naddress=%p : %d\n\n", q->a, q->b, q->c, q->d, q, q);

    printf("---addresses of structs (ptr arithmetic)---\n");
    for (int i = 0; i < COUNT; i++) {
        printf("#[%d] %p (%d)\n", i, p, p); // each struct is at a 32-byte offset
        p++;
    }

    printf("---addresses of structs (array syntax)---\n");
    for (int i = 0; i < COUNT; i++) {
        printf("#[%d] %p (%d)\n", i, &arrayadd[i], &arrayadd[i]); // each struct is at a 32-byte offset
    }
    printf("Struct 3...\na is %d\nb is %f\nc is %d\nd is %lld\n", arrayadd[3].a, arrayadd[3].b, arrayadd[3].c, arrayadd[3].d);
}