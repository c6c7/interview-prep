#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cystring.h"
#include "multi-stack.h"

int SUCCESSES = 0;
int FAILURES = 0;

void str_match_test(const char *s1, const char *s2) {
  printf("Comparing '%s' to '%s' ... ", s1, s2);
  bool result = strcmp(s1, s2) == 0;
  printf("%s\n", result ? "PASS" : "FAIL");

  result ? SUCCESSES++ : FAILURES++;
}

char *double_formatter(void *d_ptr) {
  return compose_str("%.3e", *(double *)d_ptr);
}

int main(int argc, char *argv[]) {
  const char A[] = "A";
  const char B[] = "B";
  char *composed_AB = compose_str("%s%s", A, B);

  str_match_test("AB", composed_AB);
  free(composed_AB);

  struct MultiStack *ms = malloc(sizeof(struct MultiStack));
  MultiStack_init(ms);
  double some_numbers[5] = {1, 2, 3, 4, 5};
  for (int i = 0; i < 5; i++) {
    printf("Pushing %.3e...\n", some_numbers[i]);
    assert(MULTI_STACK_PUSH_SUCCESS ==
           MultiStack_push(ms, some_numbers + i, false));
  }

  printf("%s", MultiStack_to_string(ms, double_formatter));

  for (int i = 0; i < 2; i++) {
    printf("Popped %.3e\n", *(double *)MultiStack_pop(ms));
  }

  printf("%s", MultiStack_to_string(ms, double_formatter));

  for (int i = 0; i < 5; i++) {
    printf("Pushing %.3e...\n", some_numbers[i]);
    assert(MULTI_STACK_PUSH_SUCCESS ==
           MultiStack_push(ms, some_numbers + i, false));
  }

  printf("%s", MultiStack_to_string(ms, double_formatter));

  for (int i = 0; i < 8; i++) {
    printf("Popped %.3e\n", *(double *)MultiStack_pop(ms));
  }

  printf("%s", MultiStack_to_string(ms, double_formatter));

  printf("Popped %p\n", MultiStack_pop(ms));

  MultiStack_destroy(ms);

  printf("%d of %d tests passed.\n", SUCCESSES, SUCCESSES + FAILURES);

  if (FAILURES > 0) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
