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

#include <assert.h>
#include <stdlib.h>  // mainly for malloc

struct Stack {
  struct Data buffer[STACK_SIZE];
  struct Data *top;
  struct Stack *next;
};

struct Data {
  void *ptr;
  bool should_free_when_destroyed;
}

enum STACK_CONDITION {
  STACK_FULL = 0; STACK_EMPTY; STACK_READY;
}

enum STACK_PUSH_REPORT { STACK_PUSH_SUCCESS = 0; STACK_PUSH_FAIL; }

enum STACK_STATE Stack_read_stack_condition(Stack *s) {
  if (s->top - s->buffer == STACK_SIZE) {
    return STACK_FULL;
  }
  if (s->top == s->buffer) {
    return STACK_EMPTY;
  }
  return STACK_READY;
}

enum STACK_PUSH_REPORT Stack_push(Stack *s, struct Data d) {
  enum STACK_CONDITION stack_condition = Stack_read_stack_condition(s);

  switch (stack_condition) {
    case STACK_EMPTY:
    case STACK_READY:
      s->top = d;
      s->top++;
      return STACK_PUSH_SUCCESS;
      break;
    case STACK_FULL:
      return STACK_PUSH_FAIL;
      break;
    default:
      assert(false);
  }
}

void Stack_init(Stack *s) {
  s->top = s->buffer;
  s->next = NULL;
}

int MultiStack_add_stack(MultiStack *ms) {
  struct Stack *new_stack = malloc(sizeof(struct Stack));

  if (new_stack == NULL) return -1;

  Stack_init(new_stack);
  new_stack->next = ms->top_stack;
  ms->top_stack = new_stack;
  ms->number_of_stacks++;
  return 0;
}

void MultiStack_init(MultiStack *ms) {
  ms->number_of_stacks = 0;
  ms->top_stack = malloc(sizeof(struct Stack));
  Stack_init(ms->top_stack);
}

void MultiStack_pop_stack(MultiStack *ms) {
  assert(STACK_EMPTY == Stack_read_condition(ms->top_stack));
  Stack *stack_to_free = ms->top_stack;

  assert(stack_to_free != NULL);

  ms->top_stack = stack_to_free->next;
  free(stack_to_free);
}

void *MultiStack_pop(MultiStack *ms) {}

int MultiStack_push(MultiStack *ms, void *data,
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
      if (MultiStack_add_stack(ms) < 0) {
        // If we get to here, then MultiStack was unable to acquire more memory
        // to add another stack.
        return -1;
      }
      push_report = Stack_push(ms->top_stack, d);
      assert(push_report == STACK_PUSH_SUCCESS);
      return 0;
    default:
      assert(false);
  }
}
