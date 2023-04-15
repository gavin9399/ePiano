/*====================================================
File    : lcd.h
Version : V1.1
Date    : 2022.11.29
Author  : Gavin Hu
Company : ZJOU
Comments: 
IDE     : Keil C51 uVision V5.25.2.0
MCU type: 51
====================================================*/

#ifndef _LCD_H_
#define _LCD_H_ 1

#include "typedef.h"

#ifndef _DISPLAY_CODE_
#define _DISPLAY_CODE_          1
#define DISPLAY_A_CODE          10
#define DISPLAY_B_CODE          11
#define DISPLAY_C_CODE          12
#define DISPLAY_D_CODE          13
#define DISPLAY_E_CODE          14
#define DISPLAY_F_CODE          15
#define DISPLAY_G_CODE          16
#define DISPLAY_H_CODE          17
#define DISPLAY_I_CODE          1
#define DISPLAY_J_CODE          18
#define DISPLAY_L_CODE          19
#define DISPLAY_N_CODE          20
#define DISPLAY_O_CODE          21
#define DISPLAY_P_CODE          22
#define DISPLAY_R_CODE          23
#define DISPLAY_S_CODE          5
#define DISPLAY_T_CODE          24
#define DISPLAY_U_CODE          25
#define DISPLAY_MINUS_CODE      26
#define DISPLAY_OFF_CODE        27
//#define DISPLAY_DP_CODE      0x40
#define DISPLAY_TWINKLE_CODE    0x80

extern void Data2String(uint16_t u16Data, uint8_t u8Digits, uint8_t u8StrLen, uint8_t a_u8String[]);
#endif

#define LCD_PIXEL_RAM_NUMBER    5
#define LCD_DIGIT_NUMBER        3

#define LCD_CHAR_V              0x15
#define LCD_CHAR_DOT            0x34
#define LCD_ICON_6V_BATTERY     0x17
#define LCD_ICON_12V_BATTERY    0x16
#define LCD_ICON_MOTOR          0x13
#define LCD_ICON_CAR            0x12
#define LCD_ICON_SNOW           0x11
#define LCD_ICON_SPANNER        0x10
#define LCD_ICON_BATTERY        0x14
#define LCD_ICON_BATTERY1       0x07
#define LCD_ICON_BATTERY2       0x06
#define LCD_ICON_BATTERY3       0x05
#define LCD_ICON_BATTERY4       0x04

#define LCD_ALL_CLEAR           0
#define LCD_ALL_FILL            1
#define LCD_ALL_TWINKLE         2

#define LCD_ANIMATION_RUN       true
#define LCD_ANIMATION_STOP      false

#define LCD_UPDATE_PERIOD       100     /* 100ms */

#define LcdTurnOnBackLight()    GpoSet(GPO_1,GPO_MODE_ON)
#define LcdTurnOffBackLight()   GpoSet(GPO_1,GPO_MODE_OFF)

extern void LcdInitialization(void);
extern void LcdFill(uint8_t u8Parameter);
extern void LcdDisplayIcons(uint8_t u8Icon);
extern void LcdClearIcons(uint8_t u8Icon);
extern void LcdTwinkleIcons(uint8_t u8Icon);
extern void LcdDisplayNumbers(const uint8_t c_a_u8Number[]);
extern void LcdDisplayBattery(uint8_t u8Battery, bool bTwinkle);
extern void LcdUpdate(void);

#endif  /* _LCD_H_ */
