/*
 * ============================================================
 * EXERCISE 11: CAPSTONE — Drone Sensor Fusion System
 * ============================================================
 * This combines EVERYTHING from exercises 1-10 into a system
 * that simulates a real drone's sensor pipeline.
 *
 * COMPLETE THE SECTIONS MARKED WITH "TODO"
 *
 * Concepts used:
 *   - Classes & Encapsulation (Ex 1-2)
 *   - Constructors, Destructors, RAII (Ex 3)
 *   - Operator Overloading (Ex 4)
 *   - Inheritance (Ex 5)
 *   - Polymorphism & Virtual Functions (Ex 6)
 *   - Abstract Classes & Interfaces (Ex 7)
 *   - Templates (Ex 8)
 *   - Smart Pointers (Ex 9)
 *   - Design Patterns (Ex 10)
 *
 * Compile: g++ -std=c++17 -Wall -o ex11 11_capstone.cpp
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
using namespace std;

// ============================================================
// Vec3 — 3D vector (from Exercise 4)
// ============================================================

class Vec3 {
public:
    double x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3 &o) const { return {x+o.x, y+o.y, z+o.z}; }
    Vec3 operator-(const Vec3 &o) const { return {x-o.x, y-o.y, z-o.z}; }
    Vec3 operator*(double s) const { return {x*s, y*s, z*s}; }

    double magnitude() const { return sqrt(x*x + y*y + z*z); }

    friend ostream& operator<<(ostream &os, const Vec3 &v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }
};

// ============================================================
// Ring Buffer Template (from Exercise 8)
// ============================================================

template <typename T, int SIZE>
class RingBuffer {
private:
    T data[SIZE];
    int head, tail, count;

public:
    RingBuffer() : head(0), tail(0), count(0) {}

    void push(const T &val) {
        data[head] = val;
        head = (head + 1) % SIZE;
        if (count < SIZE) count++;
        else tail = (tail + 1) % SIZE;
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
};

// ============================================================
// Observer Pattern (from Exercise 10)
// ============================================================

class IDroneObserver {
public:
    virtual void onEvent(const string &event, const string &data) = 0;
    virtual ~IDroneObserver() {}
};

class DroneEventBus {
private:
    vector<IDroneObserver*> observers;

public:
    void subscribe(IDroneObserver *obs) { observers.push_back(obs); }

    void publish(const string &event, const string &data) {
        for (auto obs : observers) {
            obs->onEvent(event, data);
        }
    }
};

// ============================================================
// Sensor Interface (from Exercise 7) — Abstract Base
// ============================================================

class ISensor {
public:
    virtual void read() = 0;
    virtual string getName() const = 0;
    virtual string getDataString() const = 0;
    virtual ~ISensor() {}
};

// ============================================================
// Filter Strategy (from Exercise 10)
// ============================================================

class IFilter {
public:
    virtual double apply(double raw) = 0;
    virtual string getName() const = 0;
    virtual ~IFilter() {}
};

class NoFilter : public IFilter {
public:
    double apply(double raw) override { return raw; }
    string getName() const override { return "None"; }
};

class LowPassFilter : public IFilter {
private:
    double alpha;
    double prev;
    bool initialized;

public:
    LowPassFilter(double alpha) : alpha(alpha), prev(0), initialized(false) {}

    double apply(double raw) override {
        if (!initialized) {
            prev = raw;
            initialized = true;
            return raw;
        }
        prev = alpha * raw + (1.0 - alpha) * prev;
        return prev;
    }

    string getName() const override { return "LowPass"; }
};

// ============================================================
// Concrete Sensors (from Exercise 6)
// ============================================================

class GPSSensor : public ISensor {
private:
    Vec3 position;
    unique_ptr<IFilter> lat_filter;
    unique_ptr<IFilter> lon_filter;

public:
    GPSSensor(unique_ptr<IFilter> lf, unique_ptr<IFilter> lonf)
        : lat_filter(move(lf)), lon_filter(move(lonf)) {}

    void read() override {
        // Simulate GPS reading with noise
        double raw_lat = 34.0522 + (rand() % 100 - 50) * 0.00001;
        double raw_lon = -118.2437 + (rand() % 100 - 50) * 0.00001;
        position = Vec3(lat_filter->apply(raw_lat),
                        lon_filter->apply(raw_lon), 0);
    }

    string getName() const override { return "GPS"; }

    string getDataString() const override {
        return "lat=" + to_string(position.x) +
               " lon=" + to_string(position.y);
    }

    Vec3 getPosition() const { return position; }
};

class IMUSensor : public ISensor {
private:
    Vec3 acceleration;
    Vec3 gyro;

public:
    void read() override {
        acceleration = Vec3(
            (rand() % 100 - 50) * 0.001,
            (rand() % 100 - 50) * 0.001,
            9.81 + (rand() % 100 - 50) * 0.001
        );
        gyro = Vec3(
            (rand() % 100 - 50) * 0.01,
            (rand() % 100 - 50) * 0.01,
            (rand() % 100 - 50) * 0.01
        );
    }

    string getName() const override { return "IMU"; }

    string getDataString() const override {
        return "accel=" + to_string(acceleration.z) +
               " gyro_z=" + to_string(gyro.z);
    }

    Vec3 getAcceleration() const { return acceleration; }
    Vec3 getGyro() const { return gyro; }
};

// TODO 1: Create a BarometerSensor class that:
//   - Inherits from ISensor
//   - Has private: double pressure_hpa, double altitude
//   - read() simulates pressure around 1013.25 with noise
//   - Calculates altitude from pressure
//   - Implements getName() and getDataString()



// ============================================================
// Battery Monitor with Observer (from Exercise 10)
// ============================================================

class BatteryMonitor {
private:
    double voltage;
    DroneEventBus &eventBus;

public:
    BatteryMonitor(DroneEventBus &bus) : voltage(12.6), eventBus(bus) {}

    void update(double dt) {
        voltage -= dt * 0.01;  // drain over time
        if (voltage < 9.5) {
            eventBus.publish("BATTERY_CRITICAL",
                             to_string(getPercent()) + "%");
        } else if (voltage < 10.0) {
            eventBus.publish("BATTERY_LOW",
                             to_string(getPercent()) + "%");
        }
    }

    double getPercent() const {
        double pct = ((voltage - 9.0) / (12.6 - 9.0)) * 100.0;
        return (pct < 0) ? 0 : pct;
    }

    double getVoltage() const { return voltage; }
};

// ============================================================
// Telemetry Logger (Observer + Singleton flavor)
// ============================================================

class TelemetryLogger : public IDroneObserver {
private:
    RingBuffer<string, 20> log_buffer;

public:
    void onEvent(const string &event, const string &data) override {
        string entry = "[" + event + "] " + data;
        log_buffer.push(entry);
        cout << "    LOG: " << entry << endl;
    }

    void dumpLog() {
        cout << "\n  === Telemetry Log ===" << endl;
        string entry;
        while (log_buffer.pop(entry)) {
            cout << "  " << entry << endl;
        }
    }
};

// TODO 2: Create a SafetyMonitor class that:
//   - Implements IDroneObserver
//   - On BATTERY_CRITICAL: prints "EMERGENCY: Initiating auto-land!"
//   - On BATTERY_LOW: prints "WARNING: Return to home recommended"
//   - Has a bool isEmergency() method



// ============================================================
// Drone Controller — ties everything together
// ============================================================

class DroneController {
private:
    // Owned sensors (unique_ptr = exclusive ownership, RAII)
    vector<unique_ptr<ISensor>> sensors;

    // Subsystems
    DroneEventBus eventBus;
    BatteryMonitor battery;
    TelemetryLogger telemetry;

    // State
    Vec3 position;
    Vec3 velocity;
    bool armed;
    int tick;

public:
    DroneController()
        : battery(eventBus), armed(false), tick(0) {
        // Subscribe observers
        eventBus.subscribe(&telemetry);

        // TODO 3: Create and subscribe a SafetyMonitor here

        cout << "Drone Controller initialized." << endl;
    }

    void addSensor(unique_ptr<ISensor> sensor) {
        cout << "  Added sensor: " << sensor->getName() << endl;
        sensors.push_back(move(sensor));
    }

    void arm() {
        armed = true;
        eventBus.publish("STATUS", "ARMED");
        cout << "  Drone ARMED." << endl;
    }

    void disarm() {
        armed = false;
        eventBus.publish("STATUS", "DISARMED");
        cout << "  Drone DISARMED." << endl;
    }

    void update(double dt) {
        if (!armed) return;
        tick++;

        // Read all sensors (polymorphism!)
        for (auto &s : sensors) {
            s->read();
        }

        // Update battery
        battery.update(dt);

        // Simulate position update
        velocity = Vec3(0.1, 0.05, 0.02);
        position = position + velocity * dt;

        // Print status every 10 ticks
        if (tick % 10 == 0) {
            cout << "  [Tick " << tick << "] pos=" << position
                 << " bat=" << battery.getPercent() << "%" << endl;
            for (auto &s : sensors) {
                cout << "    " << s->getName() << ": "
                     << s->getDataString() << endl;
            }
        }
    }

    void dumpTelemetry() {
        telemetry.dumpLog();
    }
};

// ============================================================
// Main — simulate a flight
// ============================================================

int main() {
    srand(42);  // deterministic "random" for testing

    cout << "========================================" << endl;
    cout << "  DRONE SENSOR FUSION SYSTEM" << endl;
    cout << "========================================\n" << endl;

    // Create drone controller
    DroneController drone;

    // Add sensors (Strategy: each GPS axis gets its own filter)
    drone.addSensor(make_unique<GPSSensor>(
        make_unique<LowPassFilter>(0.3),
        make_unique<LowPassFilter>(0.3)
    ));
    drone.addSensor(make_unique<IMUSensor>());

    // TODO 4: Add a BarometerSensor here

    // Arm and fly
    drone.arm();

    cout << "\n--- Simulating 300 ticks of flight ---\n" << endl;
    for (int i = 0; i < 300; i++) {
        drone.update(1.0);
    }

    drone.disarm();
    drone.dumpTelemetry();

    cout << "\n========================================" << endl;
    cout << "  FLIGHT COMPLETE" << endl;
    cout << "========================================" << endl;

    return 0;
}

/*
 * ============================================================
 * AFTER COMPLETING ALL TODOs:
 *
 * 1. Add a TODO 5: Create a FlightMode strategy pattern that
 *    switches between Manual, Stabilize, and AutoLand modes
 *    based on battery level events.
 *
 * 2. Draw the class diagram on paper:
 *    - Interfaces: ISensor, IFilter, IDroneObserver
 *    - Concrete: GPSSensor, IMUSensor, BarometerSensor
 *    - Templates: RingBuffer<T, N>
 *    - Show which classes own which (composition vs reference)
 *
 * 3. Identify every OOP concept used:
 *    - Encapsulation: private members in every class
 *    - Abstraction: ISensor interface hides implementation details
 *    - Inheritance: GPSSensor extends ISensor
 *    - Polymorphism: vector<unique_ptr<ISensor>> with read()
 *    - RAII: unique_ptr manages sensor lifetime
 *    - Observer: DroneEventBus + IDroneObserver
 *    - Strategy: IFilter with LowPassFilter vs NoFilter
 *    - Templates: RingBuffer<string, 20>
 *
 * 4. Practice explaining this system to an interviewer.
 *    Aim for 2 minutes. Hit these points:
 *    - "I designed a modular sensor system using interfaces..."
 *    - "Sensors are polymorphic — adding new types requires no
 *      changes to the controller..."
 *    - "I use the Observer pattern for events like battery
 *      warnings, so subsystems are decoupled..."
 *    - "The Strategy pattern lets me swap filtering algorithms
 *      at runtime..."
 *    - "Memory is managed with unique_ptr for ownership and
 *      raw pointers for non-owning references..."
 * ============================================================
 */
