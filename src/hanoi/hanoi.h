#ifndef HANOI_H
#define HANOI_H

typedef struct Disk {
  int size;
} Disk;

typedef struct Tower {
  struct Disk *_space;
  int _capacity;
  struct Disk *bottom;
  struct Disk *top;
} Tower;

typedef enum TowerLabel { TowerA = 0, TowerB = 1, TowerC = 2 } TowerLabel;

typedef struct TowerOfHanoi {
  Tower *towers[3];
  struct TowerOfHanoi *next_step;
} TowerOfHanoi;

char *Tower_to_str(Tower *t);
char *TowerOfHanoi_to_str(TowerOfHanoi *toh);
char *TowerOfHanoi_solution_to_str(TowerOfHanoi *toh);

Disk Tower_remove_disk(Tower *t);
void Tower_add_disk(Tower *t, Disk d);
int Tower_num_disks(Tower *t);

void Tower_init(Tower *t, int num_disks, int capacity);
void Tower_destroy(Tower *t);
void TowerOfHanoi_destroy(TowerOfHanoi *toh);
void TowerOfHanoi_solution_destroy(TowerOfHanoi *toh);
Tower *Tower_dup(Tower *t);

void TowerOfHanoi_init(TowerOfHanoi *t, int num_disks);
TowerOfHanoi *TowerOfHanoi_dup(TowerOfHanoi *t);
TowerOfHanoi *TowerOfHanoi_solve(TowerOfHanoi *toh, TowerLabel origin,
                                 TowerLabel dest, TowerLabel other);
#endif
