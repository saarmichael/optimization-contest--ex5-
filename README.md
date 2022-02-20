# optimization-contest

In this project I was given a very slow code and needed to improve its run time as much as possible.  
This was practically a contest between the whole class (about 250 students), when grades where given based on the improvement in running time compared to the original code.  
<img src="images/slow_code_run.png" width="400"> <img src="images/fast_code_run.png" width="470">  
The left was taken after running the slow code, which took 80ms, and the right picture is the optimized code- 16 times faster!
### What is the code doing?
The file *myfunction.c* gets as the first parameter a *.bmp* file, which is a picture format such as each pixel is represented by 3 bytes- Red, Green and Blue (RGB).
The second parameter is a number, defining what type of **filter** the function should apply on the given picture.  
### How does the slow Code work?
Let's dive in.  
I'll go through the general structure in order to later demonstrate the optimizations I made.

