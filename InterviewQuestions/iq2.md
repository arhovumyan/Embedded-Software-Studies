how do we reduce latency during the ISR(interrupt service routine) call ? 
    write small and efficient ISRs
    avoid loops
    do not disable interrupts
    avoid high latency instructions

what is the difference between a thread and a process?
    process is an instace of program(your code) that is being executed.
    thread is a unit of execution within a process.

What is the difference between TCP(Transmission Control Protocol) and UDP(User Datagram Protocol)
    TCP uses Three way handshake. First it sends SYN to the receiving computer, the receiver then sends SYN-ACK that includes a message of its own. Lastly the sender sends an ACK message to the receiver. Now you have an open TCP connection. A similar process happens when closing this connection down. Each data is sent with a Sequence number which is used to store the data on the other end for the receiver. If the data has been transferred successfully, it generates a checksum result. This works because both side are generating a checksum and if both of their CHECKSUMs match, that means data was successfully transferred. 

    UDP just sends data like a retard and don't stop