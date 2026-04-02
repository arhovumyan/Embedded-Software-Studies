# Embedded C Study Guide — Interview Prep for Red Cat Holdings
## April 8th Interview — You have 7 days

---

## STUDY SCHEDULE

### Day 1 (Today) — Pointers & Pointer Arithmetic
- Read and run: `01_pointer_basics.c`
- Read and run: `02_swap_with_pointers.c`
- Read and run: `03_pointer_arithmetic.c`
- Complete all tasks (1a, 1b, 2a, 2b, 3a, 3b, 3c)
- Goal: You can explain what `*(p + 3)` does without hesitation

### Day 2 — Double Pointers & Function Pointers
- Read and run: `04_double_pointers.c`
- Read and run: `05_function_pointers.c`
- Complete all tasks (4a, 4b, 5a, 5b)
- Goal: You can explain when you need `int**` vs `int*`

### Day 3 — Memory & Keywords
- Read and run: `06_stack_vs_heap.c`
- Read and run: `07_static_const_volatile.c`
- Complete all tasks (6a, 6b, 6c, 7a, 7b, 7c, 7d)
- Goal: You can answer "why does embedded avoid malloc?" in 20 seconds

### Day 4 — Capstone
- Complete: `08_capstone_sensor_system.c`
- Fill in ALL TODOs. Debug until it compiles and runs clean.
- Draw the memory layout on paper
- Practice explaining the program out loud (set a timer, 2 minutes max)

### Day 5 — Review & Verbal Practice
- Re-do any tasks you struggled with
- Practice answering these OUT LOUD (record yourself if possible):
  1. What's the difference between a pointer and the value it points to?
  2. What happens when you do pointer + 1 on an int* vs a struct*?
  3. When do you need a double pointer?
  4. What are function pointers used for in embedded?
  5. Stack vs heap: what goes where?
  6. Why does embedded avoid malloc?
  7. What does static do? (all 3 meanings)
  8. What does volatile do and when do you use it?
  9. Read `const int *p` vs `int *const p` — what's the difference?
  10. What's a ring buffer and why is it useful in embedded?

### Day 6 — Red Cat Specific Prep
- Research Red Cat Holdings: drones, Teal drones, military/defense
- Think about: motor control, flight controllers, sensor fusion,
  communication protocols (UART, SPI, I2C)
- Be ready to talk about: real-time constraints, interrupt handling,
  bare-metal vs RTOS

### Day 7 — Light Review
- Don't cram. Skim your completed tasks.
- Practice your verbal answers one more time.
- Get good sleep.

---

## QUICK REFERENCE CHEAT SHEET

### Pointer Declarations — Read Right to Left
```
int *p;              → p is a pointer to int
int **pp;            → pp is a pointer to a pointer to int
const int *p;        → p is a pointer to a const int (can't change value)
int *const p;        → p is a const pointer to int (can't change pointer)
const int *const p;  → p is a const pointer to a const int (can't change anything)
int (*fp)(int, int); → fp is a pointer to a function taking two ints, returning int
```

### Pointer Arithmetic
```
int *p = arr;
p + 1            → moves forward sizeof(int) = 4 bytes
p + n            → moves forward n * sizeof(int) bytes

MYSTRUCT *p = structs;
p + 1            → moves forward sizeof(MYSTRUCT) bytes (includes padding!)

*(p + i)  is the same as  p[i]
&arr[i]   is the same as  arr + i
p->member is the same as  (*p).member
```

### Memory Segments
```
STACK:   Local variables. Grows down. Automatic cleanup. Fast.
HEAP:    malloc/calloc. Grows up. YOU must free. Fragmentation risk.
DATA:    Global and static variables. Lives for entire program.
BSS:     Uninitialized globals (zeroed at startup).
TEXT:    Your code (read-only).
```

### Keywords
```
static local var:     Persists across calls. Stored in data segment.
static global/func:   File-private. Not visible to other .c files.
const:                Cannot be modified. May go in flash/ROM.
volatile:             Compiler must always read from memory, no caching.
                      Use for: HW registers, ISR variables, shared memory.
```

### Common Embedded Patterns
```
Ring buffer:          Lock-free producer-consumer (ISR writes, main reads)
Jump table:           Array of function pointers replacing switch/case
Memory pool:          Pre-allocated fixed-size blocks replacing malloc
Bit manipulation:     SET: reg |= (1 << bit)
                      CLEAR: reg &= ~(1 << bit)
                      TOGGLE: reg ^= (1 << bit)
                      CHECK: if (reg & (1 << bit))
```

### Struct Padding Rule
```
Each member is aligned to its own size:
  char:      1-byte aligned
  short:     2-byte aligned
  int:       4-byte aligned
  double:    8-byte aligned
  long long: 8-byte aligned

Struct total size is rounded up to alignment of largest member.

struct Example {
    int a;      // 4 bytes at offset 0
                // 4 bytes PADDING (to align double at offset 8)
    double b;   // 8 bytes at offset 8
    int c;      // 4 bytes at offset 16
                // 4 bytes PADDING (total must be multiple of 8)
};              // sizeof = 24, not 16!
```

### Hex Quick Reference
```
0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15

0x0 + 4 = 0x4
0x4 + 4 = 0x8
0x8 + 4 = 0xC    (8 + 4 = 12 = C)
0xC + 4 = 0x10   (12 + 4 = 16 = 0x10)
```
