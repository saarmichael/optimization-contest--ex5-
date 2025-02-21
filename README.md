# Optimization Contest

In this project, I was given a very slow piece of code and tasked with improving its runtime as much as possible.
This was essentially a competition among the entire class (about 250 students), with grades awarded based on the improvement in execution time compared to the original code.

<img src="images/slow_code_run.png" width="400"> <img src="images/fast_code_run.png" width="470">  
The first image shows the runtime of the original slow code, which took 80ms, while the second image shows the optimized version—16 times faster!

## What Does the Code Do?
The file *myfunction.c* processes an input *.bmp* file, a bitmap image format where each pixel is represented by three bytes—Red, Green, and Blue (RGB). The second parameter specifies the type of **filter** to be applied to the image.

## How Does the Slow Code Work?
Let's break it down to highlight the inefficiencies and the optimizations I made.

### Data Representation
The basic data unit in the image is a pixel, represented by:
```c
typedef struct {  
    unsigned char red;  
    unsigned char green;  
    unsigned char blue;  
} pixel;
```

Since a BMP image is stored as a two-dimensional array of bytes, where every three consecutive bytes represent one pixel, the first operation in the slow code was converting this byte array into an array of *pixel* structures.

Next, the code iterates over the image and applies a filter to each pixel, modifying all pixels in the process to generate a new, filtered image.

### Applying a Filter
Each pixel and its eight neighboring pixels form a 3x3 square. Depending on the filter option provided as a command-line argument (1 or 2), the function performs calculations on this nine-pixel square.

#### Inefficiencies in the Original Code
The original implementation contained several inefficiencies:
- **Nested Loops:** Iterating over 9 pixels per iteration using multiple for-loops.
- **Excessive Function Calls:** Using separate function calls for basic calculations, adding unnecessary overhead.
- **Index-Based Memory Access:** Each pixel access required index calculations, significantly slowing down execution.

Example of inefficient code:
```c
static pixel applyKernel(int dim, int i, int j, pixel *src, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {
    int ii, jj;
    pixel_sum sum;
    initialize_pixel_sum(&sum);
    int istart = max(i-1, 0);
    int istop = min(i+1, dim-1);
    for (ii = istart; ii <= istop; ii++) {
        for (jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) {
            int kRow = (ii < i) ? 0 : (ii > i) ? 2 : 1;
            int kCol = (jj < j) ? 0 : (jj > j) ? 2 : 1;
            sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);
        }
    }
```
This is just one example, but similar inefficient patterns were present throughout the code.

## How Did I Optimize the Code?

### Eliminating Unnecessary Conversions
The first inefficiency I addressed was the conversion of the byte array into an array of *pixel* structures. While *pixel* structures improve code readability, the conversion process adds unnecessary overhead. I decided to work directly with the raw byte array instead.

### Identifying Bottlenecks
Using the **gprof** profiler, I identified that *applyKernel* (shown above) was the most time-consuming function.

### Using Pointer Arithmetic for Fast Access
To speed up pixel access, I introduced **register pointers** to directly reference the 3x3 pixel square:
```c
register unsigned char* a = src;
register unsigned char* x = src + 3;
register unsigned char* u = src + 6;
register unsigned char* b = src + dim;
register unsigned char* y = src + dim + 3;
register unsigned char* v = src + dim + 6;
register unsigned char* c = src + dim * 2;
register unsigned char* z = src + dim * 2 + 3;
register unsigned char* w = src + dim * 2 + 6;
```

### Why is This Faster?
1. **Pointer Arithmetic is Faster:** Instead of performing index calculations for every pixel access, the pointers are updated directly.
2. **Better Spatial Locality:** Since pixels are stored sequentially in memory, accessing them via pointers improves cache efficiency, reducing memory access time.
3. **Processing Multiple Pixels at Once:** Instead of working on a single 3x3 square per iteration, the optimized approach allows multiple pixel squares to be processed in parallel.

## Results
By implementing these optimizations, I was able to achieve a **16x speedup** compared to the original slow code. This significantly improved performance and ranked among the top solutions in the class.

---
This project was a great learning experience in performance optimization, profiling, and leveraging low-level memory operations for efficiency.

