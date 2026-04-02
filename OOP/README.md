# Object-Oriented Programming Study Guide
## From C to C++ — A Complete Roadmap

---

## WHO THIS IS FOR

You know C. You understand pointers, structs, memory layout.
Now you need to understand OOP — not just "what is a class" but
WHY these concepts exist and HOW they solve real engineering problems.

We use C++ because it's the natural bridge from C, and it's
heavily used in embedded/robotics/drones (including companies like Red Cat).

---

## STUDY SCHEDULE (2 weeks, ~2 hours/day)

### Week 1: Core OOP

| Day | Exercise | Concepts |
|-----|----------|----------|
| 1 | 01, 02 | Classes vs Structs, Encapsulation |
| 2 | 03 | Constructors, Destructors, RAII |
| 3 | 04 | Operator Overloading |
| 4 | 05 | Inheritance |
| 5 | 06 | Polymorphism & Virtual Functions |
| 6 | 07 | Abstract Classes & Interfaces |
| 7 | Review | Re-do any tasks you struggled with |

### Week 2: Advanced & Applied

| Day | Exercise | Concepts |
|-----|----------|----------|
| 8 | 08 | Templates & Generics |
| 9 | 09 | Smart Pointers & Memory Management |
| 10 | 10 | Design Patterns (Observer, Strategy, Singleton) |
| 11 | 11 | Capstone: Build a Drone Sensor System |
| 12 | 12 | SOLID Principles |
| 13 | Verbal | Practice explaining every concept out loud |
| 14 | Light | Review cheat sheet, rest |

---

## QUICK REFERENCE CHEAT SHEET

### The 4 Pillars of OOP
```
ENCAPSULATION:   Bundle data + behavior. Hide internals.
                 "A Motor object knows how to spin. You don't
                  need to know how it manages PWM internally."

ABSTRACTION:     Expose only what matters. Hide complexity.
                 "You call motor.setSpeed(50). You don't care
                  if it's brushless, stepper, or servo underneath."

INHERITANCE:     Build new classes from existing ones. Reuse code.
                 "A QuadcopterMotor IS-A Motor. It inherits
                  everything Motor can do, and adds 4-motor coordination."

POLYMORPHISM:    Same interface, different behavior.
                 "sensor.read() works on GPS, IMU, and Barometer.
                  Each one reads differently, but the interface is the same."
```

### Class Anatomy
```cpp
class Motor {
private:              // Only Motor's own methods can access
    int speed;
    bool enabled;

protected:            // Motor AND its children can access
    int max_rpm;

public:               // Anyone can access
    Motor(int max);           // Constructor
    ~Motor();                 // Destructor
    void setSpeed(int s);     // Method
    int getSpeed() const;     // const method (promises not to modify)

    virtual void spin() = 0;  // Pure virtual (abstract — must override)
};
```

### Inheritance Syntax
```cpp
class Base { };
class Derived : public Base { };    // IS-A relationship
// public:    Base's public stays public in Derived
// protected: Base's public becomes protected in Derived
// private:   Base's public becomes private in Derived (rarely used)
```

### Virtual Function Rules
```
- Non-virtual:  Compiler decides at COMPILE time which function to call
                (based on pointer/reference TYPE)
- Virtual:      Runtime decides which function to call
                (based on actual OBJECT type)
- Pure virtual: virtual void f() = 0;  — makes class abstract,
                forces children to implement
- Always make destructors virtual in base classes with virtual methods
```

### Smart Pointers (Modern C++)
```cpp
unique_ptr<T>    One owner. Can't copy, can move. Use by default.
shared_ptr<T>    Reference counted. Multiple owners. Slight overhead.
weak_ptr<T>      Non-owning reference to shared_ptr. Breaks cycles.

// Old way (don't do this):
Motor *m = new Motor();    // who deletes this?
delete m;                  // hope you don't forget

// New way:
auto m = make_unique<Motor>();  // automatically deleted when out of scope
```

### Design Pattern Quick Hits
```
STRATEGY:     Swap algorithms at runtime via interface/function pointer
OBSERVER:     "Hey everyone, something changed" — pub/sub pattern
SINGLETON:    One global instance (use sparingly — often a code smell)
FACTORY:      Create objects without specifying exact class
STATE:        Object changes behavior when internal state changes
```

### SOLID Principles
```
S — Single Responsibility:  A class should do ONE thing
O — Open/Closed:           Open for extension, closed for modification
L — Liskov Substitution:   Derived class must be usable wherever Base is
I — Interface Segregation:  Many small interfaces > one fat interface
D — Dependency Inversion:   Depend on abstractions, not concrete classes
```

### Key Differences: C Struct vs C++ Class
```
C struct:                           C++ class:
  - Only data                         - Data AND functions together
  - No access control                 - private/protected/public
  - No constructors                   - Automatic initialization
  - No destructors                    - Automatic cleanup
  - No inheritance                    - IS-A relationships
  - No virtual functions              - Polymorphism
  - Pass-by-pointer for "methods"     - this pointer is implicit
  - Manual function dispatch          - Virtual dispatch table (vtable)
```

### Compile Commands
```bash
# Basic compile
g++ -std=c++17 -Wall -Wextra -o program file.cpp

# With optimization (to see how virtual dispatch works)
g++ -std=c++17 -O2 -Wall -o program file.cpp

# Multiple files
g++ -std=c++17 -Wall -o program file1.cpp file2.cpp
```
