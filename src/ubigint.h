
#include "stdint.h"
#include <stddef.h>

typedef uint32_t storage_t;
typedef uint64_t overflow_t;
typedef int64_t  signed_t;

typedef struct ubigint{
  storage_t* data;
  size_t data_len; // value = zero indicates unset
  size_t data_capacity;
  size_t buff_size;
} ubigint_t;

typedef enum ubigint_err{
  UBIGINT_ERR_NONE = 0,
  UBIGINT_ERR_MALLOC,
  UBIGINT_ERR_ARG,
  UBIGINT_NEG_RESULT
} ubigint_err_t;

typedef void*(*malloc_t)(size_t);
typedef void(*free_t)(void*);

void ubigint_initialize_sys(malloc_t mf, free_t ff);

int ubigint_alloc(ubigint_t* i, size_t is);

int ubigint_free(ubigint_t* i);

int ubigint_set_value(ubigint_t* i, storage_t iv);

int ubigint_copy(ubigint_t* dst, ubigint_t* src);

int ubigint_as_string(ubigint_t* i, char* buff, size_t bs, int r);

int ubigint_lt(ubigint_t* l, ubigint_t* r);

int ubigint_gt(ubigint_t* l, ubigint_t* r);

int ubigint_eq(ubigint_t* l, ubigint_t* r);

int ubigint_neq(ubigint_t* l, ubigint_t* r);

int ubigint_add(ubigint_t* out, ubigint_t* l, ubigint_t* r);

int ubigint_sub(ubigint_t* out, ubigint_t* l, ubigint_t* r);
