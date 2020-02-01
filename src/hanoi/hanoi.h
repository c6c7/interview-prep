#ifndef HANOI_H
#define HANOI_H

struct TowerOfHanoi {
  int *tower_A;
  int tower_A_size;
  int *tower_B;
  int tower_B_size;
  int *tower_C;
  int tower_C_size;
};

char *TowerOfHanoi_to_str(struct TowerOfHanoi *t);

#endif
