#pragma once

/* Add new item onto the message queue. */
int produce(int item);

/* Return oldest item from the message queue. */
int consume(int *res);
