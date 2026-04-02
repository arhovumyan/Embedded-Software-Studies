/*
 * ============================================================
 * EXERCISE 2: Encapsulation — Why Hiding Data Matters
 * ============================================================
 * GOAL: Understand that encapsulation isn't about being secretive.
 * It's about PROTECTING INVARIANTS — rules that must always be
 * true about your data.
 *
 * Example invariant: "speed must always be between 0 and max_speed"
 * If anyone can write directly to speed, this invariant can be broken.
 * Encapsulation means the ONLY way to change speed is through methods
 * that enforce the rules.
 *
 * Compile: g++ -std=c++17 -Wall -o ex02 02_encapsulation.cpp
 * ============================================================
 */

#include <iostream>
#include <string>
using namespace std;

// ============================================================
// PART 1: A BankAccount — classic encapsulation example
// ============================================================

class BankAccount {
private:
    string owner;
    double balance;
    int transaction_count;

    // Private helper — internal logic nobody outside needs to know
    void log_transaction(const string &type, double amount) {
        transaction_count++;
        cout << "  [TX #" << transaction_count << "] " << type
             << ": $" << amount << " | Balance: $" << balance << endl;
    }

public:
    BankAccount(const string &owner, double initial_balance) {
        this->owner = owner;
        this->balance = (initial_balance >= 0) ? initial_balance : 0;
        this->transaction_count = 0;
        cout << "Account created for " << owner
             << " with $" << balance << endl;
    }

    // Deposit enforces: amount must be positive
    bool deposit(double amount) {
        if (amount <= 0) {
            cout << "  ERROR: Deposit amount must be positive" << endl;
            return false;
        }
        balance += amount;
        log_transaction("Deposit", amount);
        return true;
    }

    // Withdraw enforces: can't go negative, amount must be positive
    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "  ERROR: Withdrawal amount must be positive" << endl;
            return false;
        }
        if (amount > balance) {
            cout << "  ERROR: Insufficient funds (have $" << balance
                 << ", need $" << amount << ")" << endl;
            return false;
        }
        balance -= amount;
        log_transaction("Withdraw", amount);
        return true;
    }

    // Read-only access — 'const' means this can't modify anything
    double getBalance() const { return balance; }
    string getOwner() const { return owner; }
    int getTransactionCount() const { return transaction_count; }

    void printStatement() const {
        cout << "\n  === Statement for " << owner << " ===" << endl;
        cout << "  Balance: $" << balance << endl;
        cout << "  Total transactions: " << transaction_count << endl;
        cout << "  ============================\n" << endl;
    }
};

// ============================================================
// PART 2: Sensor with validation — embedded example
// ============================================================

class TemperatureSensor {
private:
    double current_temp;
    double min_valid;        // readings below this are noise
    double max_valid;        // readings above this are noise
    int valid_readings;
    int rejected_readings;

    // Private: only the sensor decides if a reading is valid
    bool isValidReading(double temp) const {
        return (temp >= min_valid && temp <= max_valid);
    }

public:
    TemperatureSensor(double min_range, double max_range)
        : current_temp(0), min_valid(min_range), max_valid(max_range),
          valid_readings(0), rejected_readings(0) {
        // ^^^ This is a MEMBER INITIALIZER LIST
        // It's the preferred way to initialize members in C++
        // More efficient than assigning in the body
    }

    // Process a new reading from hardware
    bool update(double raw_reading) {
        if (!isValidReading(raw_reading)) {
            rejected_readings++;
            cout << "  REJECTED: " << raw_reading
                 << "°C (out of range [" << min_valid
                 << ", " << max_valid << "])" << endl;
            return false;
        }
        current_temp = raw_reading;
        valid_readings++;
        return true;
    }

    double getTemperature() const { return current_temp; }

    double getRejectRate() const {
        int total = valid_readings + rejected_readings;
        if (total == 0) return 0;
        return (double)rejected_readings / total * 100.0;
    }

    void printStatus() const {
        cout << "  Temp: " << current_temp << "°C"
             << " | Valid: " << valid_readings
             << " | Rejected: " << rejected_readings
             << " | Reject rate: " << getRejectRate() << "%"
             << endl;
    }
};

// ============================================================
// PART 3: Access levels explained
// ============================================================

class AccessDemo {
private:
    int secret;          // only this class

protected:
    int family_secret;   // this class + children (inheritance)

public:
    int open_book;       // everyone

    AccessDemo() : secret(1), family_secret(2), open_book(3) {}

    void showAll() const {
        // Inside the class, you can access EVERYTHING
        cout << "  private: " << secret
             << ", protected: " << family_secret
             << ", public: " << open_book << endl;
    }
};

// ============================================================
// PART 4: Const correctness — a form of encapsulation
// ============================================================

class Point {
private:
    double x, y;

public:
    Point(double x, double y) : x(x), y(y) {}

    // CONST methods: promise they won't modify the object
    double getX() const { return x; }
    double getY() const { return y; }

    double distanceTo(const Point &other) const {
        // 'const Point &other' means: I borrow other by reference
        // (efficient, no copy) and I promise not to modify it
        double dx = x - other.x;
        double dy = y - other.y;
        return sqrt(dx * dx + dy * dy);
    }

    // NON-CONST: this method DOES modify the object
    void move(double dx, double dy) {
        x += dx;
        y += dy;
    }

    void print() const {
        cout << "  (" << x << ", " << y << ")" << endl;
    }
};

// This function takes a CONST reference — it can only call
// const methods on the Point
void printPointInfo(const Point &p) {
    cout << "  Point at: ";
    p.print();          // OK: print() is const
    // p.move(1, 1);    // ERROR: move() is not const!
}

// ============================================================

int main() {
    cout << "=== BankAccount Encapsulation ===" << endl;
    BankAccount acct("Areg", 1000);
    acct.deposit(500);
    acct.withdraw(200);
    acct.withdraw(5000);    // rejected!
    acct.deposit(-100);     // rejected!
    // acct.balance = 999999;   // WON'T COMPILE: private
    acct.printStatement();

    cout << "=== Temperature Sensor ===" << endl;
    TemperatureSensor sensor(-40, 85);  // valid range: -40 to 85°C
    double readings[] = {22.5, 23.1, -50.0, 24.0, 100.0, 23.8, 999.0, 22.0};
    for (double r : readings) {
        sensor.update(r);
    }
    sensor.printStatus();

    cout << "\n=== Access Levels ===" << endl;
    AccessDemo demo;
    demo.showAll();
    cout << "  Public access: " << demo.open_book << endl;
    // demo.secret = 10;          // ERROR: private
    // demo.family_secret = 10;   // ERROR: protected

    cout << "\n=== Const Correctness ===" << endl;
    Point p1(3.0, 4.0);
    Point p2(6.0, 8.0);
    p1.print();
    p2.print();
    cout << "  Distance: " << p1.distanceTo(p2) << endl;
    printPointInfo(p1);

    return 0;
}

/*
 * ============================================================
 * YOUR TASK 2a:
 * Create a class called Password with:
 *   - Private: string hash (store "hashed" password), int attempts
 *   - Private method: string hashPassword(string) — just reverse
 *     the string for simplicity (real hashing is complex)
 *   - Public: setPassword(string), bool checkPassword(string),
 *     int getAttempts()
 *   - Lock out after 3 failed attempts
 *
 * The point: the actual hash is NEVER exposed. The only way to
 * interact is through the controlled public interface.
 * ============================================================
 *
 * YOUR TASK 2b:
 * Add a getRejectRate() method to TemperatureSensor that returns
 * the percentage of rejected readings. Make sure it's const.
 * Then create a function:
 *   void analyzesSensor(const TemperatureSensor &s)
 * that can only call const methods. Try calling update() inside
 * it — read the error message.
 * ============================================================
 *
 * YOUR TASK 2c:
 * Create a class called CircularBuffer (like the ring buffer from
 * the C exercises, but as a proper class):
 *   - Private: int data[], int head, tail, count, capacity
 *   - Public: push(int), int pop(), int peek(), bool isEmpty(),
 *     bool isFull(), int size()
 *   - Constructor takes capacity
 *   - All read-only methods should be const
 *
 * Compare how clean this is vs the C struct + function approach.
 * ============================================================
 *
 * INTERVIEW TIP: "What is encapsulation?"
 * "Encapsulation bundles data with the methods that operate on
 * it, and restricts direct access to the internal state. This
 * protects invariants — rules about the data that must always
 * be true. For example, a motor's speed should never exceed its
 * max RPM, so the setter enforces that constraint."
 * ============================================================
 */
