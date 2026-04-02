/*
 * ============================================================
 * EXERCISE 8: Templates — Write Once, Use for Any Type
 * ============================================================
 * GOAL: Understand how templates let you write code that works
 * with ANY type without sacrificing type safety.
 *
 * Think of templates as "code stencils" — the compiler stamps
 * out a specialized version for each type you use.
 *
 * Compile: g++ -std=c++17 -Wall -o ex08 08_templates.cpp
 * ============================================================
 */

#include <iostream>
#include <string>
using namespace std;

// ============================================================
// PART 1: Function Templates
// ============================================================

// Without templates — you'd need one for each type:
// int max_int(int a, int b) { return (a > b) ? a : b; }
// double max_double(double a, double b) { return (a > b) ? a : b; }
// This is tedious and error-prone.

// With templates — one version handles ALL types:
template <typename T>
T findMax(T a, T b) {
    return (a > b) ? a : b;
}

// The compiler generates findMax<int>, findMax<double>, etc.
// as needed. Zero runtime cost — it's all done at compile time.

template <typename T>
void swapValues(T &a, T &b) {
    T temp = a;
    a = b;
    b = temp;
}

// Template with multiple type parameters
template <typename T, typename U>
void printPair(const T &first, const U &second) {
    cout << "  (" << first << ", " << second << ")" << endl;
}

void function_template_demo() {
    cout << "=== Function Templates ===" << endl;

    // Compiler deduces the type automatically
    cout << "  max(3, 7) = " << findMax(3, 7) << endl;
    cout << "  max(3.14, 2.71) = " << findMax(3.14, 2.71) << endl;
    cout << "  max('a', 'z') = " << findMax('a', 'z') << endl;

    // Or you can specify explicitly
    cout << "  max<string>(\"apple\", \"banana\") = "
         << findMax<string>("apple", "banana") << endl;

    int a = 10, b = 20;
    cout << "\n  Before swap: a=" << a << " b=" << b << endl;
    swapValues(a, b);
    cout << "  After swap:  a=" << a << " b=" << b << endl;

    cout << "\n  Mixed pairs:" << endl;
    printPair(42, "hello");
    printPair(3.14, true);
    printPair("sensor", 9.81);
}

// ============================================================
// PART 2: Class Templates
// ============================================================

// A generic ring buffer that works with ANY type
template <typename T, int SIZE>
class RingBuffer {
private:
    T data[SIZE];     // SIZE known at compile time — no heap allocation!
    int head;
    int tail;
    int count;

public:
    RingBuffer() : head(0), tail(0), count(0) {
        for (int i = 0; i < SIZE; i++) data[i] = T();
    }

    bool push(const T &value) {
        data[head] = value;
        head = (head + 1) % SIZE;
        if (count < SIZE) {
            count++;
            return true;
        } else {
            tail = (tail + 1) % SIZE;  // overwrite oldest
            return false;  // indicate overflow
        }
    }

    bool pop(T &out) {
        if (count == 0) return false;
        out = data[tail];
        tail = (tail + 1) % SIZE;
        count--;
        return true;
    }

    int size() const { return count; }
    bool isEmpty() const { return count == 0; }
    bool isFull() const { return count == SIZE; }
    int capacity() const { return SIZE; }

    void print() const {
        cout << "  RingBuffer[" << count << "/" << SIZE << "]: ";
        int idx = tail;
        for (int i = 0; i < count; i++) {
            cout << data[idx] << " ";
            idx = (idx + 1) % SIZE;
        }
        cout << endl;
    }
};

void class_template_demo() {
    cout << "\n=== Class Templates ===" << endl;

    // Integer ring buffer with capacity 5
    RingBuffer<int, 5> intBuf;
    for (int i = 1; i <= 7; i++) {
        intBuf.push(i * 10);
    }
    cout << "  Int buffer (pushed 7 into size 5):" << endl;
    intBuf.print();

    // Double ring buffer with capacity 3
    RingBuffer<double, 3> doubleBuf;
    doubleBuf.push(3.14);
    doubleBuf.push(2.71);
    doubleBuf.push(1.41);
    cout << "  Double buffer:" << endl;
    doubleBuf.print();

    // String ring buffer
    RingBuffer<string, 4> strBuf;
    strBuf.push("GPS");
    strBuf.push("IMU");
    strBuf.push("BARO");
    cout << "  String buffer:" << endl;
    strBuf.print();

    // Pop from int buffer
    int val;
    while (intBuf.pop(val)) {
        cout << "  Popped: " << val << endl;
    }
}

// ============================================================
// PART 3: Template with custom types
// ============================================================

struct SensorReading {
    string name;
    double value;
    int timestamp;

    // Need this for T() default construction in RingBuffer
    SensorReading() : name(""), value(0), timestamp(0) {}
    SensorReading(const string &n, double v, int t)
        : name(n), value(v), timestamp(t) {}

    // Need this for cout << in print()
    friend ostream& operator<<(ostream &os, const SensorReading &r) {
        os << "{" << r.name << ":" << r.value << "@" << r.timestamp << "}";
        return os;
    }
};

void custom_type_demo() {
    cout << "\n=== Templates with Custom Types ===" << endl;

    RingBuffer<SensorReading, 4> sensorLog;
    sensorLog.push(SensorReading("GPS", 34.05, 1000));
    sensorLog.push(SensorReading("IMU", 9.81, 1001));
    sensorLog.push(SensorReading("BARO", 1013.25, 1002));

    sensorLog.print();

    // Pop and process
    SensorReading r;
    while (sensorLog.pop(r)) {
        cout << "  Processing: " << r.name << " = " << r.value << endl;
    }
}

// ============================================================
// PART 4: Why this matters for embedded
// ============================================================

/*
 * TEMPLATES IN EMBEDDED:
 *
 * 1. ZERO RUNTIME COST: Templates are resolved at compile time.
 *    RingBuffer<int, 8> generates specialized code — no virtual
 *    dispatch, no heap allocation, no runtime type checking.
 *
 * 2. FIXED-SIZE CONTAINERS: RingBuffer<T, SIZE> puts the array
 *    on the stack or in static memory. No malloc needed.
 *    This is PERFECT for embedded where heap is dangerous.
 *
 * 3. TYPE SAFETY: Unlike void* in C (which loses all type info),
 *    templates keep full type checking. The compiler catches
 *    errors at build time, not at runtime on a flying drone.
 *
 * 4. CODE SIZE TRADEOFF: Each instantiation (RingBuffer<int,8>,
 *    RingBuffer<double,16>) generates separate code. On a
 *    microcontroller with 64KB flash, this can matter.
 *    Use templates judiciously in very constrained environments.
 *
 * COMPARISON:
 *   C void* approach:     Flexible but unsafe. Errors at runtime.
 *   C++ virtual approach:  Safe but has runtime cost (vtable).
 *   C++ template approach: Safe AND no runtime cost. Compile-time only.
 */

// ============================================================

int main() {
    function_template_demo();
    class_template_demo();
    custom_type_demo();
    return 0;
}

/*
 * ============================================================
 * YOUR TASK 8a:
 * Write a template function:
 *   template <typename T>
 *   T clamp(T value, T min, T max)
 * that clamps a value to a range. Test with int, double, char.
 * ============================================================
 *
 * YOUR TASK 8b:
 * Create a template class Pair<T, U> with:
 *   - Two members: first (type T), second (type U)
 *   - Constructor, getters, print()
 *   - operator== that checks both members
 *
 * Test: Pair<string, int>("temperature", 22)
 *       Pair<int, double>(1, 3.14)
 * ============================================================
 *
 * YOUR TASK 8c:
 * Create a template class StaticArray<T, int N> that wraps a
 * fixed-size array with bounds checking:
 *   - T data[N] stored inside the object (stack allocation)
 *   - set(index, value) with bounds check
 *   - T get(index) with bounds check
 *   - int size() returns N
 *
 * This is basically a simplified std::array. No heap allocation,
 * perfect for embedded. Compare it to your C dynamic array.
 * ============================================================
 *
 * INTERVIEW TIP: "Templates vs Virtual functions?"
 * "Templates provide compile-time polymorphism with zero runtime
 * cost but increase code size. Virtual functions provide runtime
 * polymorphism with a small indirection cost. In embedded, I'd
 * use templates for performance-critical data structures and
 * virtual functions for plugin architectures where flexibility
 * matters more than nanoseconds."
 * ============================================================
 */
