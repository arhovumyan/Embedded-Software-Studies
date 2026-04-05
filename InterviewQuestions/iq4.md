what is real time OS ? 
    RTOS is a os that guarantees task execution within a bounded time window, not only fast but in a predictable manner. it provides a scheduler, queues, semaphores and mutexes. the key metric is Worst case execution time WCET. FreeRTOS is one of the examples. hard real time means missin a deadline(airbag controller), soft real time means degraded quality(video buffer). 

Memory-mapped I/O — what it means to read/write hardware registers

Flash vs RAM vs EEPROM differences
    RAM = temp working memory, the CPU uses it while the system is running, very fast, loses memory when powered off, used for variables, stack , heap, program data

    Flash = non volatile memory, keeps data even when the power is off, slower than RAM, written and erased in Blocks, not as freely as RAM.

    EEPROM = non volatile memory, keeps data when power is off, usually for smaller(than flash) data storage, slower than RAM, better than flash for changing small data several bytes at time. 

Stack overflow and how to prevent it in constrained systems
    temp data that we used during our program exection (for example recursions since they will over produce data if not handled correctly or not cleared), keep executing and keep filling up the memory on our machine. until it triggers that stack overflow. In this step the machine will either kill the process or shut down the machine. Basically the program has used up all the memory that we have in the stack. 

    for example
        #include <stdio.h>

        void crash() {
            crash();
        }

        int main() {
            crash();
            return 0;
        }
    // Since crash it calling itself indevinitely, it clutters the stack and overflow happens. 

Memory alignment and padding in structs
    idk

Bootloader basics — what happens when a microcontroller powers on
    on ARM cortex-M
        1) the code reads address 0x00000000 for the initial stack pointer, then 0x000000004 for the reset handler address. 
        2) initializes clock, PLL, watchdog, checks for update trigger
        3) firmware validation, verifies application hash at the start of the application flash region
        4) sets MSP to the applications's vector table stack pointer then branches to its reset handler. 
        5) startup.s copies .data from flash to RAM, zeroes .bss then calls main()

Communication Protocols

UART/USART — how it works, baud rate, start/stop bits
    serial communication through 1 wire(not parallel)
    point to point no shared clock, both sides agree on baud rate (pre arrange speed measurement in which they send data to each other) ahead of time. Since UART dont have a timing, it mainly uses baud rates for this. 
    These can be suplex, half-duplex and full duplex, one sends only, each sends but one at a time and both speak simulaneously. data is transmited as frames. 

SPI — master/slave, MOSI/MISO/CLK/CS, when you'd use it
    usually used for transferring smart controller and less smart controller. Usually goes with 1 master to other slaves. this is how they talk
        cs -> cs (choose communication target)
        synchronous clock -> synchronous clock (for providing time and synchronization)
        mosi -> mosi (data transmissted by master) (stands for master-in-slave-out)
        miso <- miso (data received by the master)

    1) chip select (cs) sets as low to indicate that data is incoming
    2) starts the clock using sclk
    3) mosi sends bits to the slave
    4) if slave has bits to send, then miso sends bits to master
    5) at the end chip select (cs) is put to high

I2C — addressing, SDA/SCL, pull-up resistors, read/write transactions

Know the tradeoffs: speed, number of wires, number of devices