/*
 * ============================================================
 * EXERCISE 2: Swap Variables with Pointers
 * ============================================================
 * GOAL: Understand pass-by-value vs pass-by-pointer.
 * This is a CLASSIC interview question for embedded roles.
 *
 * In C, everything is pass-by-value. If you pass an int to
 * a function, the function gets a COPY. To modify the original,
 * you must pass a POINTER to it.
 * ============================================================
 */

#include <stdio.h>

// THIS DOES NOT WORK — explain why in a comment
void swap_broken(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
    // a and b are LOCAL COPIES. The originals are untouched.
}

// THIS WORKS — explain why in a comment
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int x = 10, y = 20;

    printf("Before broken swap: x=%d, y=%d\n", x, y);
    swap_broken(x, y);
    printf("After broken swap:  x=%d, y=%d\n\n", x, y);

    printf("Before real swap:   x=%d, y=%d\n", x, y);
    swap(&x, &y);  // passing ADDRESSES, not values
    printf("After real swap:    x=%d, y=%d\n", x, y);

    return 0;
}

/*
 * ============================================================
 * YOUR TASK 2a:
 * Write a function called swap_three(int *a, int *b, int *c)
 * that rotates three values: a gets b's value, b gets c's, c gets a's.
 * Test it.
 * ============================================================
 *
 * YOUR TASK 2b:
 * Write a function: void get_min_max(int *arr, int size, int *min, int *max)
 * It should find the min and max of the array and store them
 * through the pointers. This is how embedded C returns multiple
 * values from a function — through output pointers.
 * Test with: int arr[] = {34, 7, 23, 1, 89, 12};
 * ============================================================
 *
 * INTERVIEW TIP: Be ready to explain WHY you need &x when
 * calling swap. "Because we need to pass the address so the
 * function can modify the original variable, not a copy."
 * ============================================================
 */
