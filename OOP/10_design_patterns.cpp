/*
 * ============================================================
 * EXERCISE 10: Design Patterns — Reusable Solutions
 * ============================================================
 * GOAL: Learn the 3 most common patterns you'll encounter in
 * embedded/systems code: Observer, Strategy, and Singleton.
 * These come up in interviews constantly.
 *
 * Compile: g++ -std=c++17 -Wall -o ex10 10_design_patterns.cpp
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
using namespace std;

// ============================================================
// PATTERN 1: OBSERVER — "Something happened, tell everyone"
// ============================================================
//
// USE WHEN: Multiple objects need to react when something changes,
// but the thing that changes shouldn't know who's listening.
//
// DRONE EXAMPLE: When battery drops below 20%, the flight
// controller, LED controller, and telemetry system all need
// to know — but the battery monitor shouldn't know about all
// those systems.

class IObserver {
public:
    virtual void onNotify(const string &event, double value) = 0;
    virtual ~IObserver() {}
};

class EventEmitter {
private:
    vector<IObserver*> observers;

public:
    void subscribe(IObserver *obs) {
        observers.push_back(obs);
    }

    void unsubscribe(IObserver *obs) {
        observers.erase(
            remove(observers.begin(), observers.end(), obs),
            observers.end()
        );
    }

protected:
    void notify(const string &event, double value) {
        for (auto obs : observers) {
            obs->onNotify(event, value);
        }
    }
};

// The subject — doesn't know who's listening
class BatteryMonitor : public EventEmitter {
private:
    double voltage;
    double percent;

public:
    BatteryMonitor() : voltage(12.6), percent(100) {}

    void update(double new_voltage) {
        voltage = new_voltage;
        percent = ((voltage - 9.0) / (12.6 - 9.0)) * 100.0;
        if (percent < 0) percent = 0;

        cout << "  Battery: " << voltage << "V (" << percent << "%)" << endl;

        // Notify observers when thresholds are crossed
        if (percent < 10) {
            notify("BATTERY_CRITICAL", percent);
        } else if (percent < 20) {
            notify("BATTERY_LOW", percent);
        }
    }
};

// Observers — each reacts differently to the same event
class FlightController : public IObserver {
public:
    void onNotify(const string &event, double value) override {
        if (event == "BATTERY_CRITICAL") {
            cout << "  [FlightCtrl] EMERGENCY LANDING! Battery at "
                 << value << "%" << endl;
        } else if (event == "BATTERY_LOW") {
            cout << "  [FlightCtrl] Return to home. Battery at "
                 << value << "%" << endl;
        }
    }
};

class LEDController : public IObserver {
public:
    void onNotify(const string &event, double value) override {
        if (event == "BATTERY_CRITICAL") {
            cout << "  [LED] FLASHING RED!" << endl;
        } else if (event == "BATTERY_LOW") {
            cout << "  [LED] Solid yellow warning" << endl;
        }
    }
};

class TelemetrySystem : public IObserver {
public:
    void onNotify(const string &event, double value) override {
        cout << "  [Telemetry] Sending alert: " << event
             << " (" << value << "%)" << endl;
    }
};

void observer_demo() {
    cout << "=== OBSERVER PATTERN ===" << endl;

    BatteryMonitor battery;
    FlightController fc;
    LEDController led;
    TelemetrySystem telemetry;

    // Subscribe — battery doesn't know what these objects ARE
    battery.subscribe(&fc);
    battery.subscribe(&led);
    battery.subscribe(&telemetry);

    // Simulate battery drain
    cout << "\nSimulating battery drain:" << endl;
    battery.update(12.0);   // 83% — no alerts
    battery.update(11.0);   // 56% — no alerts
    battery.update(10.0);   // 28% — no alerts
    battery.update(9.7);    // 19% — LOW alert!
    battery.update(9.2);    // 6%  — CRITICAL alert!

    // Unsubscribe telemetry
    cout << "\nAfter unsubscribing telemetry:" << endl;
    battery.unsubscribe(&telemetry);
    battery.update(9.1);    // Only FC and LED respond
}

// ============================================================
// PATTERN 2: STRATEGY — "Swap algorithms at runtime"
// ============================================================
//
// USE WHEN: You have multiple ways to do something and want
// to switch between them without changing the code that uses them.
//
// DRONE EXAMPLE: Different flight modes (manual, auto-hover,
// follow-me) use different control algorithms, but the main
// flight loop doesn't change.

class IFlightStrategy {
public:
    virtual void computeMotorOutputs(double roll, double pitch,
                                      double yaw, double throttle) = 0;
    virtual string getName() const = 0;
    virtual ~IFlightStrategy() {}
};

class ManualMode : public IFlightStrategy {
public:
    void computeMotorOutputs(double roll, double pitch,
                              double yaw, double throttle) override {
        cout << "  [Manual] Direct stick input -> motors"
             << " (R:" << roll << " P:" << pitch
             << " Y:" << yaw << " T:" << throttle << ")" << endl;
    }
    string getName() const override { return "Manual"; }
};

class StabilizeMode : public IFlightStrategy {
public:
    void computeMotorOutputs(double roll, double pitch,
                              double yaw, double throttle) override {
        // Apply PID corrections
        double corrected_roll = roll * 0.5;   // dampen inputs
        double corrected_pitch = pitch * 0.5;
        cout << "  [Stabilize] PID corrected -> motors"
             << " (R:" << corrected_roll << " P:" << corrected_pitch
             << " Y:" << yaw << " T:" << throttle << ")" << endl;
    }
    string getName() const override { return "Stabilize"; }
};

class AutoHoverMode : public IFlightStrategy {
public:
    void computeMotorOutputs(double roll, double pitch,
                              double yaw, double throttle) override {
        // Ignore stick inputs, maintain position
        cout << "  [AutoHover] Holding position, ignoring sticks"
             << " (using GPS + baro)" << endl;
    }
    string getName() const override { return "Auto-Hover"; }
};

class FlightControllerV2 {
private:
    IFlightStrategy *strategy;

public:
    FlightControllerV2(IFlightStrategy *initial)
        : strategy(initial) {}

    // SWAP strategy at runtime — this is the power
    void setMode(IFlightStrategy *new_strategy) {
        cout << "  Switching from " << strategy->getName()
             << " to " << new_strategy->getName() << endl;
        strategy = new_strategy;
    }

    void processInput(double roll, double pitch,
                       double yaw, double throttle) {
        // Same function, different behavior depending on strategy
        strategy->computeMotorOutputs(roll, pitch, yaw, throttle);
    }
};

void strategy_demo() {
    cout << "\n\n=== STRATEGY PATTERN ===" << endl;

    ManualMode manual;
    StabilizeMode stabilize;
    AutoHoverMode autohover;

    FlightControllerV2 fc(&manual);

    cout << "\nFlying in manual:" << endl;
    fc.processInput(0.5, 0.3, 0.0, 0.8);

    cout << "\nPilot switches to stabilize:" << endl;
    fc.setMode(&stabilize);
    fc.processInput(0.5, 0.3, 0.0, 0.8);

    cout << "\nPilot switches to auto-hover:" << endl;
    fc.setMode(&autohover);
    fc.processInput(0.5, 0.3, 0.0, 0.8);
}

// ============================================================
// PATTERN 3: SINGLETON — "Only one of me, globally accessible"
// ============================================================
//
// USE WHEN: There must be exactly ONE instance (hardware manager,
// logger, configuration). Use sparingly — singletons are often
// a code smell and make testing harder.
//
// DRONE EXAMPLE: One hardware abstraction layer, one config manager.

class Logger {
private:
    int log_count;

    // Private constructor — nobody can create instances
    Logger() : log_count(0) {
        cout << "  Logger instance created" << endl;
    }

    // Delete copy and assignment
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

public:
    // Static method to get the ONE instance
    static Logger& getInstance() {
        static Logger instance;  // created on first call, destroyed at program exit
        return instance;
    }

    void log(const string &level, const string &message) {
        log_count++;
        cout << "  [" << log_count << "][" << level << "] " << message << endl;
    }

    int getLogCount() const { return log_count; }
};

void singleton_demo() {
    cout << "\n\n=== SINGLETON PATTERN ===" << endl;

    // Both get the SAME instance
    Logger &log1 = Logger::getInstance();
    Logger &log2 = Logger::getInstance();

    log1.log("INFO", "System starting");
    log2.log("WARN", "Battery at 50%");
    log1.log("ERROR", "GPS signal lost");

    cout << "  Same instance? " << (&log1 == &log2 ? "YES" : "NO") << endl;
    cout << "  Total logs: " << log1.getLogCount() << endl;

    // Can't create new instances:
    // Logger myLogger;              // ERROR: constructor is private
    // Logger copy = log1;           // ERROR: copy deleted
}

// ============================================================

int main() {
    observer_demo();
    strategy_demo();
    singleton_demo();
    return 0;
}

/*
 * ============================================================
 * YOUR TASK 10a:
 * Add a BuzzerController observer that:
 *   - Beeps slowly on BATTERY_LOW
 *   - Beeps rapidly on BATTERY_CRITICAL
 * Subscribe it to the battery monitor and test.
 * ============================================================
 *
 * YOUR TASK 10b:
 * Create a Strategy pattern for data filtering:
 *   Interface: IFilter with method double filter(double raw)
 *   Implementations:
 *     - NoFilter: returns raw
 *     - LowPassFilter: uses exponential moving average
 *     - MedianFilter: stores last 5 readings, returns median
 *
 * Create a SensorReader class that takes an IFilter* and
 * applies it to each reading. Swap filters at runtime.
 * ============================================================
 *
 * YOUR TASK 10c:
 * Create a Factory pattern:
 *   static unique_ptr<Sensor> SensorFactory::create(string type)
 *     - "gps" returns a GPSSensor
 *     - "imu" returns an IMUSensor
 *     - "baro" returns a BarometerSensor
 *     - unknown returns nullptr
 *
 * This lets you create sensors from config files or commands
 * without hardcoding which type to construct.
 * ============================================================
 *
 * INTERVIEW TIP: "What design patterns do you know?"
 * Pick 2-3 and explain with a CONCRETE example:
 * "I use Observer for event-driven systems like battery monitoring
 *  where multiple subsystems need to react to the same event.
 *  I use Strategy to swap algorithms at runtime, like different
 *  flight control modes. And I use Singleton sparingly for
 *  hardware managers where only one instance makes sense."
 * ============================================================
 */
