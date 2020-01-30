#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#define STACK_SIZE 1

struct Data {
  void *ptr;
  bool should_free_when_destroyed;
};

struct OptionalData {
  struct Data data;
  bool exists;
};

struct Stack {
  struct Data buffer[STACK_SIZE];
  struct Data *top;
  struct Stack *next;
};

enum STACK_CONDITION { STACK_FULL = 0, STACK_EMPTY, STACK_READY };

enum STACK_PUSH_REPORT { STACK_PUSH_SUCCESS = 0, STACK_PUSH_FAIL };

enum STACK_CONDITION Stack_read_stack_condition(struct Stack *s);

struct OptionalData Stack_pop(struct Stack *s);

enum STACK_PUSH_REPORT Stack_push(struct Stack *s, struct Data d);

void Stack_init(struct Stack *s);

void Stack_destroy(struct Stack *s);

char *Stack_to_string(struct Stack *s, char *(*data_formatter)(void *ptr));

#endif
