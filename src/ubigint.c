
#include "ubigint.h"
#include "string.h"

#define MAX_STORAGE_VALUE (~((storage_t)0))
#define LSB_MASK MAX_STORAGE_VALUE
#define BITS_IN_BYTE 8
#define STORAGE_BIT_COUNT (sizeof(storage_t)*BITS_IN_BYTE) 
#define MSB_SHIFT STORAGE_BIT_COUNT



typedef struct ubigint_sys_s{
  malloc_t malloc_f;
  free_t free_f;
  int last_error;
} ubigint_sys_t;

static ubigint_sys_t ubigint_sys;

int ubigint_realloc(ubigint_t* i, size_t new_capacity);

void ubigint_initialize_sys(malloc_t malloc_f, free_t free_f)
{
  ubigint_sys.malloc_f = malloc_f;
  ubigint_sys.free_f = free_f;
  ubigint_sys.last_error = UBIGINT_ERR_NONE;
}

int ubigint_alloc(ubigint_t* i, size_t initial_size)
{
  if(i == NULL)
  {
    ubigint_sys.last_error = UBIGINT_ERR_ARG;
    return 1;
  }
  storage_t* new_data = NULL;
  size_t alloc_size = sizeof(storage_t) * initial_size;
  if(alloc_size > 0)
  {
    new_data = (storage_t*)ubigint_sys.malloc_f(alloc_size);
    if(new_data == NULL)
    {
      ubigint_sys.last_error = UBIGINT_ERR_MALLOC;
      return 1;
    }
  }
  i->data = new_data;
  i->data_len = 0;
  i->data_capacity = initial_size;
  i->buff_size = alloc_size;
  return 0;
}

int ubiginit_free(ubigint_t* i)
{
  if(i == NULL)
  {
    ubigint_sys.last_error = UBIGINT_ERR_ARG;
    return 1;
  }
  if(i->data) ubigint_sys.free_f(i->data);
  return 0;
}

int ubigint_set_value(ubigint_t* i, storage_t value)
{
  if(i == NULL)
  {
    ubigint_sys.last_error = UBIGINT_ERR_ARG;
    return 1;
  }
  // if this bigint was initialized with a size of zero
  if(i->data == NULL)
  {
    storage_t* new_data = (storage_t*)ubigint_sys.malloc_f(sizeof(storage_t));
    if(new_data == NULL)
    {
      ubigint_sys.last_error = UBIGINT_ERR_MALLOC;
      return 1;
    }
    i->data_capacity = 1;
    i->buff_size = sizeof(storage_t);
    i->data = new_data;
  }
  i->data[0] = value;
  i->data_len = 1;
  return 0;
}

int ubigint_copy(ubigint_t* dst, ubigint_t* src)
{
  if(dst == NULL || src == NULL)
  {
    ubigint_sys.last_error = UBIGINT_ERR_ARG;
  }
  if(dst->data_capacity < src->data_capacity)
  {
    if(dst->data) ubigint_sys.free_f(dst->data);
    storage_t* new_data = (storage_t*)ubigint_sys.malloc_f(src->buff_size);
    if(new_data == NULL)
    {
      ubigint_sys.last_error = UBIGINT_ERR_MALLOC;
      return 1;
    }
    dst->buff_size = src->buff_size;
    dst->data_capacity = src->data_capacity;
  }
  memcpy(dst->data, src->data, (src->data_len * sizeof(storage_t)));
  dst->data_len = src->data_len;
  return 0;
}

int ubigint_as_string(ubigint_t* i, char* buffer, size_t bf_len, int radix)
{
  if(buffer == NULL || bf_len < 1 || radix < 2)
  {
    ubigint_sys.last_error = UBIGINT_ERR_ARG;
  }
  size_t data_index, bit_index = STORAGE_BIT_COUNT;
  ubigint_t working_n;

  bit_index *= i->data_len;
  if(ubigint_alloc(&working_n, 0)) return 1;
  if(ubigint_copy(&working_n, i)) return 1;

  for(; bit_index >= 0; bit_index--)
  {
    
  }
}

int ubigint_lt(ubigint_t* l, ubigint_t* r)
{
  if(l == NULL || r == NULL)
  {
    ubigint_sys.last_error = UBIGINT_ERR_ARG;
    return -1;
  }
  // simple case
  if(l->data_len != r->data_len) return l->data_len < r->data_len;
  // iterate over each data entry until differing entires are encountered
  storage_t lv, rv;
  for(size_t idx = l->data_len - 1; idx >= 0; idx--)
  {
    lv = l->data[idx];
    rv = r->data[idx];
    if(lv != rv) return lv < rv;
  }
  // if they are equal, return false
  return 0;
}

int ubigint_gt(ubigint_t* l, ubigint_t* r)
{
  if(l == NULL || r == NULL)
  {
    ubigint_sys.last_error = UBIGINT_ERR_ARG;
    return -1;
  }
  // simple case
  if(l->data_len != r->data_len) return l->data_len > r->data_len;
  // iterate over each data entry until differing entires are encountered
  storage_t lv, rv;
  for(size_t idx = l->data_len - 1; idx >= 0; idx--)
  {
    lv = l->data[idx];
    rv = r->data[idx];
    if(lv != rv) return lv > rv;
  }
  // if they are equal, return false
  return 0;
}

int ubigint_eq(ubigint_t* l, ubigint_t* r)
{
  if(l == NULL || r == NULL)
  {
    ubigint_sys.last_error = UBIGINT_ERR_ARG;
    return -1;
  }
  // simple case
  if(l->data_len != r->data_len) return 0;
  // iterate over each data entry until differing entires are encountered
  storage_t lv, rv;
  for(size_t idx = l->data_len - 1; idx >= 0; idx--)
  {
    lv = l->data[idx];
    rv = r->data[idx];
    if(lv != rv) return 0;
  }
  // if they are equal, return false
  return 1;
}

int ubigint_neq(ubigint_t* l, ubigint_t* r)
{
  return !ubigint_eq(l, r);
}

int ubigint_add(ubigint_t* out, ubigint_t* l, ubigint_t* r)
{
  // cannot compute with unset values
  if(!l->data_len || !r->data_len || l == NULL || r == NULL || out == NULL)
  {
    ubigint_sys.last_error = UBIGINT_ERR_ARG;
    return 1;
  }
  size_t larger_len;
  overflow_t overflow;
  storage_t lsb, msb = 0;
  // determine the length of the larger number
  larger_len = l->data_len > r->data_len ? l->data_len : r->data_len;
  // ensure we can fit result and overflow
  while(larger_len + 1 > out->data_capacity)
  {
    if(ubigint_realloc(out, larger_len + 1)) return 1;
  }
  for(size_t idx = 0; idx < larger_len; idx++)
  {
    // value at a given index is sum of lr values at that index
    // plus overflow from previous index
    overflow = msb;
    overflow += idx < l->data_len ? l->data[idx] : 0;
    overflow += idx < r->data_len ? r->data[idx] : 0;
    lsb = (storage_t)(overflow & LSB_MASK);
    msb = (storage_t)(overflow >> MSB_SHIFT);
    out->data[idx] = lsb;
  }
  out->data_len = larger_len;
  if(msb > 0)
  {
    out->data[larger_len] = msb;
    out->data_len++;
  }
  return 0;
}

int ubigint_sub(ubigint_t* out, ubigint_t* l, ubigint_t* r)
{
  if(out == NULL || l == NULL || r == NULL)
  {
    ubigint_sys.last_error = UBIGINT_ERR_ARG;
    return 1;
  }
  // if l is smaller than r, cannot compute
  if(ubigint_lt(l, r))
  {
    ubigint_sys.last_error = UBIGINT_NEG_RESULT;
    return 1;
  }
  signed_t diff = 0;
  // ensure we can fit result
  if(l->data_len >= out->data_capacity)
  {
    if(ubigint_realloc(out, l->data_len)) return 1;
  }
  for(size_t idx = 0; idx < l->data_len; idx++)
  {
    diff += l->data[idx];
    diff -= (signed_t)(idx < r->data_len ? r->data[idx] : 0);
    if(diff >= 0)
    {
      out->data[idx] = diff;
    }
    else
    {
      out->data[idx] = (storage_t)(MAX_STORAGE_VALUE + diff + 1);
      diff = -1; // carry difference over to next word
    }
  }
  return 0;
}

int ubigint_realloc(ubigint_t* i, size_t new_capacity)
{
  size_t alloc_size;
  storage_t* new_data;

  if(i == NULL)
  {
    ubigint_sys.last_error = UBIGINT_ERR_ARG;
    return 1;
  }

  i->data_capacity = new_capacity;
  alloc_size = i->data_capacity * sizeof(storage_t);
  // allocate new data buffer
  new_data = (storage_t*)ubigint_sys.malloc_f(alloc_size);
  if(new_data == NULL)
  {
    ubigint_sys.last_error = UBIGINT_ERR_MALLOC;
    return 1;
  }
  if(i->data)
  {
    // copy data from old buffer to new
    memcpy(new_data, i->data, i->buff_size);
    // free memory from old buffer
    ubigint_sys.free_f(i->data);
  }
  // update data pointer
  i->data = new_data;
  // indicate new buffer size
  i->buff_size = alloc_size;
  return 0;
}
