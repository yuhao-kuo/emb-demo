
#ifndef __LED_BAR_H__
#define __LED_BAR_H__

#include <stdint.h>
#include <stdbool.h>

#define GLB_CMDMODE 0x00  // Work on 8-bit mode
#define GLB_ON      0xff  // 8-bit 1 data
#define GLB_OFF     0x00  // 8-bit 0 data


typedef struct led_bar_s ledbar_t;
struct led_bar_s {
    void *priv_args;
    struct led_bar_opt *opt;
};

struct led_bar_opt {
    void (*setGreenToRed)(ledbar_t *, bool);
    void (*setLevel)(ledbar_t *, float);
    void (*setLED)(ledbar_t *, uint8_t, float);
    void (*toggleLED)(ledbar_t *, uint8_t);
    void (*setBits)(ledbar_t *, uint32_t);
    uint32_t (*getBits)(ledbar_t *);
};

typedef struct {
    uint32_t port;
    uint32_t pin;
} ledbar_pin_t;

typedef uint32_t ledbar_size_t;

typedef void (*ledbar_portctl_t)(ledbar_pin_t, bool);
typedef void (*ledbar_delay_ms_t)(uint32_t msec);

extern ledbar_t *LEDBAR_New(ledbar_portctl_t func, ledbar_delay_ms_t delay, ledbar_pin_t clkport, ledbar_pin_t datport, bool greenToRed);
extern void      LEDBAR_Free(ledbar_t *ledbar);

extern void      LEDBAR_SetGreenToRed(ledbar_t *ledbar, bool greenToRed);
extern void      LEDBAR_SetLevel(ledbar_t *ledbar, float level);
extern void      LEDBAR_SetLED(ledbar_t *ledbar, uint8_t led, float brightness);
extern void      LEDBAR_ToggleLED(ledbar_t *ledbar, uint8_t led);
extern void      LEDBAR_SetBits(ledbar_t *ledbar, uint32_t bits);
extern uint32_t  LEDBAR_GetBits(ledbar_t *ledbar);



#endif
