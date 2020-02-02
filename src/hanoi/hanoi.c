#include "hanoi.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cystring.h"
#include "stack.h"

#define UNUSED __attribute__((unused))

int *Tower_to_array(Tower *t);

int *Tower_to_array(Tower *t) {
  int num_disks = t->top - t->bottom;
  int *disk_size_array = malloc(sizeof(int) * num_disks);
  for (int i = 0; i < num_disks; i++) {
    disk_size_array[i] = t->bottom[i].size;
  }
  return disk_size_array;
}

char *Tower_to_str(Tower *t) {
  int *disk_size_array = Tower_to_array(t);
  char *str = int_array_to_str(disk_size_array, t->top - t->bottom);
  free(disk_size_array);
  return str;
}

char *TowerOfHanoi_to_str(TowerOfHanoi *toh) {
  char *str_a = Tower_to_str(toh->towers[TowerA]);
  char *str_b = Tower_to_str(toh->towers[TowerB]);
  char *str_c = Tower_to_str(toh->towers[TowerC]);
  char *rtn_str = compose_str("%s\n%s\n%s\n", str_a, str_b, str_c);
  free(str_a);
  free(str_b);
  free(str_c);
  return rtn_str;
}

void *TowerOfHanoi_solution_walk(TowerOfHanoi *toh,
                                 void *(*fn)(TowerOfHanoi *, void *)) {
  void *rtn_ptr = NULL;
  TowerOfHanoi *next_toh;
  for (; toh != NULL; toh = next_toh) {
    next_toh = toh->next_step;
    rtn_ptr = fn(toh, rtn_ptr);
  }
  return rtn_ptr;
}

char *TowerOfHanoi_this_state_to_str(TowerOfHanoi *toh, char *last_str) {
  if (last_str == NULL) {
    last_str = strdup("");
  }
  char *str_state = TowerOfHanoi_to_str(toh);
  char *str = compose_str("%s\n>>>>>\n\n%s", last_str, str_state);
  free(str_state);
  free(last_str);
  return str;
}

char *TowerOfHanoi_solution_to_str(TowerOfHanoi *toh) {
  return TowerOfHanoi_solution_walk(
      toh, (void *(*)(TowerOfHanoi *, void *))TowerOfHanoi_this_state_to_str);
}

void TowerOfHanoi_destroy(TowerOfHanoi *toh) {
  Tower_destroy(toh->towers[TowerA]);
  free(toh->towers[TowerA]);
  Tower_destroy(toh->towers[TowerB]);
  free(toh->towers[TowerB]);
  Tower_destroy(toh->towers[TowerC]);
  free(toh->towers[TowerC]);
}

void *TowerOfHanoi_destroy_wrapper(TowerOfHanoi *toh, UNUSED void *_) {
  TowerOfHanoi_destroy(toh);
  free(toh);
  return NULL;
}

void TowerOfHanoi_solution_destroy(TowerOfHanoi *toh) {
  TowerOfHanoi_solution_walk(toh, TowerOfHanoi_destroy_wrapper);
}

void Tower_add_disk(Tower *t, Disk d) {
  *(t->top++) = d;
  assert(Tower_num_disks(t) <= t->_capacity);
}

Disk Tower_remove_disk(Tower *t) {
  assert(Tower_num_disks(t) > 0);
  return *(--t->top);
}

int Tower_num_disks(Tower *t) { return t->top - t->bottom; }

void Tower_init(Tower *t, int num_disks, int capacity) {
  t->_capacity = capacity;
  t->_space = malloc(sizeof(Disk) * capacity);
  t->bottom = t->_space;
  t->top = t->_space;

  for (int i = num_disks; i > 0; i--) {
    Tower_add_disk(t, (Disk){i});
  }
}

void Tower_destroy(Tower *t) { free(t->_space); }

Tower *Tower_dup(Tower *t) {
  Tower *new_t = malloc(sizeof(Tower));
  new_t->_capacity = t->_capacity;
  new_t->_space = malloc(new_t->_capacity * sizeof(Disk));
  new_t->bottom = new_t->_space + (t->bottom - t->_space);
  new_t->top = new_t->_space + (t->top - t->_space);
  memcpy(new_t->_space, t->_space, new_t->_capacity * sizeof(Disk));
  return new_t;
}

TowerOfHanoi *TowerOfHanoi_dup(TowerOfHanoi *toh) {
  TowerOfHanoi *new_toh = malloc(sizeof(TowerOfHanoi));
  new_toh->towers[TowerA] = Tower_dup(toh->towers[TowerA]);
  new_toh->towers[TowerB] = Tower_dup(toh->towers[TowerB]);
  new_toh->towers[TowerC] = Tower_dup(toh->towers[TowerC]);
  new_toh->next_step = toh->next_step;
  return new_toh;
}

void TowerOfHanoi_init(TowerOfHanoi *toh, int num_disks) {
  toh->towers[TowerA] = malloc(sizeof(Tower));
  toh->towers[TowerB] = malloc(sizeof(Tower));
  toh->towers[TowerC] = malloc(sizeof(Tower));
  Tower_init(toh->towers[TowerA], num_disks, num_disks);
  Tower_init(toh->towers[TowerB], 0, num_disks);
  Tower_init(toh->towers[TowerC], 0, num_disks);
  toh->next_step = NULL;
}

void TowerOfHanoi_lower_bottom_in_each(TowerOfHanoi *toh, TowerLabel a) {
  for (TowerOfHanoi *toh_to_transform = toh; toh_to_transform != NULL;
       toh_to_transform = toh_to_transform->next_step) {
    toh_to_transform->towers[a]->bottom--;
    assert(toh_to_transform->towers[a]->bottom >=
           toh_to_transform->towers[a]->_space);
  }
}

TowerOfHanoi *TowerOfHanoi_solve(TowerOfHanoi *toh, TowerLabel origin,
                                 TowerLabel dest, TowerLabel other) {
  assert(Tower_num_disks(toh->towers[dest]) == 0);
  assert(Tower_num_disks(toh->towers[other]) == 0);
  if (Tower_num_disks(toh->towers[origin]) == 0) {
    toh->next_step = NULL;
    return toh;
  }

  // Solve the problem while ignoring the bottom disk at the origin
  toh->towers[origin]->bottom++;
  TowerOfHanoi *last_toh = TowerOfHanoi_solve(toh, origin, other, dest);
  TowerOfHanoi_lower_bottom_in_each(toh, origin);

  // Move the bottom disk at origin to the destination
  TowerOfHanoi *next_toh = TowerOfHanoi_dup(last_toh);
  last_toh->next_step = next_toh;
  Disk d = Tower_remove_disk(next_toh->towers[origin]);
  assert(Tower_num_disks(next_toh->towers[origin]) == 0);
  Tower_add_disk(next_toh->towers[dest], d);

  // Finally, solve the remainder of the problem while ignoring the bottom disk
  // at destination
  next_toh->towers[dest]->bottom++;
  assert(Tower_num_disks(next_toh->towers[dest]) == 0);
  last_toh = TowerOfHanoi_solve(next_toh, other, dest, origin);
  TowerOfHanoi_lower_bottom_in_each(next_toh, dest);
  return last_toh;
}
