
#include "../ubigint.h"
#include "stdlib.h"
#include "stdio.h"

int main(void)
{
  ubigint_initialize_sys(malloc, free);
  ubigint_t a, b, out;
  if(ubigint_alloc(&a, 0)) return 1;
  if(ubigint_alloc(&b, 0)) return 1;
  if(ubigint_alloc(&out, 0)) return 1;
  ubigint_set_value(&a, ~((uint32_t)0));
  ubigint_set_value(&b, 257);
  ubigint_add(&out, &a, &b);
  printf("out > a ? %d\n", ubigint_gt(&out, &a));
  ubigint_add(&out, &out, &a);
  ubigint_sub(&out, &out, &b);
  return 0;
}
