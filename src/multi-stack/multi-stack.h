#ifndef MULTI_STACK_H
#define MULTI_STACK_H

#include <stdbool.h>

#define STACK_SIZE 3

struct MultiStack {
  int number_of_stacks;
  struct Stack *top_stack;
};

/* Initialize the MultiStack pointer ms. */
void MultiStack_init(MultiStack *ms);

/* Returns the pointer at the top of the MultiStack *ms */
void *MultiStack_pop(MultiStack *ms);

/* Pushes the data pointer data onto the MultiStack pointed to by ms.
 * If should_free_when_destroyed is true, then MultiStack_destroy will
 * free the memory pointed to by data. */
int MultiStack_push(MultiStack *ms, void *data,
                    bool should_free_when_destroyed);

/* Returns the number of elements in the ms MultiStack pointer. */
int MultiStack_num_elements(MultiStack *ms);

/* Free all memory allocated for the MultiStack pointed to by ms. */
int MultiStack_destroy(MultiStack *ms);

#endif
