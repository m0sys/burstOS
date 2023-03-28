#include "buffer.h"

/* The global buffer shared by producers/consumers. */
buffer_item buffer[BUFFER_SZ];

int insert_item(buffer_item item) {
  return 0;  /* if succesful */
  return -1; /* if err */
}
int rm_itme(buffer_item *item_ptr) {
  return 0;  /* if succesful */
  return -1; /* if err */
}
