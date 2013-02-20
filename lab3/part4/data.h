#include "wheel.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef data_h
#define data_h


typedef struct {
  Wheel *wheel;
  uint8_t direction;
} Task;

typedef struct {
  Wheel *wheels[TASK_LIST_SIZE];
  uint8_t directions[TASK_LIST_SIZE];
  uint16_t times[TASK_LIST_SIZE];
  uint8_t numTasks;
} TaskList;

#endif

#ifdef __cplusplus
}
#endif
