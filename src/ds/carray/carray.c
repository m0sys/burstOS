#define BUFF_SZ 10

/*
 * Conditions:
 * in: points to next available pos.
 * out: points to first item in buffer.
 * full: (in + 1) % BUFF_SZ == out.
 * empty: in == out.
 */

static int in = 0;
static int out = 0;
int buffer[BUFF_SZ] = {0};

int is_empty(void) { return in == out; }

int is_full(void) { return (in + 1) % BUFF_SZ == out; }

int produce(int item) {
  if (is_full()) {
    return 0;
  }

  buffer[in] = item;
  in = (in + 1) % BUFF_SZ;
  return 1;
}

int consume(int *res) {
  if (is_empty())
    return 0;

  *res = buffer[out];
  out = (out + 1) % BUFF_SZ;
  return 1;
}
