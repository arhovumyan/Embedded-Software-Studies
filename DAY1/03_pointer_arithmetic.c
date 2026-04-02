/*
 * ============================================================
 * EXERCISE 3: Pointer Arithmetic with Arrays
 * ============================================================
 * GOAL: Understand that arrays and pointers are closely related,
 * and pointer arithmetic scales by the size of the type.
 *
 * KEY RULE: If p is a pointer to type T, then p+1 advances
 * by sizeof(T) bytes, NOT by 1 byte.
 * ============================================================
 */

#include <stdio.h>

void demo_int_array() {
    int arr[] = {10, 20, 30, 40, 50};
    int *p = arr;  // same as: int *p = &arr[0];

    printf("=== INT array (each element = %zu bytes) ===\n", sizeof(int));

    // Three equivalent ways to access arr[2]:
    printf("arr[2]     = %d\n", arr[2]);
    printf("*(arr + 2) = %d\n", *(arr + 2));
    printf("*(p + 2)   = %d\n", *(p + 2));

    // Walk through with pointer arithmetic
    printf("\nWalking through array with p++:\n");
    for (int i = 0; i < 5; i++) {
        printf("  p = %p, *p = %d\n", (void*)p, *p);
        p++;  // moves forward by sizeof(int) = 4 bytes
    }

    // Show the address gaps
    p = arr;  // reset
    printf("\nAddress differences:\n");
    for (int i = 0; i < 5; i++) {
        printf("  &arr[%d] = %p", i, (void*)&arr[i]);
        if (i > 0) {
            printf("  (diff from [0]: %ld bytes)",
                   (long)((char*)&arr[i] - (char*)&arr[0]));
        }
        printf("\n");
    }
}

void demo_double_array() {
    double arr[] = {1.1, 2.2, 3.3, 4.4};
    double *p = arr;

    printf("\n=== DOUBLE array (each element = %zu bytes) ===\n", sizeof(double));
    printf("Address differences:\n");
    for (int i = 0; i < 4; i++) {
        printf("  &arr[%d] = %p", i, (void*)&arr[i]);
        if (i > 0) {
            printf("  (diff from [0]: %ld bytes)",
                   (long)((char*)&arr[i] - (char*)&arr[0]));
        }
        printf("\n");
    }
    // Notice: addresses jump by 8 now, not 4!
}

void demo_struct_array() {
    typedef struct {
        int x;      // 4 bytes
        double y;   // 8 bytes
        int z;      // 4 bytes
    } MyStruct;     // total with padding: 24 bytes (verify!)

    MyStruct arr[3] = {
        {1, 1.0, 10},
        {2, 2.0, 20},
        {3, 3.0, 30}
    };
    MyStruct *p = arr;

    printf("\n=== STRUCT array (each element = %zu bytes) ===\n", sizeof(MyStruct));
    printf("Address differences:\n");
    for (int i = 0; i < 3; i++) {
        printf("  &arr[%d] = %p", i, (void*)&arr[i]);
        if (i > 0) {
            printf("  (diff from [0]: %ld bytes)",
                   (long)((char*)&arr[i] - (char*)&arr[0]));
        }
        printf("\n");
    }

    // Access with arrow operator through pointer
    printf("\nAccessing struct members:\n");
    printf("  p->x = %d, p->y = %f, p->z = %d\n", p->x, p->y, p->z);
    p++;  // jumps by sizeof(MyStruct) bytes!
    printf("  After p++: p->x = %d, p->y = %f, p->z = %d\n", p->x, p->y, p->z);
}

int main() {
    demo_int_array();
    demo_double_array();
    demo_struct_array();
    return 0;
}

/*
 * ============================================================
 * YOUR TASK 3a:
 * Write a function: void reverse_array(int *arr, int size)
 * that reverses an array IN PLACE using only pointer arithmetic.
 * No arr[i] allowed — only *, +, -, and pointer comparisons.
 * Hint: use two pointers, one at the start, one at the end.
 * ============================================================
 *
 * YOUR TASK 3b:
 * Write a function: int *find_value(int *arr, int size, int target)
 * that returns a POINTER to the first occurrence of target,
 * or NULL if not found. Use pointer arithmetic to walk the array.
 * In main, check if the returned pointer is NULL before using it.
 * ============================================================
 *
 * YOUR TASK 3c:
 * Create a char array: char str[] = "HELLO";
 * Use a char* pointer to walk through it and print each character
 * and its address. Notice: addresses increment by 1 (sizeof(char)=1).
 * Compare this to int where addresses increment by 4.
 * ============================================================
 */
