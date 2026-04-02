/*
 * ============================================================
 * EXERCISE 6: Polymorphism & Virtual Functions
 * ============================================================
 * GOAL: This is the BIG ONE. Polymorphism is what makes OOP
 * truly powerful. It means: "one interface, many implementations."
 *
 * You call sensor->read() and the RIGHT read() runs depending
 * on whether it's a GPS, IMU, or barometer — decided at RUNTIME.
 *
 * This is the C++ equivalent of function pointers in C, but
 * safer and more organized.
 *
 * Compile: g++ -std=c++17 -Wall -o ex06 06_polymorphism.cpp
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ============================================================
// PART 1: The problem WITHOUT polymorphism
// ============================================================

/*
 * Without virtual functions, the compiler decides which method to
 * call based on the POINTER TYPE, not the actual object:
 *
 *   Sensor *s = new GPSSensor();
 *   s->read();   // calls Sensor::read(), NOT GPSSensor::read()
 *                 // because the POINTER type is Sensor*
 *
 * This is "static dispatch" — decided at compile time.
 * It's WRONG when you want different behavior per subclass.
 */

// ============================================================
// PART 2: Virtual functions — dynamic dispatch
// ============================================================

class Sensor {
protected:
    string name;
    double value;

public:
    Sensor(const string &name) : name(name), value(0) {
        cout << "  Sensor(" << name << ") created" << endl;
    }

    // VIRTUAL keyword enables dynamic dispatch
    // The correct version is called based on the OBJECT type,
    // not the pointer type
    virtual void read() {
        value = 0;
        cout << "  Sensor::read() — base implementation" << endl;
    }

    virtual void print() const {
        cout << "  [" << name << "] value=" << value << endl;
    }

    // ALWAYS make base class destructors virtual when you have
    // virtual methods. Otherwise, deleting through a base pointer
    // won't call the derived destructor = resource leak.
    virtual ~Sensor() {
        cout << "  ~Sensor(" << name << ")" << endl;
    }

    string getName() const { return name; }
    double getValue() const { return value; }
};

class GPSSensor : public Sensor {
private:
    double latitude, longitude;

public:
    GPSSensor() : Sensor("GPS"), latitude(0), longitude(0) {}

    // 'override' keyword tells the compiler: "I intend to override
    // a virtual function." If the base doesn't have a matching
    // virtual, you get a compile error. ALWAYS use override.
    void read() override {
        latitude = 34.0522;     // simulate GPS reading (Los Angeles)
        longitude = -118.2437;
        value = latitude;
        cout << "  GPS::read() — got coordinates" << endl;
    }

    void print() const override {
        cout << "  [GPS] lat=" << latitude << " lon=" << longitude << endl;
    }

    ~GPSSensor() override {
        cout << "  ~GPSSensor()" << endl;
    }
};

class IMUSensor : public Sensor {
private:
    double accel_x, accel_y, accel_z;

public:
    IMUSensor() : Sensor("IMU"), accel_x(0), accel_y(0), accel_z(0) {}

    void read() override {
        accel_x = 0.01;
        accel_y = -0.02;
        accel_z = 9.81;     // gravity
        value = accel_z;
        cout << "  IMU::read() — got acceleration" << endl;
    }

    void print() const override {
        cout << "  [IMU] accel=(" << accel_x << ", " << accel_y
             << ", " << accel_z << ")" << endl;
    }

    ~IMUSensor() override {
        cout << "  ~IMUSensor()" << endl;
    }
};

class BarometerSensor : public Sensor {
private:
    double pressure_hpa;

public:
    BarometerSensor() : Sensor("Barometer"), pressure_hpa(0) {}

    void read() override {
        pressure_hpa = 1013.25;
        value = pressure_hpa;
        cout << "  Barometer::read() — got pressure" << endl;
    }

    void print() const override {
        double altitude = 44330.0 * (1.0 - pow(pressure_hpa / 1013.25, 0.1903));
        cout << "  [Barometer] " << pressure_hpa << " hPa (alt="
             << altitude << "m)" << endl;
    }

    ~BarometerSensor() override {
        cout << "  ~BarometerSensor()" << endl;
    }
};

// ============================================================
// PART 3: The magic — using base class pointers
// ============================================================

// This function doesn't know or CARE what kind of sensor it gets.
// It just knows it has a Sensor* and can call read() and print().
// The right version runs automatically.
void readAndDisplay(Sensor *s) {
    cout << "  Reading " << s->getName() << "..." << endl;
    s->read();    // calls the RIGHT read() — GPS, IMU, or Barometer
    s->print();   // calls the RIGHT print()
}

void polymorphism_demo() {
    cout << "\n=== Polymorphism in Action ===" << endl;

    // Create different sensors
    GPSSensor gps;
    IMUSensor imu;
    BarometerSensor baro;

    // Pass them all to the SAME function through base pointer
    cout << "\n--- Reading all sensors ---" << endl;
    readAndDisplay(&gps);
    readAndDisplay(&imu);
    readAndDisplay(&baro);

    // Store different types in one array — this is the power
    cout << "\n--- Array of mixed sensors ---" << endl;
    Sensor *sensors[] = {&gps, &imu, &baro};

    for (int i = 0; i < 3; i++) {
        sensors[i]->read();
        sensors[i]->print();
    }
}

// ============================================================
// PART 4: Without 'virtual' — to see the difference
// ============================================================

class Base {
public:
    // NOT virtual
    void speak() { cout << "  Base speaking" << endl; }

    // Virtual
    virtual void shout() { cout << "  BASE SHOUTING" << endl; }

    virtual ~Base() {}
};

class Derived : public Base {
public:
    void speak() { cout << "  Derived speaking" << endl; }    // hides Base::speak
    void shout() override { cout << "  DERIVED SHOUTING" << endl; }
};

void virtual_vs_nonvirtual() {
    cout << "\n=== Virtual vs Non-Virtual ===" << endl;

    Derived d;
    Base *bp = &d;    // base pointer to derived object

    cout << "Through Base pointer:" << endl;
    bp->speak();      // Non-virtual: calls Base::speak()     ← WRONG behavior
    bp->shout();      // Virtual: calls Derived::shout()       ← RIGHT behavior

    cout << "\nDirectly on Derived:" << endl;
    d.speak();        // calls Derived::speak()
    d.shout();        // calls Derived::shout()

    // KEY LESSON: Without 'virtual', the compiler uses the POINTER type.
    // With 'virtual', the runtime uses the OBJECT type.
}

// ============================================================
// PART 5: How it works under the hood — vtable
// ============================================================

/*
 * HOW VIRTUAL DISPATCH WORKS:
 *
 * When a class has virtual functions, the compiler creates a
 * VTABLE (virtual function table) — an array of function pointers.
 * Each object gets a hidden pointer (vptr) to its class's vtable.
 *
 *   GPSSensor object:
 *   ┌──────────┐
 *   │ vptr ──────────→ GPSSensor vtable:
 *   │ name     │        ┌─────────────────┐
 *   │ value    │        │ read() → GPS::read
 *   │ lat      │        │ print()→ GPS::print
 *   │ lon      │        │ ~dtor()→ GPS::~GPS
 *   └──────────┘        └─────────────────┘
 *
 *   IMUSensor object:
 *   ┌──────────┐
 *   │ vptr ──────────→ IMUSensor vtable:
 *   │ name     │        ┌─────────────────────┐
 *   │ value    │        │ read() → IMU::read
 *   │ accel_x  │        │ print()→ IMU::print
 *   │ accel_y  │        │ ~dtor()→ IMU::~IMU
 *   │ accel_z  │        └─────────────────────┘
 *   └──────────┘
 *
 * When you call s->read():
 *   1. Follow the vptr to the vtable
 *   2. Look up the read() entry
 *   3. Call that function pointer
 *
 * This is EXACTLY like the function pointer jump table from
 * Exercise 5 in the C guide — but the compiler builds it for you!
 *
 * COST: Each object is sizeof(void*) bytes larger (the vptr).
 * Each virtual call has one extra indirection (pointer lookup).
 * In embedded, this matters if you have millions of objects or
 * nanosecond-critical code paths.
 */

// ============================================================
// PART 6: Why virtual destructors matter
// ============================================================

void destructor_demo() {
    cout << "\n=== Virtual Destructor Demo ===" << endl;

    cout << "Deleting through base pointer:" << endl;
    Sensor *s = new GPSSensor();
    delete s;
    // Because ~Sensor() is virtual, this calls:
    //   1. ~GPSSensor()
    //   2. ~Sensor()
    // If ~Sensor() were NOT virtual, only ~Sensor() would run,
    // leaking any resources GPSSensor allocated.
}

// ============================================================

int main() {
    polymorphism_demo();
    virtual_vs_nonvirtual();
    destructor_demo();

    cout << "\n=== Cleanup ===" << endl;
    return 0;
}

/*
 * ============================================================
 * YOUR TASK 6a:
 * Add a new sensor type: UltrasonicSensor
 *   - Measures distance in cm
 *   - Override read() to simulate a distance reading
 *   - Override print() to show distance
 *   - Add it to the sensors[] array in polymorphism_demo
 *
 * Notice: you didn't need to change readAndDisplay() or the
 * loop. THAT'S the power of polymorphism — new types work
 * automatically with existing code.
 * ============================================================
 *
 * YOUR TASK 6b:
 * Write a function:
 *   Sensor* findClosest(Sensor **sensors, int count, double target)
 * that finds the sensor whose last reading is closest to a
 * target value. It works on ANY sensor type through polymorphism.
 * ============================================================
 *
 * YOUR TASK 6c:
 * Create a Shape hierarchy:
 *   Shape (base) — virtual: area(), perimeter(), print()
 *     ├── Circle (radius)
 *     ├── Rectangle (width, height)
 *     └── Triangle (a, b, c sides)
 *
 * Store them in a vector<Shape*>. Loop through and print all.
 * Calculate total area. This is a textbook OOP exercise but
 * it really hammers home how polymorphism works.
 * ============================================================
 *
 * YOUR TASK 6d:
 * Draw the vtable for your Shape hierarchy on paper.
 * Show the vptr in each object pointing to the right vtable.
 * This is what interviewers want when they ask "how do virtual
 * functions work?"
 * ============================================================
 *
 * INTERVIEW TIP: "How do virtual functions work?"
 * "Each class with virtual functions gets a vtable — an array
 * of function pointers. Each object has a hidden vptr pointing
 * to its class's vtable. When you call a virtual function through
 * a base pointer, the runtime follows the vptr to the vtable and
 * calls the correct function. It's essentially compiler-generated
 * function pointer dispatch."
 * ============================================================
 */
