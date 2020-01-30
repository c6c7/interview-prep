#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include "cystring.h"

enum STACK_CONDITION Stack_read_stack_condition(struct Stack *s) {
  if (s->top - s->buffer == STACK_SIZE) {
    return STACK_FULL;
  }
  if (s->top == s->buffer) {
    return STACK_EMPTY;
  }
  return STACK_READY;
}

struct OptionalData Stack_pop(struct Stack *s) {
  if (STACK_EMPTY == Stack_read_stack_condition(s)) {
    return (struct OptionalData){(struct Data){NULL, false}, false};
  }
  return (struct OptionalData){*(--s->top), true};
}

enum STACK_PUSH_REPORT Stack_push(struct Stack *s, struct Data d) {
  if (STACK_FULL == Stack_read_stack_condition(s)) {
    return STACK_PUSH_FAIL;
  }
  *s->top = d;
  s->top++;
  return STACK_PUSH_SUCCESS;
}

void Stack_init(struct Stack *s) {
  s->top = s->buffer;
  s->next = NULL;
}

void Stack_destroy(struct Stack *s) {
  for (struct Data *d = s->buffer; d < s->top; d++) {
    if (d->should_free_when_destroyed) {
      free(d->ptr);
    }
  }
}

char *Stack_to_string(struct Stack *s, char *(*data_formatter)(void *ptr)) {
  char *output = strdup("");
  for (struct Data *d = s->buffer; d < s->top; d++) {
    char *prev_output = output;
    char *data_string = data_formatter(d->ptr);
    output =
        compose_str("%s%s%s", output, d == s->buffer ? "" : " ", data_string);
    free(prev_output);
    free(data_string);
  }
  return output;
}
