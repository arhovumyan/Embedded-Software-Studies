/*
 * ============================================================
 * EXERCISE 4: Double Pointers (Pointer to Pointer)
 * ============================================================
 * GOAL: Understand why you sometimes need int** instead of int*.
 *
 * RULE OF THUMB: If a function needs to CHANGE what a pointer
 * points to (not just change the value it points at), you need
 * a pointer TO that pointer — a double pointer.
 *
 * Think of it like the swap exercise:
 *   - To change an int, you pass int*
 *   - To change an int*, you pass int**
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

// THIS DOES NOT WORK — the pointer is a local copy
void allocate_broken(int *p, int size) {
    p = (int*)malloc(size * sizeof(int));
    // p is a LOCAL copy of the pointer. The original in main is unchanged.
    // Also, this malloc is now a memory leak — nobody has the address.
}

// THIS WORKS — we receive a pointer to the pointer
void allocate(int **p, int size) {
    *p = (int*)malloc(size * sizeof(int));
    // *p modifies the ORIGINAL pointer in main
}

// Visualize the levels:
void explain_levels() {
    int x = 42;
    int *p = &x;
    int **pp = &p;

    printf("=== Levels of indirection ===\n");
    printf("x     = %d\n", x);          // the value: 42
    printf("&x    = %p\n", (void*)&x);  // address of x
    printf("p     = %p\n", (void*)p);   // same as &x
    printf("*p    = %d\n", *p);         // dereference once: 42
    printf("&p    = %p\n", (void*)&p);  // address of the pointer itself
    printf("pp    = %p\n", (void*)pp);  // same as &p
    printf("*pp   = %p\n", (void*)*pp); // dereference once: get p's value (= &x)
    printf("**pp  = %d\n", **pp);       // dereference twice: get x's value: 42

    // Modify x through the double pointer
    **pp = 100;
    printf("\nAfter **pp = 100: x = %d\n", x);
}

int main() {
    explain_levels();

    printf("\n=== Allocating through double pointer ===\n");

    int *data = NULL;

    // Wrong way
    allocate_broken(data, 5);
    printf("After allocate_broken: data = %p (still NULL!)\n", (void*)data);

    // Right way
    allocate(&data, 5);
    printf("After allocate:        data = %p (now valid!)\n", (void*)data);

    // Fill and print
    for (int i = 0; i < 5; i++) {
        data[i] = (i + 1) * 10;
    }
    for (int i = 0; i < 5; i++) {
        printf("data[%d] = %d\n", i, data[i]);
    }

    free(data);
    return 0;
}

/*
 * ============================================================
 * YOUR TASK 4a:
 * Draw a memory diagram on paper for explain_levels().
 * Three boxes: x, p, pp. Draw arrows showing what points where.
 * Label each box with its address and value.
 * This is the kind of thing interviewers LOVE to ask you to
 * whiteboard.
 * ============================================================
 *
 * YOUR TASK 4b:
 * Write a function:
 *   void split_array(int *arr, int size, int **evens, int *even_count,
 *                    int **odds, int *odd_count)
 *
 * It should malloc two new arrays: one for even numbers, one for
 * odd numbers from arr. The caller (main) passes in pointers that
 * get filled. You need double pointers because you're allocating
 * inside the function.
 *
 * Test with: int arr[] = {1, 4, 7, 2, 9, 6, 3, 8};
 * Don't forget to free both arrays in main!
 * ============================================================
 *
 * INTERVIEW TIP: "When do you use a double pointer?"
 * Answer: "When a function needs to modify the pointer itself,
 * like allocating memory or changing what it points to, not just
 * modifying the data it points at."
 * ============================================================
 */
