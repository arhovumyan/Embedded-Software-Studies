/*
 * ============================================================
 * EXERCISE 8: CAPSTONE — Build a Mini Sensor System
 * ============================================================
 * This combines EVERYTHING from exercises 1-7 into one program
 * that simulates something you'd actually write at Red Cat.
 *
 * You're building a sensor data pipeline for a drone:
 *   - A ring buffer (circular buffer) stores sensor readings
 *   - An ISR writes new readings
 *   - A processing function reads them
 *   - Function pointers select which filter to apply
 *   - Proper use of static, const, volatile
 *
 * COMPLETE THE SECTIONS MARKED WITH "TODO"
 * ============================================================
 */

#include <stdio.h>
#include <string.h>

// === Configuration (const — could go in flash/ROM) ===
#define BUFFER_SIZE 8

// TODO 1: Make this array const (it's a lookup table, shouldn't change)
/* ??? */ int calibration_offsets[] = {-2, -1, 0, 1, 0, -1, 2, 1};

// === Ring Buffer Structure ===
typedef struct {
    int data[BUFFER_SIZE];
    int head;           // where to write next
    int tail;           // where to read next
    int count;          // how many items in buffer
} RingBuffer;

// TODO 2: This variable is written by an ISR and read by main loop.
//         What keyword does it need? Add it.
/* ??? */ int new_data_available = 0;

// === Ring Buffer Functions ===

// TODO 3: The RingBuffer pointer parameter should indicate we WILL modify it.
//         Should it be const or non-const? (trick question — think about it)
void ring_buffer_init(RingBuffer *rb) {
    memset(rb->data, 0, sizeof(rb->data));
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

void ring_buffer_push(RingBuffer *rb, int value) {
    rb->data[rb->head] = value;
    rb->head = (rb->head + 1) % BUFFER_SIZE;  // wrap around
    if (rb->count < BUFFER_SIZE) {
        rb->count++;
    } else {
        rb->tail = (rb->tail + 1) % BUFFER_SIZE;  // overwrite oldest
    }
}

// TODO 4: This function should NOT modify the ring buffer.
//         What keyword should you add to the parameter?
int ring_buffer_peek(RingBuffer *rb, int index) {
    if (index >= rb->count) return -1;
    int actual_index = (rb->tail + index) % BUFFER_SIZE;
    return rb->data[actual_index];
}

int ring_buffer_pop(RingBuffer *rb) {
    if (rb->count == 0) return -1;
    int value = rb->data[rb->tail];
    rb->tail = (rb->tail + 1) % BUFFER_SIZE;
    rb->count--;
    return value;
}

// === Filter Functions (for function pointers) ===

// Simple pass-through
int filter_none(int value) {
    return value;
}

// Apply calibration offset
int filter_calibrate(int value) {
    // TODO 5: Use the calibration_offsets array.
    // Use a static local to track which offset to use (cycling 0-7).
    // Hint: static int index = 0; then index = (index + 1) % BUFFER_SIZE;
    return value;  // fix this
}

// Clamp to range [0, 100]
int filter_clamp(int value) {
    if (value < 0) return 0;
    if (value > 100) return 100;
    return value;
}

// === Filter function pointer type ===
typedef int (*filter_func)(int);

// === Simulated ISR (called by "hardware") ===

static RingBuffer sensor_buffer;  // static — file scope only

void simulated_sensor_isr(int reading) {
    ring_buffer_push(&sensor_buffer, reading);
    new_data_available = 1;
}

// === Processing function ===

// TODO 6: Implement this function.
// It should:
//   1. Read all available data from sensor_buffer using ring_buffer_pop
//   2. Apply the filter function to each reading
//   3. Print each filtered reading
//   4. Keep a running sum using a static local variable
//   5. Print the running sum at the end
//   6. Reset new_data_available to 0
void process_readings(filter_func filter) {
    // YOUR CODE HERE
}

// === Main: Simulate the system ===

int main() {
    ring_buffer_init(&sensor_buffer);

    // Simulated raw sensor readings (as if from ADC)
    int raw_readings[] = {45, 52, -3, 78, 102, 67, 33, 91, 55, 12};
    int num_readings = sizeof(raw_readings) / sizeof(raw_readings[0]);

    // Array of available filters — jump table pattern
    filter_func filters[] = {filter_none, filter_calibrate, filter_clamp};
    const char *filter_names[] = {"none", "calibrate", "clamp"};

    // TODO 7: Select which filter to use (0, 1, or 2)
    int selected_filter = 0;
    printf("Using filter: %s\n\n", filter_names[selected_filter]);

    // Simulate: ISR fires for each reading, then we process
    for (int batch = 0; batch < num_readings; batch += 3) {
        printf("--- ISR batch %d ---\n", batch / 3);

        // Simulate 3 ISR fires
        for (int i = batch; i < batch + 3 && i < num_readings; i++) {
            printf("  ISR received: %d\n", raw_readings[i]);
            simulated_sensor_isr(raw_readings[i]);
        }

        // Main loop detects new data and processes
        if (new_data_available) {
            process_readings(filters[selected_filter]);
        }
        printf("\n");
    }

    // TODO 8: Print the address of sensor_buffer, showing it's
    // in the global/static area (not stack, not heap)
    printf("sensor_buffer address: ???\n");

    // TODO 9: Show that you understand pointer arithmetic.
    // Create a pointer to the raw_readings array.
    // Use pointer arithmetic (not []) to find and print the 5th element.

    return 0;
}

/*
 * ============================================================
 * AFTER COMPLETING THIS:
 *
 * 1. Compile and run it. Fix any bugs.
 *
 * 2. Add -O2 flag. Does the volatile/non-volatile behavior
 *    change anything?
 *
 * 3. Draw the memory layout on paper:
 *    - Where is sensor_buffer? (static/global area)
 *    - Where is raw_readings? (stack, inside main)
 *    - Where are the filter functions? (code/text segment)
 *    - Where is the static variable inside filter_calibrate?
 *      (static/global area, even though it's "inside" a function)
 *
 * 4. Practice explaining this program to an imaginary interviewer:
 *    "This is a simulated sensor pipeline. I use a ring buffer
 *    for lock-free producer-consumer between an ISR and the main
 *    loop. The ISR flag is volatile because... I use function
 *    pointers for filters because... The calibration table is
 *    const because..."
 *
 * If you can explain this program fluently, you're ready for
 * the embedded interview.
 * ============================================================
 */
