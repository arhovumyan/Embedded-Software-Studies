
For C embedded fundamentals, watch "Fastbit Embedded Brain Academy" on YouTube — they have a great embedded C series. "EmbeTronicX" (embetronicx.com) has excellent written tutorials on everything from bitwise ops to Linux device drivers. For quick reference, "Embedded Artistry" (embeddedartistry.com) has solid articles. For interactive C practice, use leetcode or hackerrank but focus only on bit manipulation and pointer problems. "Low-Level Learning" on YouTube explains memory, pointers, and hardware concepts really well.
Day-by-Day Plan
Day 1 — C Fundamentals and Memory
Spend the morning studying pointers, pointer arithmetic, double pointers, and function pointers. Write small programs in VS Code that swap variables with pointers, iterate through arrays with pointer arithmetic, and pass function pointers as callbacks. In the afternoon, study stack vs heap, malloc/free, and why embedded avoids dynamic allocation. Study static, const, volatile — write examples of each. Practice explaining out loud what each keyword does because you'll need to verbalize this in the interview. Use this exercise: write a program that demonstrates what happens with and without volatile in a simulated ISR scenario.
Day 2 — Bitwise Operations (drill this hard)
Morning: learn AND, OR, XOR, NOT, shifts. Then practice these specific functions until you can write them from memory:
c// Set bit n
reg |= (1 << n);
// Clear bit n
reg &= ~(1 << n);
// Toggle bit n
reg ^= (1 << n);
// Check bit n
if (reg & (1 << n))
Afternoon: practice bit masking to extract fields from a register value, write a function that reverses bits, counts set bits, and checks if a number is a power of 2. Do 5-10 bit manipulation problems on LeetCode (easy level). This topic is almost guaranteed in interviews so drill it until it's automatic.
Day 3 — Communication Protocols (theory heavy day)
Study UART, SPI, and I2C. For each one, learn how it works physically (wires, signals), when you'd choose it, and its limitations. Make yourself a comparison table. Use YouTube — "Controllers Tech" and "Fastbit" channels have excellent protocol explanations with timing diagrams. Then go to Wokwi and simulate an I2C sensor reading on an ESP32 or Arduino — even in simulation this makes the concepts stick. By end of day, you should be able to answer "explain the difference between SPI and I2C" confidently for 2 minutes straight.
Day 4 — GPIO, Peripherals, and Interrupts
Morning: go to Wokwi and build these projects in simulation — blink an LED (GPIO output), read a button (GPIO input with pull-up), dim an LED with PWM, read an analog sensor with ADC. Write all of these in C, not Arduino's simplified API. Afternoon: study interrupts. Understand what an ISR is, why it should be short, what volatile has to do with ISRs, and what a race condition looks like. Write a button-interrupt handler in Wokwi that toggles an LED. Then write a software debounce for it. Study interrupt priority and nesting conceptually.
Day 5 — RTOS and Embedded Linux Basics
Morning: study RTOS concepts — tasks, priorities, preemptive scheduling, semaphores, mutexes, queues. You don't need to run FreeRTOS (though you can in Wokwi on ESP32). Focus on understanding when you'd use a mutex vs a semaphore, what priority inversion is, and what causes deadlock. Afternoon: embedded Linux — study cross-compilation (what and why), kernel space vs user space, what a device tree is at a high level, and basic debug commands (dmesg, lsmod, lsusb, i2cdetect). Practice these in WSL or any Linux terminal. Read one short tutorial on writing a basic Linux kernel module conceptually.
Day 6 — Memory, Boot Process, and Debugging
Morning: study the microcontroller boot process (power on, reset vector, startup code, main). Study memory-mapped I/O — understand that writing to a specific address actually controls hardware. Study linker scripts at a high level (what .text, .data, .bss sections are). Afternoon: study debugging techniques — JTAG/SWD conceptually, using UART for printf debugging, logic analyzers, common embedded bugs (stack overflow, buffer overflow, uninitialized variables, watchdog timeouts). Study endianness and struct padding/alignment — write a program that shows sizeof different structs with and without packing.
Day 7 — Mock Interview and Review
Morning: review your weak areas. Go through the common interview questions I listed yesterday and answer every single one out loud as if someone is sitting across from you. Time yourself — aim for 1-2 minute clear answers. Afternoon: do a mock interview. Find embedded systems interview questions on glassdoor or search "embedded C interview questions" and go through 30-40 of them. For any question you can't answer, look it up and write a short note. Review your Wokwi projects and make sure you can explain every line of code. End the day by reviewing your comparison table for protocols and your bit manipulation cheat sheet.
Quick Reference Sites to Bookmark
Use "Embedded Artistry" for well-written articles on specific topics, "EmbeTronicX" for structured tutorials, "GeeksforGeeks Embedded Systems" section for quick concept summaries, and the "Interrupt" blog by Memfault for professional-level embedded engineering articles. For video learners, the YouTube channels "Fastbit Embedded Brain Academy", "Controllers Tech", "Low-Level Learning", and "Jacob Sorber" cover nearly everything on this list.
One Last Tip
Don't try to memorize everything. The interviewer wants to see that you understand why things work the way they do. If you understand why volatile exists (because the compiler might optimize away reads to a variable that an ISR modifies), you can derive the answer even under pressure. Focus on understanding the reasoning, not memorizing definitions.