/*
 * ============================================================
 * EXERCISE 5: Inheritance — Building on What Exists
 * ============================================================
 * GOAL: Understand IS-A relationships and code reuse through
 * inheritance. A BrushlessMotor IS-A Motor. A GPSSensor IS-A
 * Sensor. The child class inherits everything from the parent
 * and can add or modify behavior.
 *
 * Compile: g++ -std=c++17 -Wall -o ex05 05_inheritance.cpp
 * ============================================================
 */

#include <iostream>
#include <string>
using namespace std;

// ============================================================
// PART 1: Basic Inheritance
// ============================================================

// BASE CLASS (parent)
class Sensor {
protected:        // accessible by this class AND children
    string name;
    int id;
    double last_reading;
    bool calibrated;

public:
    Sensor(const string &name, int id)
        : name(name), id(id), last_reading(0), calibrated(false) {
        cout << "  Sensor constructor: " << name << endl;
    }

    virtual ~Sensor() {
        cout << "  Sensor destructor: " << name << endl;
    }

    void calibrate() {
        calibrated = true;
        cout << "  " << name << " calibrated." << endl;
    }

    double getLastReading() const { return last_reading; }
    string getName() const { return name; }
    bool isCalibrated() const { return calibrated; }

    void printBase() const {
        cout << "  [" << name << " #" << id << "] reading="
             << last_reading << " cal=" << (calibrated ? "Y" : "N") << endl;
    }
};

// DERIVED CLASS (child) — inherits everything from Sensor
class TemperatureSensor : public Sensor {
private:
    double offset;       // calibration offset — specific to temp sensors
    string unit;

public:
    // Call the parent constructor with : Sensor(name, id)
    TemperatureSensor(const string &name, int id, const string &unit = "C")
        : Sensor(name, id), offset(0.0), unit(unit) {
        cout << "  TemperatureSensor constructor: " << name << endl;
    }

    ~TemperatureSensor() {
        cout << "  TemperatureSensor destructor: " << name << endl;
    }

    void setOffset(double off) { offset = off; }

    void readTemperature(double raw) {
        last_reading = raw + offset;  // can access 'protected' member from parent
    }

    void print() const {
        cout << "  TEMP [" << name << "] " << last_reading << "°" << unit;
        if (calibrated) cout << " (calibrated, offset=" << offset << ")";
        cout << endl;
    }
};

// Another derived class
class PressureSensor : public Sensor {
private:
    double altitude_offset;

public:
    PressureSensor(const string &name, int id)
        : Sensor(name, id), altitude_offset(0.0) {
        cout << "  PressureSensor constructor: " << name << endl;
    }

    ~PressureSensor() {
        cout << "  PressureSensor destructor: " << name << endl;
    }

    void readPressure(double hPa) {
        last_reading = hPa;
    }

    // Calculate altitude from pressure — specific to pressure sensors
    double getAltitude() const {
        // Simplified barometric formula
        return 44330.0 * (1.0 - pow(last_reading / 1013.25, 0.1903));
    }

    void print() const {
        cout << "  PRES [" << name << "] " << last_reading << " hPa"
             << " (altitude: " << getAltitude() << " m)" << endl;
    }
};

// ============================================================
// PART 2: Constructor/Destructor Order
// ============================================================

void constructor_order_demo() {
    cout << "\n=== Constructor Order (Base first, then Derived) ===" << endl;
    TemperatureSensor ts("Thermocouple", 1);
    cout << "\n=== Destructor Order (Derived first, then Base) ===" << endl;
}
// Notice: Constructors fire BASE → DERIVED
//         Destructors fire  DERIVED → BASE

// ============================================================
// PART 3: What gets inherited
// ============================================================

void inheritance_demo() {
    cout << "\n=== Inheritance Demo ===" << endl;

    TemperatureSensor temp("Engine Temp", 1);
    PressureSensor pres("Barometer", 2);

    // Methods from BASE class work on derived objects
    temp.calibrate();           // inherited from Sensor
    pres.calibrate();           // inherited from Sensor

    // Methods specific to each derived class
    temp.setOffset(-1.5);
    temp.readTemperature(25.0);
    temp.print();

    pres.readPressure(1013.25);
    pres.print();

    // Inherited methods still work
    cout << "\n  Base class info:" << endl;
    temp.printBase();           // inherited from Sensor
    pres.printBase();           // inherited from Sensor
}

// ============================================================
// PART 4: Multi-level inheritance
// ============================================================

class I2CSensor : public TemperatureSensor {
private:
    int i2c_address;

public:
    I2CSensor(const string &name, int id, int addr)
        : TemperatureSensor(name, id), i2c_address(addr) {
        cout << "  I2CSensor constructor: " << name
             << " at addr 0x" << hex << addr << dec << endl;
    }

    ~I2CSensor() {
        cout << "  I2CSensor destructor: " << name << endl;
    }

    void readFromBus() {
        // Simulate reading from I2C bus
        double fake_raw = 23.5;  // pretend we read this from hardware
        readTemperature(fake_raw);  // inherited from TemperatureSensor
        cout << "  Read " << fake_raw << " from I2C addr 0x"
             << hex << i2c_address << dec << endl;
    }
};

// Chain: Sensor → TemperatureSensor → I2CSensor
// I2CSensor has EVERYTHING from both parents

// ============================================================
// PART 5: When NOT to use inheritance
// ============================================================

/*
 * WRONG: class Car : public Engine { }
 *   A Car is NOT an Engine. A Car HAS an Engine.
 *   Use COMPOSITION instead:
 *
 *   class Car {
 *   private:
 *       Engine engine;    // Car HAS-A Engine
 *       Wheel wheels[4];  // Car HAS 4 Wheels
 *   };
 *
 * RULE: Use inheritance for IS-A relationships.
 *       Use composition for HAS-A relationships.
 *
 * A drone example:
 *   class Drone {
 *       Motor motors[4];          // HAS-A (composition)
 *       IMUSensor imu;            // HAS-A
 *       GPSSensor gps;            // HAS-A
 *       BatteryMonitor battery;   // HAS-A
 *   };
 *
 *   class QuadcopterDrone : public Drone { };  // IS-A (inheritance)
 */

// ============================================================

int main() {
    constructor_order_demo();
    inheritance_demo();

    cout << "\n=== Multi-level Inheritance ===" << endl;
    I2CSensor i2c_temp("BME280", 3, 0x76);
    i2c_temp.calibrate();          // from Sensor (grandparent!)
    i2c_temp.setOffset(-0.5);      // from TemperatureSensor (parent)
    i2c_temp.readFromBus();        // own method
    i2c_temp.print();              // from TemperatureSensor

    cout << "\n=== Cleanup ===" << endl;
    return 0;
}

/*
 * ============================================================
 * YOUR TASK 5a:
 * Create a class hierarchy:
 *   Vehicle (base) — has: name, max_speed, current_speed
 *     ├── GroundVehicle — adds: num_wheels
 *     │     └── Car — adds: trunk_capacity
 *     └── AirVehicle — adds: max_altitude, current_altitude
 *           └── Drone — adds: num_rotors, battery_percent
 *
 * Each class should have a constructor that chains to its parent,
 * a print() method, and a destructor that prints a message.
 * Create one of each and call methods from different levels.
 * ============================================================
 *
 * YOUR TASK 5b:
 * Refactor the Drone class to use COMPOSITION instead of inheritance:
 *   class Drone {
 *       Motor motors[4];
 *       Battery battery;
 *       GPSSensor gps;
 *   };
 * Is this cleaner? When would inheritance be better?
 * Write a comment explaining your reasoning.
 * ============================================================
 *
 * YOUR TASK 5c:
 * Create a Logger base class with a log(string message) method.
 * Derive ConsoleLogger (prints to cout) and FileLogger (prints
 * to a file using RAII). Demonstrate that both "are" Loggers.
 * ============================================================
 *
 * INTERVIEW TIP: "Inheritance vs Composition?"
 * "Inheritance models IS-A relationships: a Dog IS-A Animal.
 * Composition models HAS-A: a Car HAS an Engine. Prefer
 * composition when possible — it's more flexible and avoids
 * deep inheritance hierarchies. Use inheritance when you need
 * polymorphism."
 * ============================================================
 */
