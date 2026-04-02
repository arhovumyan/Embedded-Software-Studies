C/C++ Fundamentals (highest priority)

Pointers and pointer arithmetic, double pointers, function pointers
Memory layout: stack vs heap, static vs global variables
Dynamic memory allocation (malloc/free, new/delete) and why you avoid it in embedded
Structs, unions, enums, bitfields
The volatile keyword and why it matters in embedded
The const keyword and its various uses
static keyword (in functions vs file scope)
Data types and sizes, stdint.h types like uint8_t, uint32_t
Preprocessor directives (#define, #ifdef, macros)
Endianness (big vs little endian) and byte ordering

Bitwise Operations (very commonly asked)

AND, OR, XOR, NOT, left/right shift
Setting, clearing, toggling, and checking a specific bit
Bit masking to extract or modify fields in a register
Know how to write these from scratch on a whiteboard

Memory and Hardware Concepts

Memory-mapped I/O — what it means to read/write hardware registers
Flash vs RAM vs EEPROM differences
Stack overflow and how to prevent it in constrained systems
Memory alignment and padding in structs
Bootloader basics — what happens when a microcontroller powers on

Communication Protocols

UART/USART — how it works, baud rate, start/stop bits
SPI — master/slave, MOSI/MISO/CLK/CS, when you'd use it
I2C — addressing, SDA/SCL, pull-up resistors, read/write transactions
Know the tradeoffs: speed, number of wires, number of devices

Interrupts and Concurrency

What an interrupt is, ISR (interrupt service routine)
Why ISRs should be short and what you should/shouldn't do inside them
Interrupt priority and nesting
Volatile variables in the context of ISRs
Race conditions and how to prevent them (disabling interrupts, mutexes)
Debouncing (hardware vs software)

Peripherals and GPIO

What GPIO is, input/output modes
Pull-up and pull-down resistors
PWM — what it is, duty cycle, common uses (motor control, LED dimming)
ADC/DAC basics — converting analog signals to digital and vice versa
Timers and counters

RTOS Basics (if they mention real-time)

What an RTOS is and why you'd use one over bare-metal
Tasks/threads, priorities, scheduling
Semaphores, mutexes, queues for inter-task communication
Priority inversion and how to handle it
Deadlock — what it is and how to avoid it

Embedded Linux (since the JD mentions it)

Cross-compilation — what it is and why it's needed
Device trees basics
Kernel space vs user space
Basic Linux commands for debugging (dmesg, lsusb, i2cdetect)
Writing a simple device driver conceptually

Debugging and Tools

How you'd debug without a screen (LEDs, UART printf, logic analyzer)
JTAG/SWD debugging concepts
Common embedded bugs: stack overflow, uninitialized variables, race conditions, watchdog resets
Oscilloscope and logic analyzer — when you'd use each

Git/Version Control

Basic commands: clone, branch, commit, merge, rebase
Resolving merge conflicts
Branching strategies (feature branches, pull requests)

Common Interview Questions to Practice

"Write a function to set bit N of a register"
"What does volatile do and when do you use it?"
"Explain the difference between SPI and I2C"
"What happens when you press the power button on a microcontroller?"
"How would you debounce a button in software?"
"Why shouldn't you use malloc in an embedded system?"
"What's a race condition and how do you prevent it in firmware?"