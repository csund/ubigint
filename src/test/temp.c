
#include "stdint.h"
#include "stdio.h"

int main(void)
{
  uint32_t a = ~((uint32_t)0);
  printf("%u | %x\n", a, a);
  return 0;
}
