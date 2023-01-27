#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
Stack *stack_create(int capacity, char name) {
  Stack *s = (Stack *)malloc(sizeof(Stack));
  if (!s) {
    return 0;
  }
  if (capacity < 1) {
    capacity = 1;
  }
  s->name = name;
  s->top = 0;
  s->capacity = capacity;
  s->items = (int *)malloc(sizeof(int) * capacity);
  if (!s->items) // if (s->items = NULL)
  {
    return 0;
  }
  return s;
}
void stack_delete(Stack *s) {
  free(s->items);
  free(s);
  return;
}
int stack_pop(Stack *s) {
  if (!s) {
    return -1;
  }
  if (s->top > 0) {
    s->top -= 1;
    return s->items[s->top];
  } else {
    return -1;
  }
}
void stack_push(Stack *s, int item) {
  if (!s) {
    return;
  }
  if (s->top == s->capacity) {
    s->capacity *= 2;
    s->items = (int *)realloc(s->items, s->capacity * sizeof(int));
  }
  if (s->items) {
    s->items[s->top] = item;
    s->top += 1;
  }
  return;
}
bool stack_empty(Stack *s) {
  return s->top == 0; //the return type is bool, return true when top == 0
}
int stack_peek(Stack *s) {
  if (!stack_empty(s)) {
    return s->items[s->top - 1];
  } else {
    return 0;
  }
}
