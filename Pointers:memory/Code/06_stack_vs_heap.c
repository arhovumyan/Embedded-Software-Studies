/*
 * ============================================================
 * EXERCISE 6: Stack vs Heap
 * ============================================================
 * GOAL: Understand where variables live in memory and why
 * embedded systems care deeply about this.
 *
 * MEMORY MAP (simplified):
 *
 *   HIGH ADDRESSES
 *   ┌──────────────────┐
 *   │      STACK        │  ← grows DOWN. Local variables, function
 *   │        ↓          │    call frames. Automatic cleanup.
 *   │                   │
 *   │        ↑          │
 *   │      HEAP         │  ← grows UP. malloc/calloc. YOU must free.
 *   ├──────────────────┤
 *   │   Global/Static   │  ← lives for entire program lifetime
 *   ├──────────────────┤
 *   │    Code (.text)   │  ← your compiled instructions
 *   └──────────────────┘
 *   LOW ADDRESSES
 *
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>

int global_var = 100;           // global — lives in data segment
static int static_var = 200;    // static global — same segment, but file-scoped

void show_addresses() {
    int stack_var = 300;                          // stack
    static int static_local = 400;                // static — NOT on stack!
    int *heap_var = (int*)malloc(sizeof(int));     // heap
    *heap_var = 500;

    printf("=== Where things live ===\n");
    printf("Code (function addr):    %p\n", (void*)show_addresses);
    printf("Global variable:         %p  (value: %d)\n", (void*)&global_var, global_var);
    printf("Static global:           %p  (value: %d)\n", (void*)&static_var, static_var);
    printf("Static local:            %p  (value: %d)\n", (void*)&static_local, static_local);
    printf("Stack variable:          %p  (value: %d)\n", (void*)&stack_var, stack_var);
    printf("Heap variable:           %p  (value: %d)\n", (void*)heap_var, *heap_var);

    // Notice: code < global/static < heap <<< stack (on most systems)
    free(heap_var);
}

// === DANGLING POINTER BUG — Classic embedded pitfall ===

int *dangling_pointer_bug() {
    int local = 42;
    return &local;  // WARNING: local dies when function returns!
    // The returned pointer points to garbage — undefined behavior
}

int *correct_version() {
    int *p = (int*)malloc(sizeof(int));
    *p = 42;
    return p;  // Caller is responsible for freeing this
}

// === STACK OVERFLOW DEMO ===

void recursive_bomb(int depth) {
    int big_array[1024];  // 4KB per call frame
    big_array[0] = depth;
    printf("Depth: %d, stack addr: %p\n", depth, (void*)big_array);
    if (depth < 50)
        recursive_bomb(depth + 1);
    // Watch the addresses decrease — stack grows downward
}

// === WHY EMBEDDED AVOIDS DYNAMIC ALLOCATION ===

void why_no_malloc_in_embedded() {
    printf("\n=== Why Embedded Avoids malloc ===\n");
    printf("1. FRAGMENTATION: After many malloc/free cycles, memory\n");
    printf("   gets fragmented. A 32KB heap might have 10KB free but\n");
    printf("   no single contiguous block larger than 2KB.\n\n");
    printf("2. NON-DETERMINISTIC: malloc takes unpredictable time.\n");
    printf("   In a hard real-time system, you need guaranteed timing.\n\n");
    printf("3. FAILURE HANDLING: What if malloc returns NULL on a\n");
    printf("   system with no display and no OS? You can't show an\n");
    printf("   error dialog. The drone just crashes.\n\n");
    printf("4. MEMORY LEAKS: No garbage collector. One missed free()\n");
    printf("   and your device runs out of memory after 3 days.\n\n");
    printf("INSTEAD, embedded uses:\n");
    printf("  - Static allocation (fixed arrays, known at compile time)\n");
    printf("  - Memory pools (pre-allocated fixed-size blocks)\n");
    printf("  - Stack allocation (local variables, automatic cleanup)\n");
}

int main() {
    show_addresses();

    printf("\n=== Stack growth demo (watch addresses decrease) ===\n");
    recursive_bomb(0);

    why_no_malloc_in_embedded();

    return 0;
}

/*
 * ============================================================
 * YOUR TASK 6a:
 * Intentionally create a memory leak. Allocate in a loop 100
 * times without freeing. Print the addresses. (This is to see
 * what a leak looks like — don't do this in real code!)
 * Then fix it by adding free() in the right place.
 * ============================================================
 *
 * YOUR TASK 6b:
 * Implement a simple memory pool:
 *   #define POOL_SIZE 10
 *   static int pool[POOL_SIZE];
 *   static int pool_used[POOL_SIZE];  // 0 = free, 1 = in use
 *
 *   int *pool_alloc(void);       // returns pointer to a free slot
 *   void pool_free(int *ptr);    // marks a slot as free
 *
 * This is how real embedded systems handle "dynamic" allocation
 * without using malloc. Test it by allocating 10 slots, freeing
 * some, and allocating again.
 * ============================================================
 *
 * YOUR TASK 6c:
 * Explain out loud (practice for interview):
 *   1. What happens to a local variable when its function returns?
 *   2. Why is returning a pointer to a local variable dangerous?
 *   3. When would you use static local vs malloc?
 *   4. Why does Red Cat (a drone company) probably avoid malloc?
 * ============================================================
 */
