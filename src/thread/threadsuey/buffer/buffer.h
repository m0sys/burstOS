/* Circular buffer where producers/consumers can insert and remove itms from.*/
#pragma once

typedef int buffer_item;
#define BUFFER_SZ 5

int insert_item(buffer_item item);
int rm_itme(buffer_item *item_ptr);
