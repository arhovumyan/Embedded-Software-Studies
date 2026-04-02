/*
 * ============================================================
 * EXERCISE 9: Smart Pointers — Never Leak Again
 * ============================================================
 * GOAL: Understand unique_ptr, shared_ptr, and weak_ptr.
 * These use RAII to automatically manage heap memory.
 *
 * THE RULE: In modern C++, you should NEVER use raw new/delete.
 * Use smart pointers instead. They prevent:
 *   - Memory leaks (forgot to delete)
 *   - Double frees (deleted twice)
 *   - Dangling pointers (use after delete)
 *
 * Compile: g++ -std=c++17 -Wall -o ex09 09_smart_pointers.cpp
 * ============================================================
 */

#include <iostream>
#include <memory>
#include <vector>
using namespace std;

// A class to track construction/destruction
class Sensor {
private:
    string name;
    double value;

public:
    Sensor(const string &name) : name(name), value(0) {
        cout << "  + Sensor CREATED: " << name << endl;
    }

    ~Sensor() {
        cout << "  - Sensor DESTROYED: " << name << endl;
    }

    void read() {
        value = 42.0;
        cout << "  " << name << " read: " << value << endl;
    }

    string getName() const { return name; }
};

// ============================================================
// PART 1: The problem with raw pointers
// ============================================================

void raw_pointer_problems() {
    cout << "=== Raw Pointer Problems ===" << endl;

    // Problem 1: Forgetting to delete
    {
        Sensor *s = new Sensor("Leaked");
        s->read();
        // Oops, forgot: delete s;
        // Memory leak! Sensor destructor never called.
    }
    cout << "  (Leaked sensor was never destroyed)\n" << endl;

    // Problem 2: Exception/early return
    {
        Sensor *s = new Sensor("EarlyReturn");
        s->read();
        if (true) {
            // Some condition causes early return
            // delete s;  // You need this on EVERY exit path
            // In real code with exceptions, this is nearly impossible
            delete s;
        }
    }
    cout << endl;
}

// ============================================================
// PART 2: unique_ptr — one owner, automatic cleanup
// ============================================================

void unique_ptr_demo() {
    cout << "=== unique_ptr (Exclusive Ownership) ===" << endl;

    // Create with make_unique (preferred)
    {
        auto sensor = make_unique<Sensor>("UniqueSensor");
        sensor->read();           // use -> just like a raw pointer
        cout << "  Name: " << sensor->getName() << endl;
        // No delete needed! Automatically destroyed here.
    }
    cout << "  (UniqueSensor automatically destroyed)\n" << endl;

    // You CAN'T copy a unique_ptr — that's the point (one owner)
    {
        auto a = make_unique<Sensor>("Original");
        // auto b = a;          // COMPILE ERROR: can't copy
        auto b = move(a);       // But you can MOVE ownership
        // Now 'a' is nullptr and 'b' owns the sensor
        cout << "  After move: a is " << (a ? "valid" : "null")
             << ", b is " << (b ? "valid" : "null") << endl;
        b->read();
    }
    cout << endl;

    // Perfect for containers
    {
        vector<unique_ptr<Sensor>> sensors;
        sensors.push_back(make_unique<Sensor>("GPS"));
        sensors.push_back(make_unique<Sensor>("IMU"));
        sensors.push_back(make_unique<Sensor>("Baro"));

        cout << "  Reading all sensors in vector:" << endl;
        for (auto &s : sensors) {  // must use reference! can't copy unique_ptr
            s->read();
        }
        // All sensors automatically destroyed when vector goes out of scope
    }
    cout << "  (All sensors in vector destroyed)\n" << endl;
}

// ============================================================
// PART 3: shared_ptr — multiple owners, reference counted
// ============================================================

void shared_ptr_demo() {
    cout << "=== shared_ptr (Shared Ownership) ===" << endl;

    shared_ptr<Sensor> s1;

    {
        auto s2 = make_shared<Sensor>("SharedSensor");
        cout << "  Ref count: " << s2.use_count() << endl;  // 1

        s1 = s2;  // Both s1 and s2 now own the sensor
        cout << "  Ref count: " << s1.use_count() << endl;  // 2

        {
            auto s3 = s1;  // Three owners
            cout << "  Ref count: " << s1.use_count() << endl;  // 3
        }
        // s3 destroyed, ref count drops to 2
        cout << "  After s3 dies: " << s1.use_count() << endl;  // 2
    }
    // s2 destroyed, ref count drops to 1
    cout << "  After s2 dies: " << s1.use_count() << endl;  // 1

    s1->read();  // still valid!
    cout << "  Resetting s1..." << endl;
    s1.reset();  // ref count drops to 0 — sensor destroyed
    cout << endl;
}

// ============================================================
// PART 4: weak_ptr — non-owning observer
// ============================================================

void weak_ptr_demo() {
    cout << "=== weak_ptr (Non-Owning Reference) ===" << endl;

    weak_ptr<Sensor> observer;

    {
        auto owner = make_shared<Sensor>("ObservedSensor");
        observer = owner;  // weak_ptr watches but doesn't own

        cout << "  Owner ref count: " << owner.use_count() << endl;  // still 1!
        cout << "  Observer expired? " << observer.expired() << endl;  // 0 (false)

        // To use a weak_ptr, you must "lock" it to get a shared_ptr
        if (auto locked = observer.lock()) {
            locked->read();
            cout << "  Locked ref count: " << owner.use_count() << endl;  // 2 temporarily
        }
        // locked destroyed, back to 1
    }
    // owner destroyed, sensor gone

    cout << "  Observer expired? " << observer.expired() << endl;  // 1 (true)

    // Trying to lock now returns nullptr
    if (auto locked = observer.lock()) {
        locked->read();  // won't execute
    } else {
        cout << "  Can't lock — object is gone!" << endl;
    }
    cout << endl;
}

// ============================================================
// PART 5: Passing smart pointers to functions
// ============================================================

// Use raw pointer/reference when function doesn't affect ownership
void readSensor(Sensor *s) {  // or: void readSensor(Sensor &s)
    s->read();
}

// Use const unique_ptr& to "borrow" without taking ownership
void inspectSensor(const unique_ptr<Sensor> &s) {
    cout << "  Inspecting: " << s->getName() << endl;
}

// Use unique_ptr by value to TAKE ownership (caller gives it up)
void consumeSensor(unique_ptr<Sensor> s) {
    cout << "  Consuming: " << s->getName() << endl;
    // s is destroyed at end of function
}

void passing_demo() {
    cout << "=== Passing Smart Pointers ===" << endl;

    auto s = make_unique<Sensor>("PassDemo");

    readSensor(s.get());        // .get() gives raw pointer — no ownership transfer
    inspectSensor(s);           // borrow via const reference
    // consumeSensor(s);        // ERROR: can't copy unique_ptr
    consumeSensor(move(s));     // move: transfer ownership
    cout << "  After consume: s is " << (s ? "valid" : "null") << endl;
    cout << endl;
}

// ============================================================
// PART 6: When to use which
// ============================================================

/*
 * DECISION GUIDE:
 *
 * unique_ptr:  DEFAULT CHOICE. Use when one thing owns the resource.
 *              99% of the time, this is what you want.
 *              Zero overhead compared to raw pointer.
 *              Examples: local objects, class members, factory returns.
 *
 * shared_ptr:  Use when ownership is genuinely shared — multiple
 *              things need the resource and you don't know which
 *              dies last. Has overhead (reference count, control block).
 *              Examples: shared cache, observer patterns, graph nodes.
 *
 * weak_ptr:    Use to observe a shared_ptr without keeping it alive.
 *              Prevents circular reference memory leaks.
 *              Examples: cache that shouldn't prevent cleanup, parent
 *              references in tree structures.
 *
 * Raw pointer: Use for non-owning references when you know the
 *              lifetime is managed elsewhere. Function parameters
 *              that just "borrow" an object.
 *
 * EMBEDDED NOTE: unique_ptr has ZERO overhead — same as raw pointer.
 * shared_ptr has overhead (atomic ref count). In real-time embedded,
 * prefer unique_ptr or just use stack/static allocation.
 */

// ============================================================

int main() {
    raw_pointer_problems();
    unique_ptr_demo();
    shared_ptr_demo();
    weak_ptr_demo();
    passing_demo();
    return 0;
}

/*
 * ============================================================
 * YOUR TASK 9a:
 * Rewrite the Sensor vector example from unique_ptr_demo using
 * raw pointers (new/delete). Count how many places you need
 * to remember to call delete. Then appreciate unique_ptr.
 * ============================================================
 *
 * YOUR TASK 9b:
 * Create a class SensorManager that:
 *   - Stores a vector<unique_ptr<Sensor>>
 *   - Has addSensor(string name) that creates and stores a sensor
 *   - Has readAll() that reads every sensor
 *   - Destructor prints "Manager shutting down"
 * Watch all sensors get cleaned up automatically when the manager dies.
 * ============================================================
 *
 * YOUR TASK 9c:
 * Create a circular reference scenario with shared_ptr:
 *   class Node {
 *       shared_ptr<Node> next;   // This creates a cycle!
 *   };
 * Create two nodes pointing to each other. Neither gets destroyed.
 * Fix it by making one of them a weak_ptr.
 * ============================================================
 *
 * INTERVIEW TIP: "unique_ptr vs shared_ptr?"
 * "unique_ptr is exclusive ownership with zero overhead — use it
 * by default. shared_ptr is reference-counted shared ownership
 * with atomic increment/decrement overhead. In embedded, I prefer
 * unique_ptr or stack allocation to avoid the overhead and
 * unpredictable destructor timing of shared_ptr."
 * ============================================================
 */
