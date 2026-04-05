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