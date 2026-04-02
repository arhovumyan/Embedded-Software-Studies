/*
 * ============================================================
 * EXERCISE 7: static, const, volatile
 * ============================================================
 * GOAL: These three keywords come up in EVERY embedded interview.
 * You need to be able to explain each one clearly.
 * ============================================================
 */

#include <stdio.h>

// ============================================================
// STATIC — three different meanings depending on context
// ============================================================

// MEANING 1: Static global variable — visible only in THIS file
// (If another .c file tries to use "extern int file_private;", it fails)
static int file_private = 42;

// MEANING 2: Static local variable — persists across function calls
void counter() {
    static int count = 0;  // initialized ONCE, not every call
    count++;
    printf("  counter() called %d times\n", count);
}

void static_local_demo() {
    printf("=== Static Local Variable ===\n");
    counter();  // 1
    counter();  // 2
    counter();  // 3
    // If count were NOT static, it would print 1 every time
}

// MEANING 3: Static function — visible only in this file
// (Can't be called from other .c files)
static void helper() {
    printf("  I'm a static (file-private) function\n");
}

/*
 * INTERVIEW ANSWER for "What does static do?":
 * "It depends on context. For a local variable, it persists across
 * function calls — it's stored in the data segment, not the stack.
 * For a global variable or function, it limits the scope to the
 * current translation unit, which helps avoid naming conflicts in
 * large projects."
 */

// ============================================================
// CONST — "I promise not to modify this"
// ============================================================

void const_demo() {
    printf("\n=== const keyword ===\n");

    const int x = 10;
    // x = 20;  // COMPILE ERROR: x is const

    // POINTER + CONST — The tricky part. Read RIGHT to LEFT.

    int a = 10, b = 20;

    // 1. Pointer to const int — can't change the VALUE through the pointer
    const int *p1 = &a;
    // *p1 = 30;    // ERROR: can't modify value
    p1 = &b;        // OK: can change what it points to
    printf("  const int *p1: points to %d\n", *p1);

    // 2. Const pointer to int — can't change WHERE it points
    int *const p2 = &a;
    *p2 = 30;       // OK: can modify the value
    // p2 = &b;     // ERROR: can't change the pointer itself
    printf("  int *const p2: points to %d\n", *p2);

    // 3. Const pointer to const int — can't change anything
    const int *const p3 = &a;
    // *p3 = 40;    // ERROR
    // p3 = &b;     // ERROR
    printf("  const int *const p3: points to %d\n", *p3);

    /*
     * MEMORY TRICK for reading declarations:
     * Read the declaration from right to left:
     *   const int *p     → "p is a pointer to int that is const"
     *                       (the int is const, pointer can move)
     *   int *const p     → "p is a const pointer to int"
     *                       (the pointer is const, value can change)
     *   const int *const p → "p is a const pointer to const int"
     *                         (nothing can change)
     */
}

// Using const in function parameters — "I won't modify your data"
void print_array(const int *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
        // arr[i] = 0;  // ERROR: arr points to const
    }
    printf("\n");
}

/*
 * INTERVIEW ANSWER for "What does const do?":
 * "const tells the compiler a value shouldn't be modified. For
 * function parameters, it's a contract that says 'I won't change
 * your data.' In embedded, const data can be placed in ROM/flash
 * instead of RAM, which is critical when you have 64KB of RAM
 * but 256KB of flash."
 */

// ============================================================
// VOLATILE — "Don't optimize reads/writes to this variable"
// ============================================================

/*
 * WHY VOLATILE EXISTS:
 * The compiler is smart. If it sees you read a variable twice
 * and nothing in between changes it, the compiler will skip
 * the second read and reuse the cached value.
 *
 * BUT: in embedded systems, a hardware register might change
 * on its own (e.g., a sensor value, a status flag set by
 * an interrupt). The compiler can't see this, so it optimizes
 * away the read — and your code breaks.
 *
 * volatile tells the compiler: "This variable can change at any
 * time without warning. ALWAYS read from actual memory."
 */

// Simulated hardware register (pretend this address is mapped to hardware)
volatile int hardware_status = 0;

// Simulated ISR (Interrupt Service Routine)
// In real embedded, this would be triggered by hardware
void simulated_isr() {
    hardware_status = 1;  // Hardware sets this flag
}

void volatile_demo() {
    printf("\n=== volatile keyword ===\n");

    // WITHOUT volatile, the compiler might optimize this loop
    // into an infinite loop because it sees hardware_status
    // never changes inside the loop body.

    // Simulate: we pretend the ISR fires and sets the flag
    printf("  Before ISR: hardware_status = %d\n", hardware_status);
    simulated_isr();
    printf("  After ISR:  hardware_status = %d\n", hardware_status);

    // In real code, you'd see something like:
    // while (!hardware_status) {
    //     // wait for interrupt
    // }
    // Without volatile, compiler optimizes this to: while(1) {}
    // because it doesn't see anything changing hardware_status

    printf("\n  WHERE YOU USE VOLATILE:\n");
    printf("  1. Hardware registers (memory-mapped I/O)\n");
    printf("  2. Variables modified by ISRs\n");
    printf("  3. Variables shared between threads\n");
    printf("  4. Memory-mapped status flags\n");
}

/*
 * INTERVIEW ANSWER for "What does volatile do?":
 * "volatile tells the compiler not to optimize reads or writes
 * to a variable because its value can change outside the normal
 * program flow — for example, a hardware register or a variable
 * modified by an interrupt service routine. Without it, the
 * compiler might cache the value in a CPU register and never
 * re-read it from memory, causing your code to miss updates."
 */

// ============================================================
// BONUS: const volatile — yes, this is a real thing
// ============================================================

// "This value can change on its own (volatile), but I'm not
// allowed to change it (const)"
// Example: a read-only hardware status register
const volatile int * const status_register = (const volatile int *)0x40000000;
// In real embedded, 0x40000000 would be a real hardware address

int main() {
    static_local_demo();
    helper();  // works because we're in the same file
    const_demo();
    volatile_demo();

    printf("\n=== const array for ROM placement ===\n");
    const int lookup_table[] = {0, 1, 4, 9, 16, 25, 36, 49, 64, 81};
    printf("  Lookup table in (potentially) flash: ");
    print_array(lookup_table, 10);

    return 0;
}

/*
 * ============================================================
 * YOUR TASK 7a:
 * Write a program with a function that has a static local counter.
 * Call it 10 times and print the count each time.
 * Then make a SECOND function with its own static counter.
 * Call both in alternation and show they're independent.
 * ============================================================
 *
 * YOUR TASK 7b:
 * Write a function:
 *   void copy_string(char *dest, const char *src)
 * The src parameter is const because you shouldn't modify the
 * source. Try modifying src inside the function — see the error.
 * ============================================================
 *
 * YOUR TASK 7c (MOST IMPORTANT):
 * Write a program that simulates an ISR scenario:
 *
 *   volatile int button_pressed = 0;
 *
 *   void button_isr() {
 *       button_pressed = 1;
 *   }
 *
 *   int main() {
 *       // Poll loop waiting for button
 *       printf("Waiting for button...\n");
 *       // Simulate ISR firing after some iterations
 *       for (int i = 0; i < 1000000; i++) {
 *           if (i == 500000) button_isr();  // simulate interrupt
 *           if (button_pressed) {
 *               printf("Button detected at iteration %d!\n", i);
 *               break;
 *           }
 *       }
 *   }
 *
 * Compile with: gcc -O2 -o test test.c
 * Run with volatile. Then REMOVE the volatile keyword and compile
 * with -O2 again. See if the behavior changes. The optimizer at
 * -O2 or -O3 may cause the non-volatile version to never see the
 * button press.
 * ============================================================
 *
 * YOUR TASK 7d:
 * Practice saying each of these out loud (seriously, do it):
 *   1. "What does static mean for a local variable?"
 *   2. "What does const mean for a pointer parameter?"
 *   3. "When do you use volatile in embedded?"
 *   4. "Can something be both const and volatile?"
 * Time yourself — each answer should be under 30 seconds
 * and crystal clear.
 * ============================================================
 */
