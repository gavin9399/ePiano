/*====================================================
File    : lcd.c
Version : V1.1
Date    : 2022.11.29
Author  : Gavin Hu
Company : ZJOU
Comments: 
IDE     : Keil C51 uVision V5.25.2.0
MCU type: 51
====================================================*/

#define LCD_DEBUG 0

#include "lcd.h"
#include "ht1621.h"
#include "system_timer.h"
#include "gpio.h"
#if LCD_DEBUG
    #include "stdio.h"
#endif

static idata uint8_t l_a_u8LcdPixelRam[LCD_PIXEL_RAM_NUMBER]={0,0,0,0,0};
static idata uint8_t l_a_u8LcdPixelTwinkle[LCD_PIXEL_RAM_NUMBER]={0,0,0,0,0};
static bit l_bLcdPixelNew=false;
/*----------------------------------------------
Function: LCD initialization
Parameter: void
Return: void
----------------------------------------------*/
void LcdInitialization(void)
{
    Ht1621Initialization();
  #if LCD_DEBUG
    {
        uint8_t i, j;
        for (i=0; i<23; i++)
        {
            WriteHt1621Data(i, 0);
        }
        for (i=0; i<LCD_PIXEL_RAM_NUMBER; i++)
        {
            for (j=0; j<4;)
            {
                getchar();
                WriteHt1621Data(i*2+14, 1<<j);
                putchar(i+'0');
                putchar('.');
                putchar(j+'0');
                j++;
            }
            for (j=0; j<4;)
            {
                getchar();
                WriteHt1621Data(i*2+15, 1<<j);
                putchar(i+'0');
                putchar('.');
                putchar(j+4+'0');
                j++;
            }
        }
    }
  #endif
}
/*----------------------------------------------
Function: Fill LCD
Parameter: u8Parameter:0-all clear, 1-fill, 2-twinkle
Return: void
----------------------------------------------*/
void LcdFill(uint8_t u8Parameter)
{
    uint8_t i;
    switch (u8Parameter)
    {
        case LCD_ALL_CLEAR:
        for (i=0; i<LCD_PIXEL_RAM_NUMBER; i++)
        {
            l_a_u8LcdPixelRam[i] = 0;
            l_a_u8LcdPixelTwinkle[i] = 0;
        }
        l_bLcdPixelNew = true;
        break;
        case LCD_ALL_FILL:
        for (i=0; i<LCD_PIXEL_RAM_NUMBER; i++)
        {
            l_a_u8LcdPixelRam[i] = 0xFF;
            l_a_u8LcdPixelTwinkle[i] = 0;
        }
        l_bLcdPixelNew = true;
        break;
        case LCD_ALL_TWINKLE:
        for (i=0; i<LCD_PIXEL_RAM_NUMBER; i++)
        {
            l_a_u8LcdPixelTwinkle[i] = 0xFF;
        }
        break;
        default: break;
    }
}
/*----------------------------------------------
Function: Display Icons
Parameter: u8Icon
Return: void
----------------------------------------------*/
void LcdDisplayIcons(uint8_t u8Icon)
{
    uint8_t i,j,k;
    i = u8Icon >> 4;
    j = u8Icon & 0x07;
    k = 1 << j;
    l_a_u8LcdPixelRam[i] |= k;
    l_a_u8LcdPixelTwinkle[i] &= ~k;
    l_bLcdPixelNew = true;
}
/*----------------------------------------------
Function: Clear Icons
Parameter: u8Icon
Return: void
----------------------------------------------*/
void LcdClearIcons(uint8_t u8Icon)
{
    uint8_t i,j,k;
    i = u8Icon >> 4;
    j = u8Icon & 0x07;
    k = ~(1 << j);
    l_a_u8LcdPixelRam[i] &= k;
    l_a_u8LcdPixelTwinkle[i] &= k;
    l_bLcdPixelNew = true;
}
/*----------------------------------------------
Function: Twinkle Icons
Parameter: u8Icon
Return: void
----------------------------------------------*/
void LcdTwinkleIcons(uint8_t u8Icon)
{
    l_a_u8LcdPixelTwinkle[u8Icon >> 4] |= 1 << (u8Icon & 0x07);
}
/*----------------------------------------------
Function: Display number
Parameter: Number string
Return: void
----------------------------------------------*/
void LcdDisplayNumbers(const uint8_t c_a_u8Number[])
{
    code uint8_t c_a_u8SegmentsTable[]={
        0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x3D,0x76,0x0E,0x38,0x54,0x5C,0x73,0x50,0x78,0x3E,0x40,0x00};
        /* 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  A,  b,  C,  d,  E,  F,  G,  H,  J,  L,  n,  o,  P,  r,  t,  U,  -,  OFF*/
        /* 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,  27*/
    code uint8_t c_aa_u8LcdSegmentsPlacement[][7]={
        {0x33,0x32,0x31,0x30,0x27,0x26,0x25},{0x23,0x22,0x21,0x20,0x37,0x36,0x35},{0x43,0x42,0x41,0x40,0x03,0x02,0x01}};
    uint8_t u8Data, i, j;

    for (i=0; i<LCD_DIGIT_NUMBER; i++)
    {
        u8Data = c_a_u8SegmentsTable[c_a_u8Number[i] & ~DISPLAY_TWINKLE_CODE];
        for (j=0; j<7; j++)
        {
            LcdClearIcons(c_aa_u8LcdSegmentsPlacement[i][j]);
        }
        if (0 != (c_a_u8Number[i] & DISPLAY_TWINKLE_CODE))
        {
            for (j=0; j<7; j++)
            {
                if (0 != (u8Data & 0x01))
                {
                    LcdTwinkleIcons(c_aa_u8LcdSegmentsPlacement[i][j]);
                }
                u8Data >>= 1;
            }
        }
        else
        {
            for (j=0; j<7; j++)
            {
                if (0 != (u8Data & 0x01))
                {
                    LcdDisplayIcons(c_aa_u8LcdSegmentsPlacement[i][j]);
                }
                u8Data >>= 1;
            }
        }
    }
}
/*----------------------------------------------
Function: Display battery icon
Parameter: u8Battery: 0~5, bTwinkle
Return: void
----------------------------------------------*/
void LcdDisplayBattery(uint8_t u8Battery, bool bTwinkle)
{
    LcdClearIcons(LCD_ICON_BATTERY1);
    LcdClearIcons(LCD_ICON_BATTERY2);
    LcdClearIcons(LCD_ICON_BATTERY3);
    LcdClearIcons(LCD_ICON_BATTERY4);
    LcdDisplayIcons(LCD_ICON_BATTERY);
    switch (u8Battery)
    {
        case 4: LcdDisplayIcons(LCD_ICON_BATTERY4); /*No break*/
        case 3: LcdDisplayIcons(LCD_ICON_BATTERY3); /*No break*/
        case 2: LcdDisplayIcons(LCD_ICON_BATTERY2); /*No break*/
        case 1: LcdDisplayIcons(LCD_ICON_BATTERY1); break;
        default: break;
    }
    if (bTwinkle)
    {
        switch (u8Battery)
        {
            case 1: LcdTwinkleIcons(LCD_ICON_BATTERY1); break;
            case 2: LcdTwinkleIcons(LCD_ICON_BATTERY2); break;
            case 3: LcdTwinkleIcons(LCD_ICON_BATTERY3); break;
            case 4: LcdTwinkleIcons(LCD_ICON_BATTERY4); break;
            default: break;
        }
    }
}
/*----------------------------------------------
Function: Update LCD
Parameter: global variable "l_a_u32LcdPixelRam[]","l_a_u32LcdPixelTwinkle[]",
Return: void
----------------------------------------------*/
void LcdUpdate(void)
{
    static bit s_bTwinkleOn;
    static uint8_t s_u8UpdateTime, s_u8UpdatePeriod, su8UpdateRecord;
    uint8_t i, u8Now;
    u8Now = g_u8SystemTimeTicks;
    if ((uint8_t)(u8Now - s_u8UpdateTime) >= LCD_UPDATE_PERIOD)
    {
        s_u8UpdateTime = u8Now;
        s_u8UpdatePeriod ++;
        if (s_bTwinkleOn)
        {
            if ((uint8_t)(s_u8UpdatePeriod - su8UpdateRecord) >= 6)  /*600ms on*/
            {
                su8UpdateRecord = s_u8UpdatePeriod;
                for (i=0;i<LCD_PIXEL_RAM_NUMBER;i++)
                {
                    l_a_u8LcdPixelRam[i] &= ~l_a_u8LcdPixelTwinkle[i];
                }
                s_bTwinkleOn = false;
                l_bLcdPixelNew = true;
            }
        }
        else
        {
            if ((uint8_t)(s_u8UpdatePeriod - su8UpdateRecord) >= 4)  /*400ms off*/
            {
                su8UpdateRecord = s_u8UpdatePeriod;
                for (i=0;i<LCD_PIXEL_RAM_NUMBER;i++)
                {
                    l_a_u8LcdPixelRam[i] |= l_a_u8LcdPixelTwinkle[i];
                }
                s_bTwinkleOn = true;
                l_bLcdPixelNew = true;
            }
        }
    }
    if (l_bLcdPixelNew)
    {
        for (i=0;i<LCD_PIXEL_RAM_NUMBER;i++)
        {
            WriteHt1621Data(i*2+14,l_a_u8LcdPixelRam[i]);
            WriteHt1621Data(i*2+15,l_a_u8LcdPixelRam[i]>>4);
        }
        l_bLcdPixelNew = false;
    }
}
#ifdef _DISPLAY_CODE_
#define DATA2STRING_REMOVE_PREFIX_0     1
/*----------------------------------------------
Function: Convert data to display string
Parameter: Data, Decimal digits, String len, String
Return: void
Comments: "Decimal digits" must less than "String len"
----------------------------------------------*/
void Data2String(uint16_t u16Data, uint8_t u8Digits, uint8_t u8StrLen, uint8_t a_u8String[])
{
    uint8_t u8i;
    uint32_t u32Temp=1;
    for (u8i=0;u8i<u8StrLen;u8i++)
    {   /*Calculate the capacity of the string*/
        u32Temp *= 10;
    }
    u8i = 0;
    while ((u16Data >= u32Temp) && (u8Digits > 0))
    {   /*Truncate insignificant trailing decimals*/
        u8i = u16Data % 10;
        u16Data /= 10;
        u8Digits--;
    }
    if (u8i >= 5)   /*Rounding*/
    {
        u16Data++;
    }
    u8i = u8StrLen;
    while (u8i > 0)
    {   /* Split data */
        --u8i;
        a_u8String[u8i] = u16Data % 10;
        u16Data /= 10;
    }
    u8StrLen = u8StrLen - u8Digits - 1;
  #ifdef DISPLAY_DP_CODE
    if (0 != u8Digits)  /*Set the decimal point*/
    {
        a_u8String[u8StrLen] |= DISPLAY_DP_CODE;
    }
  #endif
  #ifdef DATA2STRING_REMOVE_PREFIX_0
    for (u8i=0;(u8i<u8StrLen)&&(0==a_u8String[u8i]);u8i++)
    {   /*Remove insignificant prefix zero*/
        a_u8String[u8i] = DISPLAY_OFF_CODE;
    }
  #endif
}
#endif
