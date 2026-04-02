/*
 * ============================================================
 * EXERCISE 3: Constructors, Destructors, and RAII
 * ============================================================
 * GOAL: Understand the object lifecycle and RAII — the most
 * important C++ idiom. RAII = "Resource Acquisition Is
 * Initialization" — meaning you acquire resources (memory,
 * files, hardware) in constructors and release them in
 * destructors. This makes resource leaks nearly impossible.
 *
 * Compile: g++ -std=c++17 -Wall -o ex03 03_constructors_destructors.cpp
 * ============================================================
 */

#include <iostream>
#include <cstring>
using namespace std;

// ============================================================
// PART 1: Types of Constructors
// ============================================================

class Sensor {
private:
    string name;
    int id;
    double *readings;       // dynamically allocated array
    int capacity;
    int count;

public:
    // 1. DEFAULT CONSTRUCTOR — no arguments
    Sensor() : name("Unknown"), id(-1), readings(nullptr),
               capacity(0), count(0) {
        cout << "  Default constructor: " << name << endl;
    }

    // 2. PARAMETERIZED CONSTRUCTOR — takes arguments
    Sensor(const string &name, int id, int capacity)
        : name(name), id(id), capacity(capacity), count(0) {
        readings = new double[capacity];  // allocate on heap
        // Zero out the array
        for (int i = 0; i < capacity; i++) {
            readings[i] = 0.0;
        }
        cout << "  Parameterized constructor: " << name
             << " (capacity=" << capacity << ")" << endl;
    }

    // 3. COPY CONSTRUCTOR — creates a copy of another object
    // Called when: Sensor b = a; or Sensor b(a); or passing by value
    Sensor(const Sensor &other)
        : name(other.name + "_copy"), id(other.id),
          capacity(other.capacity), count(other.count) {
        // DEEP COPY: allocate our own array and copy the data
        readings = new double[capacity];
        for (int i = 0; i < count; i++) {
            readings[i] = other.readings[i];
        }
        cout << "  Copy constructor: " << name << " (copied from "
             << other.name << ")" << endl;

        // WITHOUT deep copy, both objects would point to the SAME array.
        // When one destructor frees it, the other has a dangling pointer.
        // This is called the "shallow copy problem."
    }

    // 4. COPY ASSIGNMENT OPERATOR — assigns one existing object to another
    // Called when: a = b; (both already exist)
    Sensor& operator=(const Sensor &other) {
        cout << "  Copy assignment: " << name << " = " << other.name << endl;

        if (this == &other) return *this;  // self-assignment check

        // Free our old data
        delete[] readings;

        // Copy everything
        name = other.name + "_assigned";
        id = other.id;
        capacity = other.capacity;
        count = other.count;
        readings = new double[capacity];
        for (int i = 0; i < count; i++) {
            readings[i] = other.readings[i];
        }

        return *this;
    }

    // 5. DESTRUCTOR — cleanup when object dies
    ~Sensor() {
        cout << "  Destructor: " << name;
        if (readings) {
            delete[] readings;    // free the heap memory
            cout << " (freed " << capacity << " doubles)";
        }
        cout << endl;
    }

    // Methods
    bool addReading(double value) {
        if (count >= capacity) return false;
        readings[count++] = value;
        return true;
    }

    void print() const {
        cout << "  Sensor[" << name << " #" << id << "]: ";
        for (int i = 0; i < count; i++) {
            cout << readings[i] << " ";
        }
        cout << "(" << count << "/" << capacity << ")" << endl;
    }

    string getName() const { return name; }
};

// ============================================================
// PART 2: When constructors/destructors are called
// ============================================================

void lifecycle_demo() {
    cout << "\n=== Object Lifecycle Demo ===" << endl;

    cout << "\n--- Creating on stack ---" << endl;
    Sensor s1("Gyro", 1, 5);
    s1.addReading(1.1);
    s1.addReading(2.2);
    s1.print();

    cout << "\n--- Copy constructor ---" << endl;
    Sensor s2 = s1;  // copy constructor called
    s2.print();       // independent copy

    // Modify original — copy should NOT change
    s1.addReading(3.3);
    cout << "\nAfter modifying s1:" << endl;
    s1.print();
    s2.print();  // should still show just 1.1, 2.2

    cout << "\n--- Copy assignment ---" << endl;
    Sensor s3("Accel", 2, 3);
    s3.addReading(9.8);
    s3 = s1;  // copy assignment operator called
    s3.print();

    cout << "\n--- Leaving function (destructors fire in reverse) ---" << endl;
}  // s3, s2, s1 destructors fire here, in that order (LIFO)

// ============================================================
// PART 3: RAII — Resource Acquisition Is Initialization
// ============================================================

// RAII means: if you acquire a resource, tie it to an object's lifetime.
// When the object dies (goes out of scope), the resource is released.

class FileHandler {
private:
    FILE *file;
    string filename;

public:
    // Constructor ACQUIRES the resource
    FileHandler(const string &filename, const char *mode)
        : filename(filename) {
        file = fopen(filename.c_str(), mode);
        if (file) {
            cout << "  File opened: " << filename << endl;
        } else {
            cout << "  Failed to open: " << filename << endl;
        }
    }

    // Destructor RELEASES the resource
    ~FileHandler() {
        if (file) {
            fclose(file);
            cout << "  File closed: " << filename << endl;
        }
    }

    // Delete copy constructor and assignment — prevent double-close
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;

    bool write(const string &text) {
        if (!file) return false;
        fprintf(file, "%s", text.c_str());
        return true;
    }

    bool isOpen() const { return file != nullptr; }
};

// With RAII, you literally CANNOT forget to close the file.
// The destructor handles it, no matter how the function exits
// (normal return, exception, early return, etc.)

void raii_demo() {
    cout << "\n=== RAII Demo ===" << endl;

    {
        cout << "Entering scope:" << endl;
        FileHandler fh("/tmp/test_raii.txt", "w");
        fh.write("Hello from RAII!\n");
        fh.write("Resource management made safe.\n");
        cout << "Leaving scope:" << endl;
    }  // <-- file automatically closed here

    cout << "File guaranteed closed.\n" << endl;

    // Even with early returns or errors:
    cout << "Early return scenario:" << endl;
    {
        FileHandler fh("/tmp/test_raii2.txt", "w");
        fh.write("Some data\n");
        if (true) {
            cout << "Oops, returning early!" << endl;
            // In C, you'd need: fclose(file); return;
            // With RAII, just leave — destructor handles it
        }
    }  // <-- still closed properly
}

// ============================================================
// PART 4: The Rule of Three / Rule of Five
// ============================================================

/*
 * THE RULE OF THREE:
 * If your class manages a resource (raw pointer, file handle, etc.)
 * and you write ANY of these three, you probably need ALL three:
 *   1. Destructor
 *   2. Copy constructor
 *   3. Copy assignment operator
 *
 * Why? If you have a destructor, it means you're freeing something.
 * The default copy just copies the pointer (shallow copy), so two
 * objects end up pointing to the same memory. First destructor frees
 * it, second destructor frees it again = crash (double free).
 *
 * THE RULE OF FIVE (Modern C++):
 * Add these two:
 *   4. Move constructor
 *   5. Move assignment operator
 *
 * Move semantics let you STEAL resources from temporary objects
 * instead of copying them. Much faster. We'll cover this later.
 *
 * THE RULE OF ZERO (Best practice):
 * Don't manage raw resources at all. Use smart pointers and
 * standard containers. Then you need NONE of these five.
 */

// ============================================================

int main() {
    cout << "=== Constructor Types ===" << endl;

    Sensor def;                         // default constructor
    Sensor param("GPS", 1, 10);         // parameterized constructor
    Sensor copy(param);                 // copy constructor
    Sensor assigned("Baro", 2, 5);
    assigned = param;                   // copy assignment operator

    cout << "\n";
    def.print();
    param.print();
    copy.print();
    assigned.print();

    lifecycle_demo();
    raii_demo();

    cout << "\n=== Main ending (remaining destructors fire) ===" << endl;
    return 0;
}

/*
 * ============================================================
 * YOUR TASK 3a:
 * Create a class DynamicArray that:
 *   - Stores an int* and a size
 *   - Constructor allocates memory with new[]
 *   - Destructor frees it with delete[]
 *   - Has a proper copy constructor (deep copy)
 *   - Has a proper copy assignment operator
 *   - Has methods: set(index, value), get(index), print()
 *
 * Test: Create one, fill it, copy it, modify the original,
 * verify the copy didn't change. Watch destructors fire.
 * ============================================================
 *
 * YOUR TASK 3b:
 * Create a RAII class called MutexLock:
 *   - Constructor prints "Mutex LOCKED"
 *   - Destructor prints "Mutex UNLOCKED"
 *   - Delete copy constructor and assignment (= delete)
 *
 * Use it in a function with an early return:
 *   void critical_section(bool early_exit) {
 *       MutexLock lock;
 *       // ... do stuff ...
 *       if (early_exit) return;   // mutex still unlocks!
 *       // ... more stuff ...
 *   }
 *
 * This is EXACTLY how std::lock_guard works in real code.
 * ============================================================
 *
 * YOUR TASK 3c:
 * Intentionally write a class with a destructor but NO copy
 * constructor. Create two objects, copy one to the other, and
 * watch it crash (double free). Then fix it with a proper copy
 * constructor. This teaches you WHY the Rule of Three exists.
 * ============================================================
 *
 * INTERVIEW TIP: "What is RAII?"
 * "RAII ties resource lifetime to object lifetime. You acquire
 * in the constructor and release in the destructor. This means
 * resources are always cleaned up, even during exceptions or
 * early returns. It prevents leaks and makes code exception-safe."
 * ============================================================
 */
