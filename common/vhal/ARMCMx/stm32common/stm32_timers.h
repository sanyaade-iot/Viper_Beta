#ifndef __STM32_TIMERS__
#define __STM32_TIMERS__

struct _pwmconfig {
  uint16_t ccer;
  uint16_t ccmr;
  uint16_t cr2;
};

struct _icuconfig {
  uint16_t ccer;
  uint16_t ccmr;
};


typedef struct _htm_struct {
  htmFn fn;
  void *args;
  VThread *thread;
} HTM_TypeDef;


typedef struct _icu_struct {
  uint32_t time_window;
  uint32_t cfg;
  uint32_t *buffer;
  uint32_t *bufsize;
  uint16_t count;
  uint16_t last;
  uint16_t updated;
  uint8_t started;
  uint8_t unused;
  uint32_t has_capture;
  uint32_t start_capture;
  VThread thread;
} ICU_TypeDef;

typedef struct _pwm_struct {
  VThread *thread;
  int32_t npulses;
  uint16_t vpin;
} PWM_TypeDef;
#endif