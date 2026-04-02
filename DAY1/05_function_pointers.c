/*
 * ============================================================
 * EXERCISE 5: Function Pointers
 * ============================================================
 * GOAL: Understand how to store a function's address in a
 * variable and call it through that variable.
 *
 * WHY THIS MATTERS IN EMBEDDED:
 * - Callback functions (e.g., "call this function when an
 *   interrupt fires")
 * - State machines (array of function pointers for each state)
 * - Plugin/driver architectures
 * - Replacing switch/case with a jump table
 * ============================================================
 */

#include <stdio.h>

// === PART 1: Basic function pointer syntax ===

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }

void basic_function_pointer() {
    printf("=== Basic Function Pointer ===\n");

    // Declare a function pointer
    // Read it as: fp is a pointer to a function that takes
    // two ints and returns an int
    int (*fp)(int, int);

    fp = add;          // point to add (no & needed, function name IS the address)
    printf("fp = add:      %d\n", fp(3, 4));      // 7

    fp = subtract;     // now point to subtract
    printf("fp = subtract: %d\n", fp(3, 4));      // -1

    fp = multiply;     // now point to multiply
    printf("fp = multiply: %d\n", fp(3, 4));      // 12
}

// === PART 2: Callback pattern ===

// This function takes a function pointer as an argument
void apply_to_array(int *arr, int size, int (*operation)(int, int), int operand) {
    for (int i = 0; i < size; i++) {
        arr[i] = operation(arr[i], operand);
    }
}

void callback_demo() {
    printf("\n=== Callback Pattern ===\n");

    int data[] = {1, 2, 3, 4, 5};

    printf("Original: ");
    for (int i = 0; i < 5; i++) printf("%d ", data[i]);
    printf("\n");

    apply_to_array(data, 5, add, 10);
    printf("After add(10): ");
    for (int i = 0; i < 5; i++) printf("%d ", data[i]);
    printf("\n");

    apply_to_array(data, 5, multiply, 2);
    printf("After multiply(2): ");
    for (int i = 0; i < 5; i++) printf("%d ", data[i]);
    printf("\n");
}

// === PART 3: Array of function pointers (jump table) ===

void state_idle()    { printf("  STATE: Idle - waiting for input\n"); }
void state_running() { printf("  STATE: Running - motor on\n"); }
void state_error()   { printf("  STATE: Error - shutting down\n"); }
void state_reset()   { printf("  STATE: Reset - reinitializing\n"); }

typedef void (*state_handler)(void);

void jump_table_demo() {
    printf("\n=== Jump Table (Array of Function Pointers) ===\n");

    // Instead of a switch/case, use an array
    state_handler states[] = {state_idle, state_running, state_error, state_reset};
    int num_states = sizeof(states) / sizeof(states[0]);

    // Simulate a state machine
    int state_sequence[] = {0, 1, 1, 2, 3, 0};
    int seq_len = sizeof(state_sequence) / sizeof(state_sequence[0]);

    for (int i = 0; i < seq_len; i++) {
        int current = state_sequence[i];
        printf("Tick %d -> ", i);
        states[current]();  // call the function pointer!
    }
}

// === PART 4: Typedef for cleaner syntax ===

// Without typedef:  int (*fp)(int, int) = add;
// With typedef:
typedef int (*math_func)(int, int);
// Now math_func is a type — much cleaner

void typedef_demo() {
    printf("\n=== Typedef for Function Pointers ===\n");
    math_func operations[] = {add, subtract, multiply};
    char *names[] = {"add", "subtract", "multiply"};

    for (int i = 0; i < 3; i++) {
        printf("  %s(10, 3) = %d\n", names[i], operations[i](10, 3));
    }
}

int main() {
    basic_function_pointer();
    callback_demo();
    jump_table_demo();
    typedef_demo();
    return 0;
}

/*
 * ============================================================
 * YOUR TASK 5a:
 * Write a function: void sort_array(int *arr, int size, int (*compare)(int, int))
 * Implement bubble sort, but use the compare function pointer to decide
 * ordering. Then write two comparison functions:
 *   int ascending(int a, int b)  — returns 1 if a > b
 *   int descending(int a, int b) — returns 1 if a < b
 * Sort the same array both ways by passing different function pointers.
 * ============================================================
 *
 * YOUR TASK 5b (EMBEDDED CLASSIC):
 * Simulate a simple interrupt callback system:
 *   - Create a typedef for a callback: typedef void (*isr_callback)(void);
 *   - Create an array of 4 callbacks (initially all NULL)
 *   - Write register_callback(int slot, isr_callback cb)
 *   - Write trigger_interrupt(int slot) that calls the callback
 *     if it's not NULL
 *   - Register some functions and trigger them
 *
 * This is basically how real embedded interrupt handlers work.
 * ============================================================
 *
 * INTERVIEW TIP: "What's a function pointer used for in embedded?"
 * Answer: "Callbacks for interrupts, state machine jump tables,
 * and hardware abstraction layers where you register handler
 * functions for different peripherals."
 * ============================================================
 */
