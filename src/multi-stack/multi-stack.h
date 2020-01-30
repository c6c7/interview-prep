#ifndef MULTI_STACK_H
#define MULTI_STACK_H

#include <stack.h>
#include <stdbool.h>

struct MultiStack {
  int number_of_stacks;
  struct Stack *top_stack;
};

enum MULTI_STACK_PUSH_REPORT {
  MULTI_STACK_PUSH_SUCCESS = 0,
  MULTI_STACK_PUSH_FAILURE
};

/* Initialize the MultiStack pointer ms. */
void MultiStack_init(struct MultiStack *ms);

/* Returns the pointer at the top of the MultiStack *ms */
void *MultiStack_pop(struct MultiStack *ms);

/* Pushes the data pointer data onto the MultiStack pointed to by ms.
 * If should_free_when_destroyed is true, then MultiStack_destroy will
 * free the memory pointed to by data. */
enum MULTI_STACK_PUSH_REPORT MultiStack_push(struct MultiStack *ms, void *data,
                                             bool should_free_when_destroyed);

/* Returns the number of elements in the ms MultiStack pointer. */
int MultiStack_num_elements(struct MultiStack *ms);

/* Free all memory allocated for the MultiStack pointed to by ms. */
void MultiStack_destroy(struct MultiStack *ms);

char *MultiStack_to_string(struct MultiStack *ms,
                           char *(*data_formatter)(void *ptr));

#endif
