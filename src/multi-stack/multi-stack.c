/*
 * Lookups
 *
 * How should structs be cleanly defined in header files? Can I hide private
 * properties?
 *
 * What is the best way to do an assert(false) to bail of the program in C?
 *
 * What is the best way to do an inline struct?
 *
 * What is the best way to check all malloc calls? SHould I use a macro?
 *
 */

// 1 hour. All I have left is to implement MultiStack_pop and MultiStack_destroy

#include "multi-stack.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "cystring.h"
#include "stack.h"

#include <stdio.h>

enum MULTI_STACK_ADD_STACK_REPORT {
  MULTI_STACK_ADD_STACK_SUCCESS = 0,
  MULTI_STACK_ADD_STACK_FAILURE
};

enum MULTI_STACK_ADD_STACK_REPORT MultiStack_add_stack(struct MultiStack *ms) {
  struct Stack *new_stack = malloc(sizeof(struct Stack));

  if (new_stack == NULL) return MULTI_STACK_ADD_STACK_FAILURE;

  Stack_init(new_stack);
  new_stack->next = ms->top_stack;
  ms->top_stack = new_stack;
  ms->number_of_stacks++;
  return MULTI_STACK_ADD_STACK_SUCCESS;
}

void MultiStack_init(struct MultiStack *ms) {
  ms->number_of_stacks = 0;
  ms->top_stack = malloc(sizeof(struct Stack));
  Stack_init(ms->top_stack);
}

void MultiStack_destroy(struct MultiStack *ms) {
  for (struct Stack *s = ms->top_stack; s != NULL;) {
    struct Stack *prev_s = s;
    s = s->next;
    Stack_destroy(prev_s);
    free(prev_s);
  }
}

void MultiStack_pop_stack(struct MultiStack *ms) {
  struct Stack *stack_to_free = ms->top_stack;

  assert(stack_to_free != NULL);
  assert(STACK_EMPTY == Stack_read_stack_condition(ms->top_stack));

  ms->top_stack = stack_to_free->next;
  free(stack_to_free);

  ms->number_of_stacks--;
}

void *MultiStack_pop(struct MultiStack *ms) {
  assert(ms->top_stack != NULL);
  // An empty stack is only encountered when it's the last one.
  if (STACK_EMPTY == Stack_read_stack_condition(ms->top_stack)) {
    assert(ms->top_stack->next == NULL);
    return NULL;
  }
  void *ptr = Stack_pop(ms->top_stack).data.ptr;
  if (STACK_EMPTY == Stack_read_stack_condition(ms->top_stack) &&
      ms->top_stack->next != NULL) {
    MultiStack_pop_stack(ms);
  }
  return ptr;
}

enum MULTI_STACK_PUSH_REPORT MultiStack_push(struct MultiStack *ms, void *data,
                                             bool should_free_when_destroyed) {
  struct Data d = (struct Data){data, should_free_when_destroyed};
  enum STACK_PUSH_REPORT push_report = Stack_push(ms->top_stack, d);

  switch (push_report) {
    case STACK_PUSH_SUCCESS:
      return 0;
    case STACK_PUSH_FAIL:
      // In this case we add more memory to the MultiStack and push again.
      // A push should never fail after adding more memory, *unless* this
      // program is out of memory.
      if (MultiStack_add_stack(ms) == MULTI_STACK_ADD_STACK_FAILURE) {
        // If we get to here, then MultiStack was unable to acquire more memory
        // to add another stack.
        return MULTI_STACK_PUSH_FAILURE;
      }
      push_report = Stack_push(ms->top_stack, d);
      assert(push_report == STACK_PUSH_SUCCESS);
      return MULTI_STACK_PUSH_SUCCESS;
    default:
      assert(false);
  }
}

char *MultiStack_to_string(struct MultiStack *ms,
                           char *(*data_formatter)(void *ptr)) {
  char *output = strdup("");

  int stack_number = ms->number_of_stacks;
  for (struct Stack *s = ms->top_stack; s != NULL; s = s->next) {
    char *prev_output = output;
    char *stack_str = Stack_to_string(s, data_formatter);
    output = compose_str("%sStack %d: %s\n", output, stack_number--, stack_str);
    free(stack_str);
    free(prev_output);
  }
  return output;
}
