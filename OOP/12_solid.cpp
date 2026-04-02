/*
 * ============================================================
 * EXERCISE 12: SOLID Principles — Writing Maintainable Code
 * ============================================================
 * GOAL: Understand the 5 principles that separate "code that
 * works" from "code that scales." These come up in senior
 * interviews and code reviews.
 *
 * S — Single Responsibility
 * O — Open/Closed
 * L — Liskov Substitution
 * I — Interface Segregation
 * D — Dependency Inversion
 *
 * Compile: g++ -std=c++17 -Wall -o ex12 12_solid.cpp
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

// ============================================================
// S — SINGLE RESPONSIBILITY PRINCIPLE
// "A class should have only one reason to change."
// ============================================================

// BAD: This class does THREE things
class GodMotor_Bad {
public:
    void spinMotor() { /* PWM control */ }
    void logRPM() { /* write to SD card */ }
    void sendTelemetry() { /* transmit over radio */ }
    // If logging format changes, this class changes.
    // If radio protocol changes, this class changes.
    // If motor control changes, this class changes.
    // Three reasons to change = violation.
};

// GOOD: Each class does ONE thing
class Motor_Good {
public:
    void spin(int pwm) {
        cout << "  Motor spinning at PWM " << pwm << endl;
    }
    int getRPM() const { return 5000; }
};

class MotorLogger {
public:
    void log(const Motor_Good &motor) {
        cout << "  Logging RPM: " << motor.getRPM() << endl;
    }
};

class MotorTelemetry {
public:
    void send(const Motor_Good &motor) {
        cout << "  Sending telemetry: RPM=" << motor.getRPM() << endl;
    }
};

void srp_demo() {
    cout << "=== S: Single Responsibility ===" << endl;
    Motor_Good motor;
    MotorLogger logger;
    MotorTelemetry telemetry;

    motor.spin(200);
    logger.log(motor);
    telemetry.send(motor);
    // Each class has ONE reason to change. Clean.
}

// ============================================================
// O — OPEN/CLOSED PRINCIPLE
// "Open for extension, closed for modification."
// ============================================================

// BAD: Adding a new sensor requires modifying this function
void readSensor_Bad(const string &type) {
    if (type == "gps") { cout << "Reading GPS" << endl; }
    else if (type == "imu") { cout << "Reading IMU" << endl; }
    // Every new sensor = modify this function. Violation.
}

// GOOD: Add new sensors by creating new classes, not modifying existing code
class ISensor_OCP {
public:
    virtual void read() = 0;
    virtual ~ISensor_OCP() {}
};

class GPS_OCP : public ISensor_OCP {
public:
    void read() override { cout << "  GPS reading coordinates" << endl; }
};

class IMU_OCP : public ISensor_OCP {
public:
    void read() override { cout << "  IMU reading acceleration" << endl; }
};

// New sensor? Just add a class. readAll never changes.
class Lidar_OCP : public ISensor_OCP {
public:
    void read() override { cout << "  Lidar scanning environment" << endl; }
};

void readAll(vector<ISensor_OCP*> &sensors) {
    for (auto s : sensors) s->read();
    // This function is CLOSED for modification — it never changes.
    // But it's OPEN for extension — any new ISensor works automatically.
}

void ocp_demo() {
    cout << "\n=== O: Open/Closed ===" << endl;
    GPS_OCP gps;
    IMU_OCP imu;
    Lidar_OCP lidar;  // new! no existing code modified.
    vector<ISensor_OCP*> sensors = {&gps, &imu, &lidar};
    readAll(sensors);
}

// ============================================================
// L — LISKOV SUBSTITUTION PRINCIPLE
// "If B is a subtype of A, you should be able to use B
//  everywhere A is used without breaking anything."
// ============================================================

// BAD: Square inherits from Rectangle but breaks expectations
class Rectangle_Bad {
protected:
    int width, height;
public:
    Rectangle_Bad(int w, int h) : width(w), height(h) {}
    virtual void setWidth(int w) { width = w; }
    virtual void setHeight(int h) { height = h; }
    int area() const { return width * height; }
};

class Square_Bad : public Rectangle_Bad {
public:
    Square_Bad(int size) : Rectangle_Bad(size, size) {}
    void setWidth(int w) override { width = height = w; }   // FORCES both
    void setHeight(int h) override { width = height = h; }  // FORCES both
};

// This function expects Rectangle behavior but Square breaks it:
void testRectangle(Rectangle_Bad &r) {
    r.setWidth(5);
    r.setHeight(10);
    cout << "  Expected area 50, got: " << r.area() << endl;
    // With Square, this gives 100 (10*10), not 50!
}

// GOOD: Don't force Square to be a Rectangle
class Shape_Good {
public:
    virtual int area() const = 0;
    virtual ~Shape_Good() {}
};

class Rectangle_Good : public Shape_Good {
    int w, h;
public:
    Rectangle_Good(int w, int h) : w(w), h(h) {}
    int area() const override { return w * h; }
};

class Square_Good : public Shape_Good {
    int side;
public:
    Square_Good(int s) : side(s) {}
    int area() const override { return side * side; }
};

void lsp_demo() {
    cout << "\n=== L: Liskov Substitution ===" << endl;
    Rectangle_Bad rect(5, 10);
    Square_Bad square(5);

    cout << "  Rectangle:" << endl;
    testRectangle(rect);      // works: 50
    cout << "  Square (broken):" << endl;
    testRectangle(square);    // BROKEN: gives 100, not 50

    cout << "  Correct approach — separate types:" << endl;
    Rectangle_Good rg(5, 10);
    Square_Good sg(7);
    cout << "  Rect area: " << rg.area() << ", Square area: " << sg.area() << endl;
}

// ============================================================
// I — INTERFACE SEGREGATION PRINCIPLE
// "Clients shouldn't be forced to depend on methods they don't use."
// ============================================================

// BAD: One fat interface forces all implementers to define everything
class IDevice_Bad {
public:
    virtual void read() = 0;
    virtual void write(int val) = 0;
    virtual void calibrate() = 0;
    virtual void selfTest() = 0;
    virtual void sleep() = 0;
    virtual void wake() = 0;
    virtual ~IDevice_Bad() {}
    // A simple read-only sensor must implement write(), sleep(), etc.
    // even if they don't apply. Violation.
};

// GOOD: Small, focused interfaces
class IReadable_ISP {
public:
    virtual double read() = 0;
    virtual ~IReadable_ISP() {}
};

class IWritable_ISP {
public:
    virtual void write(int val) = 0;
    virtual ~IWritable_ISP() {}
};

class ICalibrateable_ISP {
public:
    virtual void calibrate() = 0;
    virtual ~ICalibrateable_ISP() {}
};

class IPowerManaged_ISP {
public:
    virtual void sleep() = 0;
    virtual void wake() = 0;
    virtual ~IPowerManaged_ISP() {}
};

// Simple sensor only implements what it needs
class SimpleSensor : public IReadable_ISP {
public:
    double read() override {
        cout << "  SimpleSensor: reading" << endl;
        return 42.0;
    }
};

// Complex sensor implements multiple interfaces
class SmartSensor : public IReadable_ISP,
                    public ICalibrateable_ISP,
                    public IPowerManaged_ISP {
public:
    double read() override { cout << "  SmartSensor: reading" << endl; return 0; }
    void calibrate() override { cout << "  SmartSensor: calibrating" << endl; }
    void sleep() override { cout << "  SmartSensor: sleeping" << endl; }
    void wake() override { cout << "  SmartSensor: waking" << endl; }
};

void isp_demo() {
    cout << "\n=== I: Interface Segregation ===" << endl;
    SimpleSensor simple;
    SmartSensor smart;

    // Both are IReadable
    IReadable_ISP *readers[] = {&simple, &smart};
    for (auto r : readers) r->read();

    // Only SmartSensor is ICalibrateable
    ICalibrateable_ISP *cal = &smart;
    cal->calibrate();
}

// ============================================================
// D — DEPENDENCY INVERSION PRINCIPLE
// "Depend on abstractions, not concrete classes."
// ============================================================

// BAD: High-level module depends on low-level concrete class
class ConcreteGPS {
public:
    double getLat() { return 34.05; }
};

class Navigator_Bad {
    ConcreteGPS gps;  // directly depends on concrete GPS
    // Can't test without real GPS. Can't swap GPS hardware. Tight coupling.
public:
    void navigate() {
        cout << "  Navigating to " << gps.getLat() << endl;
    }
};

// GOOD: Both depend on an abstraction
class IPositionSource {
public:
    virtual double getLatitude() = 0;
    virtual double getLongitude() = 0;
    virtual ~IPositionSource() {}
};

class RealGPS : public IPositionSource {
public:
    double getLatitude() override { return 34.0522; }
    double getLongitude() override { return -118.2437; }
};

class MockGPS : public IPositionSource {
public:
    double getLatitude() override { return 0.0; }    // test value
    double getLongitude() override { return 0.0; }
};

class Navigator_Good {
    IPositionSource &pos;  // depends on ABSTRACTION
public:
    Navigator_Good(IPositionSource &source) : pos(source) {}
    void navigate() {
        cout << "  Navigating to lat=" << pos.getLatitude()
             << " lon=" << pos.getLongitude() << endl;
    }
};

void dip_demo() {
    cout << "\n=== D: Dependency Inversion ===" << endl;

    RealGPS real;
    MockGPS mock;

    Navigator_Good nav_real(real);
    Navigator_Good nav_mock(mock);  // same Navigator, different source!

    cout << "  With real GPS:" << endl;
    nav_real.navigate();

    cout << "  With mock GPS (testing):" << endl;
    nav_mock.navigate();
}

// ============================================================

int main() {
    srp_demo();
    ocp_demo();
    lsp_demo();
    isp_demo();
    dip_demo();

    cout << "\n=== Summary ===" << endl;
    cout << "S: One class, one job." << endl;
    cout << "O: Add new behavior by adding code, not changing it." << endl;
    cout << "L: Subtypes must be substitutable for their base type." << endl;
    cout << "I: Many small interfaces beat one big one." << endl;
    cout << "D: Depend on abstractions, inject dependencies." << endl;

    return 0;
}

/*
 * ============================================================
 * YOUR TASK 12a:
 * Take the capstone (Ex 11) and identify which SOLID principles
 * it follows and which it could improve on. Write comments.
 * ============================================================
 *
 * YOUR TASK 12b:
 * Refactor this function using Open/Closed:
 *
 *   string formatOutput(string type, double value) {
 *       if (type == "json") return "{\"value\":" + to_string(value) + "}";
 *       if (type == "csv") return to_string(value) + ",";
 *       if (type == "xml") return "<v>" + to_string(value) + "</v>";
 *   }
 *
 * Create an IFormatter interface and concrete implementations.
 * ============================================================
 *
 * YOUR TASK 12c:
 * Apply Dependency Inversion to a motor controller:
 *   - Create IMotorDriver interface (setPWM, getStatus)
 *   - Create RealMotorDriver and MockMotorDriver
 *   - Create MotorController that takes IMotorDriver& in constructor
 *   - Demonstrate testing with the mock
 *
 * This is how you'd structure driver code at Red Cat so you
 * can test flight algorithms on your laptop without real motors.
 * ============================================================
 *
 * INTERVIEW TIP: Don't just memorize the acronym.
 * Pick your BEST example for each letter and practice saying:
 * "In my drone project, I applied [principle] by [specific thing].
 *  This helped because [concrete benefit]."
 * ============================================================
 */
