these are interview questions, and I want you to asnwer to them using the best of your ablitiy, then if you need to, write a small code that you demonstrate what that is, for exmaple you can write a code for what a pointer is, but you would not write a code example to explain what the different sizes of different variables are. Do these in C and C++. 


what is the size of these variables
    int -4
    double - 8 
    float - 4
    char - 1

what is the size of a pointer
    pointer - consistent for any data type. its just a holder to hold the address, thats it. 
        this varies by the architecture. 
            for a 32 bit architecture you have 
                intergter pointer 4 
                float pointer 4 
                char pointer 1 
            for a 64 bit architecture you have 
                intergter pointer 8
                float pointer 8 
                char pointer 2 

what is a pointer
    it is a variable that contains the address of another variable that it points to. 

what is a NULL pointer
    its a special pointer that points nowhere. No other pointer will be equal to Null pointer
    use predefined NULL constant from library <stdio.h> <stdlib.h> and <string.h>. This is used to prevent any garbage from getting into your previous pointers. its like a bottle cap. 

what is a void pointer, what is its limitations ? 
    
what is an ISR
    interrup service routine. its an interrupt handler. is called when the interrupt is encountered. 

what is the return type of ISR? 
    it dont return anything.

what it Latency?
    It means delay. interrupt latency is the time required for the interrup to be responded