/*
 * ============================================================
 * EXERCISE 7: Abstract Classes & Interfaces
 * ============================================================
 * GOAL: Understand pure virtual functions (= 0), abstract classes
 * (can't instantiate), and how C++ does interfaces.
 *
 * ABSTRACT CLASS: Has at least one pure virtual function.
 *   - You can NOT create objects of this class.
 *   - You CAN have pointers/references to it.
 *   - Children MUST implement all pure virtual methods.
 *   - It defines a CONTRACT that all children must follow.
 *
 * INTERFACE: An abstract class with ONLY pure virtual functions
 *   and no data members. C++ doesn't have a separate keyword
 *   for interfaces (unlike Java) — you just use abstract classes.
 *
 * Compile: g++ -std=c++17 -Wall -o ex07 07_abstract_interfaces.cpp
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

// ============================================================
// PART 1: Interface — pure contract, no implementation
// ============================================================

// This is a C++ "interface" — all pure virtual
class IReadable {
public:
    virtual double read() = 0;           // = 0 makes it PURE virtual
    virtual string getUnit() const = 0;
    virtual ~IReadable() {}              // virtual destructor still needed
};

// Another interface
class ICalibrateable {
public:
    virtual void calibrate(double offset) = 0;
    virtual bool isCalibrated() const = 0;
    virtual ~ICalibrateable() {}
};

// ============================================================
// PART 2: Abstract base class — some implementation, some not
// ============================================================

// Abstract class: provides SOME implementation, forces children
// to implement the rest
class SensorBase : public IReadable, public ICalibrateable {
protected:
    string name;
    double value;
    double cal_offset;
    bool calibrated;

public:
    SensorBase(const string &name)
        : name(name), value(0), cal_offset(0), calibrated(false) {}

    virtual ~SensorBase() {}

    // Implement SOME interface methods here
    void calibrate(double offset) override {
        cal_offset = offset;
        calibrated = true;
        cout << "  " << name << " calibrated with offset " << offset << endl;
    }

    bool isCalibrated() const override { return calibrated; }

    // read() and getUnit() are still pure virtual — children MUST implement
    // We don't implement them here because each sensor reads differently

    // Concrete method (not virtual)
    void printInfo() const {
        cout << "  [" << name << "] " << value << " " << getUnit()
             << (calibrated ? " (cal)" : " (uncal)") << endl;
    }
};

// ============================================================
// PART 3: Concrete classes — implement everything
// ============================================================

class Thermometer : public SensorBase {
public:
    Thermometer(const string &name) : SensorBase(name) {}

    double read() override {
        // Simulate reading
        value = 22.5 + cal_offset;
        return value;
    }

    string getUnit() const override { return "°C"; }
};

class Accelerometer : public SensorBase {
private:
    double x, y, z;

public:
    Accelerometer(const string &name) : SensorBase(name), x(0), y(0), z(0) {}

    double read() override {
        x = 0.01;
        y = -0.02;
        z = 9.81 + cal_offset;
        value = z;
        return value;
    }

    string getUnit() const override { return "m/s²"; }

    // Extra method specific to accelerometer
    void readAxes(double &ox, double &oy, double &oz) const {
        ox = x; oy = y; oz = z;
    }
};

class Voltmeter : public SensorBase {
public:
    Voltmeter(const string &name) : SensorBase(name) {}

    double read() override {
        value = 3.3 + cal_offset;  // simulate 3.3V rail
        return value;
    }

    string getUnit() const override { return "V"; }
};

// ============================================================
// PART 4: Programming to interfaces
// ============================================================

// This function works with ANY IReadable — it doesn't care
// about the concrete type. This is "programming to an interface."
void readAllSensors(vector<IReadable*> &sensors) {
    cout << "\n  Reading all sensors:" << endl;
    for (auto s : sensors) {
        double val = s->read();
        cout << "    Got " << val << " " << s->getUnit() << endl;
    }
}

// This function works with ANY ICalibrateable
void calibrateAll(vector<ICalibrateable*> &devices, double offset) {
    cout << "\n  Calibrating all devices:" << endl;
    for (auto d : devices) {
        d->calibrate(offset);
    }
}

// ============================================================
// PART 5: Multiple Interfaces — a class can implement many
// ============================================================

class ILoggable {
public:
    virtual string toLogString() const = 0;
    virtual ~ILoggable() {}
};

class IPowerManaged {
public:
    virtual void sleep() = 0;
    virtual void wake() = 0;
    virtual double getPowerDraw() const = 0;
    virtual ~IPowerManaged() {}
};

// A "smart" sensor implements multiple interfaces
class SmartThermometer : public SensorBase, public ILoggable, public IPowerManaged {
private:
    bool sleeping;
    double power_mw;

public:
    SmartThermometer(const string &name)
        : SensorBase(name), sleeping(false), power_mw(5.0) {}

    // IReadable (through SensorBase)
    double read() override {
        if (sleeping) {
            cout << "  " << name << " is sleeping! Wake it first." << endl;
            return 0;
        }
        value = 23.1 + cal_offset;
        return value;
    }

    string getUnit() const override { return "°C"; }

    // ILoggable
    string toLogString() const override {
        return name + "," + to_string(value) + "," + getUnit();
    }

    // IPowerManaged
    void sleep() override {
        sleeping = true;
        power_mw = 0.01;   // micro-amps in sleep
        cout << "  " << name << " entering sleep mode" << endl;
    }

    void wake() override {
        sleeping = false;
        power_mw = 5.0;
        cout << "  " << name << " waking up" << endl;
    }

    double getPowerDraw() const override { return power_mw; }
};

// ============================================================

int main() {
    cout << "=== Abstract Classes & Interfaces ===" << endl;

    // Can't do this — SensorBase is abstract:
    // SensorBase sb("nope");  // COMPILE ERROR

    // But you CAN create concrete children:
    Thermometer therm("Engine Temp");
    Accelerometer accel("IMU");
    Voltmeter volt("Battery");

    // Use through base class
    cout << "\n--- Through SensorBase pointer ---" << endl;
    SensorBase *sensors[] = {&therm, &accel, &volt};
    for (auto s : sensors) {
        s->read();
        s->printInfo();
    }

    // Use through interface
    cout << "\n--- Through IReadable interface ---" << endl;
    vector<IReadable*> readables = {&therm, &accel, &volt};
    readAllSensors(readables);

    // Calibrate through interface
    vector<ICalibrateable*> calibrateables = {&therm, &accel, &volt};
    calibrateAll(calibrateables, -0.5);

    // Read again after calibration
    readAllSensors(readables);

    // Smart sensor with multiple interfaces
    cout << "\n=== Multi-Interface Smart Sensor ===" << endl;
    SmartThermometer smart("Smart BME280");
    smart.read();
    smart.printInfo();

    cout << "Log: " << smart.toLogString() << endl;
    cout << "Power: " << smart.getPowerDraw() << " mW" << endl;

    smart.sleep();
    cout << "Power (sleeping): " << smart.getPowerDraw() << " mW" << endl;
    smart.read();  // should warn — sleeping!

    smart.wake();
    smart.read();
    smart.printInfo();

    return 0;
}

/*
 * ============================================================
 * YOUR TASK 7a:
 * Create an interface ISerializable with:
 *   virtual string serialize() const = 0;
 *   virtual void deserialize(const string &data) = 0;
 *
 * Implement it on Thermometer so you can save/load its state
 * as a simple string like "22.5,true" (value, calibrated).
 * ============================================================
 *
 * YOUR TASK 7b:
 * Create a MotorController interface:
 *   virtual void setSpeed(int percent) = 0;   // 0-100
 *   virtual void stop() = 0;
 *   virtual int getSpeed() const = 0;
 *   virtual bool isFault() const = 0;
 *
 * Implement it for:
 *   - BrushlessMotor (speed changes instantly)
 *   - StepperMotor (speed changes in steps of 10)
 *   - ServoMotor (speed is actually angle: 0-180)
 *
 * Write a function: void rampUp(MotorController *m) that
 * gradually increases speed from 0 to 100. It works on ALL
 * motor types without knowing which one it has.
 * ============================================================
 *
 * YOUR TASK 7c:
 * Try to instantiate SensorBase directly. Read the error message.
 * Then try creating a child that doesn't implement read().
 * Read that error message too. Understanding compiler errors for
 * abstract classes is important.
 * ============================================================
 *
 * INTERVIEW TIP: "Abstract class vs Interface?"
 * "An abstract class can have some implementation and data members.
 * An interface (in C++ terms) is an abstract class with only pure
 * virtual functions and no data. Use interfaces to define contracts.
 * Use abstract classes when you want to share common implementation
 * among children."
 * ============================================================
 */
