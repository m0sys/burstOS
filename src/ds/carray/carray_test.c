#include "carray.h"

#include <assert.h>

void carray_test_produce(void) {
  /* Test: produce item when empty. */
  assert(produce(10) != 0);
}

void carray_test_bundle(void) {
  //
  carray_test_produce();
}
