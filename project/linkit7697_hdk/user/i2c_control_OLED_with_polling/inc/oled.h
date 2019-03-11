
#ifndef __OLED_H__
#define __OLED_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define OLED_Max_X                 127 //128 Pixels
#define OLED_Max_Y                 63  //64  Pixels

#define PAGE_MODE                       ((uint8_t)01)
#define HORIZONTAL_MODE                 ((uint8_t)02)


#define OLED_Address               0x3c
#define OLED_Command_Mode          0x80
#define OLED_Data_Mode             0x40
#define OLED_Display_Off_Cmd       0xAE
#define OLED_Display_On_Cmd        0xAF
#define OLED_Normal_Display_Cmd    0xA6
#define OLED_Inverse_Display_Cmd   0xA7
#define OLED_Activate_Scroll_Cmd   0x2F
#define OLED_Dectivate_Scroll_Cmd  0x2E
#define OLED_Set_Brightness_Cmd    0x81

#define Scroll_Left             0x00
#define Scroll_Right            0x01

#define Scroll_2Frames          0x7
#define Scroll_3Frames          0x4
#define Scroll_4Frames          0x5
#define Scroll_5Frames          0x0
#define Scroll_25Frames         0x6
#define Scroll_64Frames         0x1
#define Scroll_128Frames        0x2
#define Scroll_256Frames        0x3



struct oled_s {
    uint8_t addressingMode;
    void (*sender)(uint8_t *, uint16_t);
    struct oled_opt_s *opt;
};

typedef struct oled_s oled_t;

struct oled_opt_s {
    void    (*send)(uint8_t *str, uint16_t len);
    void    (*init)(oled_t *oled);
    void    (*setNormalDisplay)(oled_t *oled);
    void    (*setInverseDisplay)(oled_t *oled);

    void    (*sendCommand)(oled_t *oled, uint8_t command);
    void    (*sendData)(oled_t *oled, uint8_t Data);

    void    (*setPageMode)(oled_t *oled);
    void    (*setHorizontalMode)(oled_t *oled);

    void    (*setTextXY)(oled_t *oled, uint8_t Row, uint8_t Column);
    void    (*clearDisplay)(oled_t *oled);
    void    (*setBrightness)(oled_t *oled, uint8_t Brightness);
    void    (*putChar)(oled_t *oled, uint8_t c);
    void    (*putString)(oled_t *oled, const char *String);
    uint8_t (*putNumber)(oled_t *oled, long n);
    uint8_t (*putFloat)(oled_t *oled, float floatNumber, uint8_t decimal);
    void    (*drawBitmap)(oled_t *oled, uint8_t *bitmaparray, int32_t bytes);

    void    (*setHorizontalScrollProperties)(oled_t *oled, uint8_t direction, uint8_t startPage, uint8_t endPage, uint8_t scrollSpeed);
    void    (*activateScroll)(oled_t *oled);
    void    (*deactivateScroll)(oled_t *oled);
};


extern void    OLED_HardwareInit(void (*)(uint32_t msec));

#ifdef NO_MALLOC_SUPPORT

    extern oled_t *OLED_New(oled_t *oled, void (*send)(uint8_t*, uint16_t));
    #define OLED_Free(oled)

#else

    extern oled_t *OLED_New(void (*send)(uint8_t*, uint16_t));
    extern void    OLED_Free(oled_t *oled);

#endif

extern void    OLED_Init(oled_t *oled);
extern void    OLED_SetInverseDisplay(oled_t *oled);
extern void    OLED_SetNormalDisplay(oled_t *oled);
extern void    OLED_DeactivateScroll(oled_t *oled);
extern void    OLED_ActivateScroll(oled_t *oled);
extern void    OLED_SetHorizontalScrollProperties(oled_t *oled, uint8_t direction, uint8_t startPage, uint8_t endPage, uint8_t scrollSpeed);
extern void    OLED_DrawBitmap(oled_t *oled, uint8_t *bitmaparray, int32_t bytes);
extern uint8_t OLED_PutFloat(oled_t *oled, float floatNumber, uint8_t decimal);
extern void    OLED_SetBrightness(oled_t *oled, uint8_t Brightness);
extern void    OLED_ClearDisplay(oled_t *oled);
extern uint8_t OLED_PutNumber(oled_t *oled, long n);
extern void    OLED_PutString(oled_t *oled, const char *str);
extern void    OLED_PutChar(oled_t *oled, uint8_t c);
extern void    OLED_SetTextXY(oled_t *oled, uint8_t Row, uint8_t Column);
extern void    OLED_SetHorizontalMode(oled_t *oled);
extern void    OLED_SetPageMode(oled_t *oled);
extern void    OLED_SendCommand(oled_t *oled, uint8_t cmd);
extern void    OLED_SendData(oled_t *oled, uint8_t data);


#ifdef __cplusplus
}
#endif

#endif
