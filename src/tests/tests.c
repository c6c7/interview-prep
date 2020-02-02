#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cystring.h"
#include "hanoi.h"
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

void test_MultiStack() {
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
}

void test_array_to_str() {
  int test_int_array[] = {1, 2, 3};
  char *test_int_array_str = int_array_to_str(test_int_array, 3);
  printf("%s\n", test_int_array_str);
  free(test_int_array_str);

  double test_array[] = {1.0, 2.0, 3.0};
  char *test_array_str = double_array_to_str(test_array, 3);
  printf("%s\n", test_array_str);
  free(test_array_str);
}

void test_TowerOfHanoi() {
  TowerOfHanoi *t = malloc(sizeof(TowerOfHanoi));
  TowerOfHanoi_init(t, 4);
  char *t_str = TowerOfHanoi_to_str(t);
  printf("%s", t_str);
  free(t_str);

  TowerOfHanoi *t_copy = TowerOfHanoi_dup(t);
  t_str = TowerOfHanoi_to_str(t);
  printf("%s", t_str);
  free(t_str);

  TowerOfHanoi_destroy(t);
  TowerOfHanoi_destroy(t_copy);
  free(t_copy);

  TowerOfHanoi_init(t, 5);
  t_str = TowerOfHanoi_solution_to_str(t);
  printf("%s", t_str);
  free(t_str);

  TowerOfHanoi_solve(t, TowerA, TowerC, TowerB);
  t_str = TowerOfHanoi_solution_to_str(t);
  printf("%s", t_str);
  free(t_str);
  TowerOfHanoi_solution_destroy(t);
}

int main(int argc, char *argv[]) {
  const char A[] = "A";
  const char B[] = "B";
  char *composed_AB = compose_str("%s%s", A, B);

  str_match_test("AB", composed_AB);
  free(composed_AB);

  // test_MultiStack();
  test_array_to_str();

  test_TowerOfHanoi();

  printf("%d of %d tests passed.\n", SUCCESSES, SUCCESSES + FAILURES);

  if (FAILURES > 0) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
