
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "led_bar.h"

struct ledbar_pri_data {
    bool greenToRed;
    uint8_t *state;
    ledbar_size_t length;
    ledbar_pin_t clk_io;
    ledbar_pin_t dat_io;
    ledbar_portctl_t func;
    ledbar_delay_ms_t delay;
};

#define LEDBAR_MAX(a,b)    (((a) > (b)) ? (a) : (b))

#define LEDBAR_MIN(a,b)    (((a) < (b)) ? (a) : (b))

#define LEDBAR_PrivData(obj)                            \
            ((struct ledbar_pri_data *)obj->priv_args)

#define LEDBAR_CLKPort(obj)                             \
            LEDBAR_PrivData(obj)->clk_io

#define LEDBAR_DATPort(obj)                             \
            LEDBAR_PrivData(obj)->dat_io

#define LEDBAR_PortWriteBit(obj, port, io_stat)         \
            LEDBAR_PrivData(obj)->func(port, io_stat)

#define LEDBAR_ClkWriteBit(obj, io_stat)                \
            LEDBAR_PortWriteBit(obj, LEDBAR_CLKPort(obj), io_stat)

#define LEDBAR_DatWriteBit(obj, io_stat)                \
            LEDBAR_PortWriteBit(obj, LEDBAR_DATPort(obj), io_stat)


static void LEDBAR_SendData(ledbar_t *ledbar, uint32_t data)
{
    uint32_t state = 0;
    for(uint8_t i = 0; i < 16; i++) {
        bool s1 = data & 0x8000 ? true : false;
        LEDBAR_DatWriteBit(ledbar, s1);
        state = 1 - state;
        s1 = state ? true : false;
        LEDBAR_ClkWriteBit(ledbar, state);
        data <<= 1;
    }
}

static void LEDBAR_LatchData(ledbar_t *ledbar)
{
    LEDBAR_DatWriteBit(ledbar, false);
    // delay 10ms
    LEDBAR_PrivData(ledbar)->delay(10);

    for(uint8_t i = 0; i < 4; i++) {
        LEDBAR_DatWriteBit(ledbar, true);
        LEDBAR_DatWriteBit(ledbar, false);
    }
    
}

static void LEDBAR_SetData(ledbar_t *ledbar)
{
    struct ledbar_pri_data *pri = LEDBAR_PrivData(ledbar);

    LEDBAR_DatWriteBit(ledbar, GLB_CMDMODE);

    for(uint8_t i = 0; i < 10; i++) {
        if(pri->greenToRed) {
            LEDBAR_SendData(ledbar, *(pri->state + (10 - i - 1)));
        } else {
            LEDBAR_SendData(ledbar, *(pri->state + i));
        }
    }

    LEDBAR_SendData(ledbar, 0x00);
    LEDBAR_SendData(ledbar, 0x00);

    LEDBAR_LatchData(ledbar);
}

static uint8_t LEDBAR_FloatConvertToByte(float a)
{
    uint8_t r = a;
    return r;
}


void LEDBAR_SetGreenToRed(ledbar_t *ledbar, bool greenToRed)
{
    LEDBAR_PrivData(ledbar)->greenToRed = greenToRed;
    LEDBAR_SetData(ledbar);
}

void LEDBAR_SetLevel(ledbar_t *ledbar, float level)
{
    uint8_t *state = LEDBAR_PrivData(ledbar)->state;

    level = LEDBAR_MAX(0.0F, LEDBAR_MIN(10.0F, level));
    level *= 8;

    for(uint8_t i = 0; i < 10; i++) {
        *(state + i) = (level > 8)
                     ? ~0
                     : ((level > 0) ? ~(~0 << LEDBAR_FloatConvertToByte(level)) : 0);
        level -= 8;
    }
    LEDBAR_SetData(ledbar);
}

void LEDBAR_SetLED(ledbar_t *ledbar, uint8_t led, float brightness)
{
    led = LEDBAR_MAX((1), (LEDBAR_MIN(10, (int)led)));
    brightness = LEDBAR_MAX(0.0F, LEDBAR_MIN(brightness, 1.0F));

    led--;
    
    *(LEDBAR_PrivData(ledbar)->state + led) = ~(~0 << (uint8_t)brightness << 3);

    LEDBAR_SetData(ledbar);
}

void LEDBAR_ToggleLED(ledbar_t *ledbar, uint8_t led)
{
    led = LEDBAR_MAX(1, LEDBAR_MIN(10, (int) led));
    led--;
    *(LEDBAR_PrivData(ledbar)->state + led) = *(LEDBAR_PrivData(ledbar)->state + led) ? 0 : ~0;
    LEDBAR_SetData(ledbar);
}

void LEDBAR_SetBits(ledbar_t *ledbar, uint32_t bits)
{
    for(uint8_t i = 0; i < 10; i++) {
        *(LEDBAR_PrivData(ledbar)->state + i) = (bits & 0x01) ? 0xff : 0x00;
        bits >>= 1;
    }
    LEDBAR_SetData(ledbar);
}

uint32_t LEDBAR_GetBits(ledbar_t *ledbar) {
    uint32_t __bits = 0x0;
    for(uint8_t i = 0; i < 10; i++) {
        if(*(LEDBAR_PrivData(ledbar)->state + i) != 0x0) {
            __bits |= (0x01 << i);
        }
    }
    return __bits;
}

static const struct led_bar_opt ledbar_opts = {
    .setGreenToRed = LEDBAR_SetGreenToRed,
    .setLevel = LEDBAR_SetLevel,
    .setLED = LEDBAR_SetLED,
    .toggleLED = LEDBAR_ToggleLED,
    .setBits = LEDBAR_SetBits,
    .getBits = LEDBAR_GetBits
};

ledbar_t *LEDBAR_New(ledbar_portctl_t func, ledbar_delay_ms_t delay, ledbar_pin_t clkport, ledbar_pin_t datport, bool greenToRed)
{
    ledbar_size_t len = 10;
    ledbar_t *ledbar = (ledbar_t *)malloc(sizeof(ledbar));
    struct ledbar_pri_data *priData = (struct ledbar_pri_data *)malloc(sizeof(struct ledbar_pri_data));
    
    priData->clk_io = clkport;
    priData->dat_io = datport;
    priData->func = func;
    priData->greenToRed = greenToRed;

    priData->state = (uint8_t *)malloc(sizeof(uint8_t) * len);
    priData->length = len;

    priData->delay = delay;

    ledbar->priv_args = (void *)priData;

    ledbar->opt = (struct led_bar_opt *)&ledbar_opts;


    return ledbar;
}

void LEDBAR_Free(ledbar_t *ledbar)
{
    struct ledbar_pri_data *priData = (struct ledbar_pri_data *)ledbar->priv_args;
    free(priData->state);
    free(priData);
    ledbar->priv_args = (void *)NULL;
    free(ledbar);
}
