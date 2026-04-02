/*
 * ============================================================
 * EXERCISE 1: From C Structs to C++ Classes
 * ============================================================
 * GOAL: Understand that a class is just a struct with
 * superpowers. If you understand structs, you already
 * understand 60% of classes.
 *
 * Compile: g++ -std=c++17 -Wall -o ex01 01_structs_to_classes.cpp
 * ============================================================
 */

#include <iostream>
#include <cstring>
using namespace std;

// ============================================================
// PART 1: The C way — struct + separate functions
// ============================================================

// In C, you'd do this:
struct MotorC {
    int speed;
    int max_speed;
    bool enabled;
};

// Functions that operate on the struct — you pass a pointer
void motor_c_init(MotorC *m, int max_speed) {
    m->speed = 0;
    m->max_speed = max_speed;
    m->enabled = false;
}

void motor_c_enable(MotorC *m) {
    m->enabled = true;
}

void motor_c_set_speed(MotorC *m, int speed) {
    if (!m->enabled) {
        printf("Error: motor not enabled!\n");
        return;
    }
    if (speed > m->max_speed) speed = m->max_speed;
    if (speed < 0) speed = 0;
    m->speed = speed;
}

void motor_c_print(const MotorC *m) {
    printf("C Motor: speed=%d/%d, enabled=%s\n",
           m->speed, m->max_speed, m->enabled ? "yes" : "no");
}

// Problems with the C way:
//   1. ANYONE can do: m.speed = 99999; — no protection
//   2. You can forget to call motor_c_init — uninitialized data
//   3. Nothing prevents you from passing the wrong struct
//   4. Function names get long (motor_c_this, motor_c_that)
//   5. Data and functions are separate — hard to see what belongs together

// ============================================================
// PART 2: The C++ way — class
// ============================================================

class Motor {
// PRIVATE: Only Motor's own methods can touch these
// This is ENCAPSULATION — protecting internal state
private:
    int speed;
    int max_speed;
    bool enabled;

// PUBLIC: Anyone can call these
// This is the INTERFACE — how the outside world interacts
public:
    // Constructor — called automatically when you create a Motor
    // No more forgetting to init!
    Motor(int max_speed) {
        this->speed = 0;                // 'this' is the pointer to yourself
        this->max_speed = max_speed;     // like 'm->' in the C version
        this->enabled = false;
        cout << "  Motor created with max_speed=" << max_speed << endl;
    }

    // Destructor — called automatically when Motor is destroyed
    ~Motor() {
        cout << "  Motor destroyed (was speed=" << speed << ")" << endl;
    }

    void enable() {
        enabled = true;      // no need for 'this->' (implicit)
    }

    void disable() {
        enabled = false;
        speed = 0;           // safety: stop motor when disabling
    }

    void setSpeed(int s) {
        if (!enabled) {
            cout << "Error: motor not enabled!" << endl;
            return;
        }
        if (s > max_speed) s = max_speed;
        if (s < 0) s = 0;
        speed = s;
    }

    // 'const' after the parentheses means: this method
    // promises NOT to modify any member variables
    int getSpeed() const {
        return speed;
    }

    int getMaxSpeed() const {
        return max_speed;
    }

    bool isEnabled() const {
        return enabled;
    }

    void print() const {
        cout << "C++ Motor: speed=" << speed << "/" << max_speed
             << ", enabled=" << (enabled ? "yes" : "no") << endl;
    }
};

// ============================================================
// PART 3: See the difference
// ============================================================

void demo_c_way() {
    cout << "=== The C Way ===" << endl;

    MotorC m;
    motor_c_init(&m, 1000);
    motor_c_print(&m);

    motor_c_enable(&m);
    motor_c_set_speed(&m, 500);
    motor_c_print(&m);

    // PROBLEM: Nothing stops this!
    m.speed = 999999;  // direct access, bypasses all safety
    m.max_speed = -1;  // nonsense values
    motor_c_print(&m);
    cout << endl;
}

void demo_cpp_way() {
    cout << "=== The C++ Way ===" << endl;

    Motor m(1000);     // Constructor called automatically
    m.print();

    m.enable();
    m.setSpeed(500);
    m.print();

    // PROTECTED: These would cause compile errors:
    // m.speed = 999999;     // ERROR: 'speed' is private
    // m.max_speed = -1;     // ERROR: 'max_speed' is private

    // You MUST use the public interface:
    m.setSpeed(999999);      // This works but clamps to max_speed
    m.print();

    cout << endl;
}  // <-- Motor destructor called automatically here!

// ============================================================
// PART 4: Stack vs Heap with objects
// ============================================================

void demo_object_lifetime() {
    cout << "=== Object Lifetime ===" << endl;

    // STACK allocation — automatic cleanup
    {
        cout << "Entering scope:" << endl;
        Motor stack_motor(500);
        stack_motor.enable();
        stack_motor.setSpeed(200);
        stack_motor.print();
        cout << "Leaving scope:" << endl;
    }  // <-- destructor called here automatically!
    cout << "Scope exited.\n" << endl;

    // HEAP allocation — manual cleanup (old C++ way)
    cout << "Heap allocation:" << endl;
    Motor *heap_motor = new Motor(800);  // like malloc + constructor
    heap_motor->enable();                // arrow operator, just like C
    heap_motor->setSpeed(400);
    heap_motor->print();
    delete heap_motor;                   // like destructor + free
    cout << "Heap motor deleted.\n" << endl;

    // NOTE: In modern C++, you'd use smart pointers instead of
    // new/delete. We'll cover that in exercise 09.
}

int main() {
    demo_c_way();
    demo_cpp_way();
    demo_object_lifetime();

    return 0;
}

/*
 * ============================================================
 * YOUR TASK 1a:
 * Create a class called LED with:
 *   - Private members: brightness (int, 0-255), is_on (bool), color (string)
 *   - Constructor that takes a color string
 *   - Methods: turnOn(), turnOff(), setBrightness(int),
 *     getBrightness(), print()
 *   - setBrightness should clamp to 0-255
 *   - Destructor that prints "LED destroyed"
 *
 * Create 3 LEDs on the stack. Manipulate them. Watch the
 * destructors fire in REVERSE order (LIFO — last in, first out).
 * ============================================================
 *
 * YOUR TASK 1b:
 * Take the MotorC struct from Part 1 and try to directly access
 * the private members of the Motor class from Part 2.
 * Read the compiler errors. Understand what they mean.
 * This is encapsulation enforced by the compiler.
 * ============================================================
 *
 * YOUR TASK 1c:
 * Create a Motor on the heap with 'new'. Deliberately forget to
 * call 'delete'. Notice the destructor message never prints.
 * This is a MEMORY LEAK. In embedded, this kills your device
 * after hours/days of running.
 * ============================================================
 *
 * KEY INSIGHT: A class is a struct where:
 *   - Data and functions live together
 *   - You control who can access what
 *   - Construction and destruction happen automatically
 *   - The compiler enforces your design decisions
 * ============================================================
 */
