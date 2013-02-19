#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  Servo motor;
  uint8_t pin;
  uint8_t state;
  int8_t tuneValue;
  bool reversed;
} Wheel;

typedef struct {
  Wheel *wheel;
  uint8_t direction;
} Transition;

#ifdef __cplusplus
}
#endif
