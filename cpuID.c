#include <stdio.h>

int main() {
    int a[4];
    __asm__("mov $0x1 , %eax\n\t");
    __asm__("cpuid\n\t");
    __asm__("mov %%eax, %0\n\t":"=r" (a[0])); //gives model and family
    __asm__("mov %%ebx, %0\n\t":"=r" (a[1])); //gives additional feature info
    __asm__("mov %%ecx, %0\n\t":"=r" (a[2])); //feature flags
    __asm__("mov %%edx, %0\n\t":"=r" (a[3])); //feature flags
    printf("ecx: %d\n", a[2]);
    printf("edx: %d\n", a[3]);
    
    int b[3];
__asm__("mov $0x0, %eax\n\t");
__asm__("cpuid\n\t");
__asm__("mov %%ebx, %0\n\t":"=r" (b[0]));
__asm__("mov %%edx, %0\n\t":"=r" (b[1]));
__asm__("mov %%ecx, %0\n\t":"=r" (b[2]));
printf ("%s\n", &b);
    return 0;
}