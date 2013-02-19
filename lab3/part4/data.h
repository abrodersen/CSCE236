#include "wheel.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef data_h
#define data_h


typedef struct {
  Wheel *wheel;
  uint8_t direction;
} Transition;

#endif

#ifdef __cplusplus
}
#endif
