#ifdef __cplusplus
extern "C" {
#endif

#ifndef wheel_h
#define wheel_h

typedef struct {
  Servo motor;
  uint8_t pin;
  uint8_t state;
  int8_t tuneValue;
  bool reversed;
} Wheel;

void initialize(Wheel *wheel);

void turnWheel(Wheel *wheel, uint8_t direction);

#endif

#ifdef __cplusplus
}
#endif
