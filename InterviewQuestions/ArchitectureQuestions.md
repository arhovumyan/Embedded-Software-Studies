How does a CPU work (central processing unit)
    cpu is the brain of the computer and executes tasks(threads) using its cores. can perform parallel tasks using multiprocessing by using its separate cores and splitting tasks among them. it uses a cycle called fetch-decode-execute
        fetch - the cpu grabs instruction from RAM using the program counter
        decode - instructions translated into signals that the hardware understands
        execute - the alu or other unit executes the work
        write back the result is stored in the memory

    there are different types of cores
        efficiency cores handle lightweight tasks, background tasks. run at lower clock speed and voltages
        performance core is clocked higher(3.4Ghz) and is focusing on highly perfomative tasks
            both of these are used at the same time and are specifically utilized with their respective tasks

How does a GPU work
    GPU is for calculating massive ammounts of data simultaneously. This is perfect for rendering.
    Compared to CPU's 4 cores, a GPU has thousands of smaller simpler cores that all work in parallel.

    for graphics
        the CPU sends geometry to the GPU
        the vertex shader transforms 3d coordingates into 2d screen space
        Rasterization figures out which pixels each triangle covers
        The fragment/pixel shader calculates the color of each pixel (lighting, textures, shadows)
        The final image is written to the framebuffer and sent to your display

What happens when you turn on your PC 
