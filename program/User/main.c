/*====================================================
File    : main.c
Project : ePiano
Version : V1.1
Date    : 2023.1.17
Author  : Gavin Hu
Company : ZJOU
Comments: 
    PCB: 
IDE     : Keil C51 uVision V5.25.2.0
          C51.exe V9.57
Chip type           : STC8G
Program type        : Application
Clock frequency     : .000000 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 256
====================================================*/
//#pragma  src
#define DEBUG 0
#include "cpu.h"
#include "typedef.h"
#include "gpio.h"
#include "timer.h"
#include "system_timer.h"
#include "pca.h"
#include "adc.h"
#include "touch_sensing.h"
#include "uart.h"
#include "stdio.h"
#include "led.h"

/*----------------------------------------------
  Constant define
----------------------------------------------*/

#define MUSIC_REST          67
#define MUSIC_END           68
#define MUSIC_BASE_TIME     78

/*----------------------------------------------
  Port define
----------------------------------------------*/
#define SPEAKER_PIN     P37

/*----------------------------------------------
  Function declaration
----------------------------------------------*/
void UartInit(void);
void PwmInit(void);
void Sounding(uint8_t u8Sound);
uint8_t u8PlayMusic(const uint8_t c_a_u8Music[]);

/*----------------------------------------------
  Public variable declaration
----------------------------------------------*/
static uint16_t l_u16PcaTimeValue=1000;
static uint16_t l_u16MusicIndex=0;

/*----------------------------------------------
Function: Main routine
Parameter: void
Return: void
----------------------------------------------*/
void main(void)
{
    code uint8_t c_a_u8MoscowNight[]=
    {
        19,4, 22,4, 26,4, 22,4, 24,8, 22,4, 20,4, 26,8, 24,8, 19,16, 22,4, 26,4, 29,4, 29,4, 
        31,8, 29,4, 27,4, 26,16, 28,8, 30,8, 32,4, 31,4, 26,12, 20,8, 19,4, 26,4, 24,4, 27,12, 
        67,4, 29,4, 27,4, 26,8, 24,4, 22,4, 26,8, 24,8, 19,24, 67,8, MUSIC_END
    };
    code uint8_t c_a_u8TeaPicking[]=
    {
        26,4, 26,2, 29,2, 24,2, 26,2, 24,2, 22,2, 22,2, 24,2, 22,2, 19,2, 17,6, 19,2, 22,4, 
        22,2, 19,2, 22,2, 24,2, 26,2, 29,2, 24,16, 24,6, 20,2, 19,2, 20,2, 19,2, 17,2, 14,2, 
        17,2, 12,2, 14,2, 17,4, 67,4, 29,4, 24,2, 26,2, 24,2, 22,2, 19,2, 22,2, 17,16, 19,4, 
        19,2, 22,2, 17,4, 67,2, 17,2, 19,6, 22,2, 22,2, 19,2, 22,4, 22,2, 24,2, 26,2, 29,2, 
        24,8, 31,2, 29,2, 26,2, 29,2, 24,2, 22,2, 24,4, 19,4, 19,2, 24,2, 20,4, 19,4, 19,4, 
        19,2, 24,2, 20,4, 19,2, 20,2, 17,4, 17,2, 19,2, 17,8, 19,2, 22,2, 24,2, 26,2, 17,2, 
        19,2, 17,4, 22,4, 19,4, 22,4, 22,2, 19,2, 22,4, 22,2, 24,2, 26,2, 29,2, 24,4, 67,4, 
        26,4, 26,2, 29,2, 26,4, 26,2, 24,2, 22,2, 24,2, 19,2, 22,2, 24,4, 67,4, 29,6, 26,2, 
        24,4, 22,2, 19,2, 17,6, 19,2, 22,4, 67,4, 24,4, 24,8, 26,4, 29,4, 26,2, 29,2, 24,4, 
        67,4, 26,4, 26,2, 29,2, 19,4, 22,4, 24,2, 26,2, 29,2, 24,4, 67,4, 29,6, 26,2, 24,4, 
        22,4, 19,4, 19,2, 22,2, 17,4, 67,4, 24,4, 29,2, 26,2, 24,2, 26,2, 17,2, 19,2, 22,12, 
        26,2, 29,2, 24,4, 24,2, 26,2, 24,2, 22,2, 19,4, 17,16, MUSIC_END
    };
    code uint8_t* code c_a_u8Music[]={c_a_u8MoscowNight, c_a_u8TeaPicking};
    uint8_t u8Key;
    uint8_t u8MusicNumber=0;
    bit bMusicPlaying=false;

    /*System initialization*/
    SystemTimerInit();
    LedInitialization();
    TouchSensingInitialization();
    PwmInit();
    EA = 1;

    LedSet(LED_0, LED_MODE_TWINKLE);

    while (1)
    {
        SystemTimerUpdate();
        u8Key = u8TouchKeyScan();
        switch (u8Key)
        {
            case TOUCH_KEY_DOWN+0:
            case TOUCH_KEY_DOWN+1:
            case TOUCH_KEY_DOWN+2:
            case TOUCH_KEY_DOWN+3:
            case TOUCH_KEY_DOWN+4:
            case TOUCH_KEY_DOWN+5:
            case TOUCH_KEY_DOWN+6:
            case TOUCH_KEY_DOWN+7:
            case TOUCH_KEY_DOWN+8:
            case TOUCH_KEY_DOWN+9:
            case TOUCH_KEY_DOWN+10:
            case TOUCH_KEY_DOWN+11:
            Sounding((u8Key & 0x0F) + 27);
            break;

            case TOUCH_KEY_UP+0:
            case TOUCH_KEY_UP+1:
            case TOUCH_KEY_UP+2:
            case TOUCH_KEY_UP+3:
            case TOUCH_KEY_UP+4:
            case TOUCH_KEY_UP+5:
            case TOUCH_KEY_UP+6:
            case TOUCH_KEY_UP+7:
            case TOUCH_KEY_UP+8:
            case TOUCH_KEY_UP+9:
            case TOUCH_KEY_UP+10:
            case TOUCH_KEY_UP+11:
            bMusicPlaying = false;
            Sounding(MUSIC_REST);
            break;

            case TOUCH_KEY_DOWN+12:
            u8MusicNumber = !u8MusicNumber;
            l_u16MusicIndex = 0;
            bMusicPlaying = true;
            break;
            
            default: break;
        }

        if (bMusicPlaying)
        {
            u8PlayMusic(c_a_u8Music[u8MusicNumber]);
        }
        LedUpdate();
    }
}

/*----------------------------------------------
Function: Sounding
Parameter: The sound
Return: void
----------------------------------------------*/
void Sounding(uint8_t u8Sound)
{
    code uint16_t c_a_u16MusicFrequency[]=
    {
        110,117,123,131,139,147,156,165,175,185,196,208,220,233,247,262,277,294,311,330,349,370,392,415,440,
        466,494,523,554,587,622,659,698,740,784,831,880,932,988,1047,1109,1175,1245,1319,1397,1480,1568,
        1661,1760,1865,1976,2093,2218,2349,2489,2637,2794,2960,3136,3322,3520,3729,3951,4186,4435,4699,4978
    };
    CR = 0;            /* Stop music */
    if (u8Sound < MUSIC_REST)
    {
        l_u16PcaTimeValue = (FOSC/12/2)/c_a_u16MusicFrequency[u8Sound];
        CR = 1;        /* Sounding */
    }
    else
    {
        SPEAKER_PIN = 0;
    }
}

/*----------------------------------------------
Function: Play music (Use T1)
Parameter: The music
Return: 0=playing, 68=end
----------------------------------------------*/
uint8_t u8PlayMusic(const uint8_t c_a_u8Music[])
{
    uint16_t u16Index;
    static uint16_t s_u16TimeRecord, s_u16MusicTime;
    u16Index = u32GetSysTicks();
    if ((uint16_t)(u16Index-s_u16TimeRecord) >= s_u16MusicTime)
    {
        s_u16TimeRecord = u16Index;
        u16Index = l_u16MusicIndex * 2;
        Sounding(c_a_u8Music[u16Index]);
        l_u16MusicIndex++;
        if (c_a_u8Music[u16Index] >= MUSIC_END)
        {
            l_u16MusicIndex = 0;
            return MUSIC_END;
        }
        u16Index++;
        s_u16MusicTime = (uint16_t)c_a_u8Music[u16Index]*MUSIC_BASE_TIME;
    }
    return 0;
}
/*----------------------------------------------
Function: PWM Init
Parameter: void
Return: void
----------------------------------------------*/
void PwmInit(void)
{
    PCA_Init_TypeDef pca;
    pca.IO_Switch = PCA_AT_P12_P11_P10_P37;
    pca.Clock = PCA_CLOCK_12T;
    pca.IdleCounter = DISABLE;
    pca.Interrupt = ENABLE;
    pca.Mode = PCA_MODE_FAST_PULSE_OUTPUT;
    pca.Run = DISABLE;
    PCA_Initialization(PCA_COUNTER, &pca);
    PCA_Initialization(PCA_CHANNEL2, &pca);
    GPIO_Init(GPIO_P3_7, GPIO_MODE_OUT_PP_LOW);
}
/*----------------------------------------------
Function: PCA output 16bits PWM interrupt routine
Parameter: void
Return: void
----------------------------------------------*/
void PCA_Output_16Bits_PWM_interrupt(void) interrupt PCA_VECTOR
{
    static uint16_t s_u16CCAP2Reload=0;
    if (CCF2)
    {
        s_u16CCAP2Reload += l_u16PcaTimeValue;
        CCAP2L = (uint8_t)s_u16CCAP2Reload;
        CCAP2H = s_u16CCAP2Reload >> 8;
        CCF2 = 0;
    }
}
/*----------------------------------------------
Function: UART Init
Parameter: void
Return: void
----------------------------------------------*/
void UartInit(void)
{
    UART_ConfInf_t UART_Inf;
    UART_Inf.UART_Mode = UART_8BIT_BRTX;
    UART_Inf.UART_BRT_Use = BRT_TIMER2;
    UART_Inf.UART_BaudRate = 115200;
    UART_Inf.UART_RxEnable = ENABLE;
    UART_Inf.UART_Interrupt = ENABLE;
    u8UART_Configuration(UART1, &UART_Inf);
    EA = 1;
}
