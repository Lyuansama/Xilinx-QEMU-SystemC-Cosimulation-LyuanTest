#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

int main(int argc, char * argv[]) {
  volatile unsigned int *base, *address;
  unsigned long addr0, offset, value;
  unsigned long val, result;

  // Predefined addresses.
  addr0 = 0xa0800000ul;  // Set the address you want to get access to  here. "ul" means unsigned long.


  // Ensure proper usage, if you give more than one arguments in cmd, the program will throw out error.
  if(argc > 2)
  {
    printf("Usage: %s [val]\n",argv[0]);
    return -1;
  }

  // Open memory as a file
  int fd = open("/dev/mem", O_RDWR|O_SYNC);
  if(!fd)
    {
      printf("Unable to open /dev/mem.  Ensure it exists (major=1, minor=1)\n");
      return -1;
    }	

  // Map the physical base address to local pointer (in virtual address space)
  base = (unsigned int *)mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, addr0 & ~MAP_MASK);	
  if((base == MAP_FAILED))
  {
    printf("mapping failed\n");
    fflush(stdout);
  }

  // If you give only one argument, the program will pass the value to addr0. Otherwise, it will tell you the value stored in addr0 without input argument.
  if(argc > 1) {
    // Assign val
    val = atol(argv[1]);

    // Write to addr0
    address = base + ((addr0 & MAP_MASK)>>2);
    *address = val;
    printf("Writing %lu into 0x%lX\n", val, addr0);

  } else {

    // Read hardware 
    address = base + ((addr0 & MAP_MASK)>>2);
    result = *address;

    printf("The value in 0x%lX is %lu \n", addr0, result);

  }

  //In the end, unmap the base address and close the memory file
  munmap((void*)base, MAP_SIZE);
  close(fd);

  return 0;
}
