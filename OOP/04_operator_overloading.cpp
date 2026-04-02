/*
 * ============================================================
 * EXERCISE 4: Operator Overloading
 * ============================================================
 * GOAL: Make your objects work with +, -, ==, <<, etc.
 * This isn't just syntactic sugar — it makes code readable
 * and lets your types feel like built-in types.
 *
 * Compile: g++ -std=c++17 -Wall -o ex04 04_operator_overloading.cpp
 * ============================================================
 */

#include <iostream>
#include <cmath>
using namespace std;

// ============================================================
// A 3D Vector class — perfect for drone math
// ============================================================

class Vec3 {
private:
    double x, y, z;

public:
    Vec3() : x(0), y(0), z(0) {}
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

    // Getters
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }

    // --- ARITHMETIC OPERATORS ---

    // Addition: Vec3 + Vec3
    Vec3 operator+(const Vec3 &other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    // Subtraction: Vec3 - Vec3
    Vec3 operator-(const Vec3 &other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    // Scalar multiplication: Vec3 * double
    Vec3 operator*(double scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    // Compound assignment: Vec3 += Vec3
    Vec3& operator+=(const Vec3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;  // return reference to self for chaining
    }

    // Negation: -Vec3
    Vec3 operator-() const {
        return Vec3(-x, -y, -z);
    }

    // --- COMPARISON OPERATORS ---

    bool operator==(const Vec3 &other) const {
        const double EPSILON = 1e-9;
        return fabs(x - other.x) < EPSILON &&
               fabs(y - other.y) < EPSILON &&
               fabs(z - other.z) < EPSILON;
        // Never compare doubles with ==, use epsilon!
    }

    bool operator!=(const Vec3 &other) const {
        return !(*this == other);  // reuse == operator
    }

    // --- SUBSCRIPT OPERATOR ---

    // Access by index: v[0]=x, v[1]=y, v[2]=z
    double operator[](int index) const {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default:
                cerr << "Vec3: index out of range!" << endl;
                return 0;
        }
    }

    // --- USEFUL METHODS ---

    double magnitude() const {
        return sqrt(x * x + y * y + z * z);
    }

    Vec3 normalized() const {
        double mag = magnitude();
        if (mag < 1e-9) return Vec3(0, 0, 0);
        return Vec3(x / mag, y / mag, z / mag);
    }

    double dot(const Vec3 &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3 cross(const Vec3 &other) const {
        return Vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // --- STREAM OUTPUT OPERATOR ---
    // This MUST be a friend function (not a member) because
    // the left operand is ostream, not Vec3
    friend ostream& operator<<(ostream &os, const Vec3 &v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;  // return stream for chaining: cout << a << b
    }
};

// Scalar * Vec3 (when scalar is on the LEFT side)
// Must be a free function, not a member
Vec3 operator*(double scalar, const Vec3 &v) {
    return v * scalar;  // reuse the member operator
}

// ============================================================
// Demo
// ============================================================

int main() {
    Vec3 position(10, 20, 5);     // drone position
    Vec3 velocity(1, -0.5, 0.2);  // drone velocity

    cout << "=== Vec3 Operator Overloading ===" << endl;
    cout << "Position:  " << position << endl;
    cout << "Velocity:  " << velocity << endl;

    // Addition
    Vec3 new_pos = position + velocity;
    cout << "\nAfter 1 timestep (pos + vel):" << endl;
    cout << "New pos:   " << new_pos << endl;

    // Scalar multiplication
    Vec3 fast = velocity * 3.0;
    cout << "3x speed:  " << fast << endl;

    // Left-side scalar
    Vec3 also_fast = 3.0 * velocity;
    cout << "Also 3x:   " << also_fast << endl;

    // Compound assignment
    position += velocity;
    cout << "\nposition += velocity:" << endl;
    cout << "Position:  " << position << endl;

    // Negation
    cout << "Reverse:   " << -velocity << endl;

    // Comparison
    cout << "\nfast == also_fast? " << (fast == also_fast ? "yes" : "no") << endl;
    cout << "fast != velocity?  " << (fast != velocity ? "yes" : "no") << endl;

    // Subscript
    cout << "\nvelocity[0]=" << velocity[0]
         << " velocity[1]=" << velocity[1]
         << " velocity[2]=" << velocity[2] << endl;

    // Math operations
    cout << "\n=== Vector Math ===" << endl;
    cout << "Magnitude: " << velocity.magnitude() << endl;
    cout << "Normalized: " << velocity.normalized() << endl;
    cout << "Dot product: " << position.dot(velocity) << endl;

    Vec3 a(1, 0, 0);
    Vec3 b(0, 1, 0);
    cout << "Cross (1,0,0) x (0,1,0) = " << a.cross(b) << endl;

    // Chaining — operators return objects, so you can chain
    cout << "\n=== Chaining ===" << endl;
    Vec3 result = position + velocity * 2.0 + Vec3(0, 0, -9.8);
    cout << "pos + vel*2 + gravity = " << result << endl;

    return 0;
}

/*
 * ============================================================
 * YOUR TASK 4a:
 * Create a class called Fraction with:
 *   - Private: int numerator, int denominator
 *   - Constructor that simplifies (divide by GCD)
 *   - Operators: +, -, *, ==, !=, <<
 *   - Method: simplify() that reduces to lowest terms
 *
 * Test:
 *   Fraction a(1, 2);   // 1/2
 *   Fraction b(1, 3);   // 1/3
 *   cout << a + b;      // should print 5/6
 *   cout << a * b;      // should print 1/6
 * ============================================================
 *
 * YOUR TASK 4b:
 * Add to Vec3:
 *   - operator/ for scalar division
 *   - operator< that compares by magnitude (useful for sorting)
 *   - A static method Vec3::distance(Vec3 a, Vec3 b) that returns
 *     the distance between two points
 *
 * Test by calculating distance between two drone positions.
 * ============================================================
 *
 * YOUR TASK 4c:
 * Create a Matrix2x2 class with operator* for matrix multiplication.
 *   | a b |   | e f |
 *   | c d | * | g h |
 * This is relevant for rotation matrices in drone control.
 * ============================================================
 *
 * INTERVIEW TIP: "When should you overload operators?"
 * "When it makes the code more readable and the operation has
 * a natural mathematical meaning. Vec3 addition makes sense.
 * Don't overload + on a UserAccount class — that's confusing."
 * ============================================================
 */
