#include <hanoi.h>
#include "cystring.h"

char *TowerOfHanoi_to_str(struct TowerOfHanoi *t) {
  char *str =
      compose_str("%s\n", int_array_to_str(t->tower_A, t->tower_A_size));
  char *next =
      compose_str("%s%s\n", str, int_array_to_str(t->tower_B, t->tower_B_size));
  free(str);
  str = next;
  next =
      compose_str("%s%s\n", str, int_array_to_str(t->tower_C, t->tower_C_size));
  free(str);
  return next;
}
