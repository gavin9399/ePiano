/*====================================================
File    : main.h
Version : V1.3
Date    : 2022.9.20
Author  : Gavin Hu
Company : ZJOU
Comments: 
IDE     : Keil C51 uVision V5.25.2.0
          C51.exe V9.57
MCU type: STC8G
====================================================*/

#ifndef _MAIN_H_
#define _MAIN_H_ 1

#include "typedef.h"

/*----------------------------------------------
  Constant define
----------------------------------------------*/
#define DEBUG_UART 1
#define DEBUG_PWM 0
#define DEBUG_ADC 0

#define IAP_FLASH_START_ADDRESS         0X0000
#define CHARGE_MODE_FLASH_ADDRESS       IAP_FLASH_START_ADDRESS
#define DELAY_TASK                      Bit0
#define MAX_ADC_VALUE                   1024
#define ADC_VREF                        5000
#define ADC_TASK_PERIOD                 1
#define ADC_CHANNLE_NUMBER              3
#define ADC_AVERAGE_TIMES               50
#define VOLTAGE_AVERAGE_TIMES           10
#define PWM_MAX_VALUE                   1000

#define MINIMUM_CHARGE_CURRENT          50
#define SHORT_CIRCUIT_VOLTAGE           500
#define NO_BATTERY_VOLTAGE              3000
#define BATTERY_TYPE12V_VOLTAGE         7500
#define BATTERY12V_MAXIMUM_VOLTAGE      16000
#define BATTERY6V_MAXIMUM_VOLTAGE       8000

#define CHARGE_MODE_MOTOR               0
#define CHARGE_MODE_CAR                 1
#define CHARGE_MODE_SNOW                2
#define CHARGE_MODE_REPAIR              3
#define CHARGE_MODE_SUPPLY              4

#define BATTERY_TYPE_6V                 0
#define BATTERY_TYPE_12V                1

#define CHARGE_FULL_STEP                0x90
#define SUPPLY_READY_STEP               0xA0
#define SUPPLY_WORK_STEP                0xA1
#define OUTPUT_SHORT_STEP               0xFB
#define NO_BATTERY_STEP                 0xFC
#define TEST_BATTERY_STEP               0xFD
#define BAD_BATTERY_STEP                0xFE
#define IDLE_STEP                       0xFF

#if DEBUG_UART||DEBUG_PWM||DEBUG_ADC
    #define MINUTE                          6000L
    #define HOUR                            (6*MINUTE)
#else
    #define MINUTE                          60000L
    #define HOUR                            (60*MINUTE)
#endif
/*----------------------------------------------
  Port define
----------------------------------------------*/
#define CHARGE_MOSFET       GPO_0
#define LCD_BACKLIGHT       GPO_1
#define FAN_CONTROL         GPO_2

#define CURRENT_PWM         PCA_CHANNEL1

#define VOLTAGE_ADC_CHANNEL     13
#define CURRENT_ADC_CHANNEL     1
#define TEMPERATURE_CHANNEL     6

/*----------------------------------------------
  Function declaration
----------------------------------------------*/
#define FeedDog() {;}
#define SetChargeOutputOn() GpoSet(CHARGE_MOSFET, GPO_MODE_ON)
#define SetChargeOutputOff() GpoSet(CHARGE_MOSFET, GPO_MODE_OFF)
#define SetChargeOutputTime(u8OnTime,u8OffTime) GpoTwinkle(CHARGE_MOSFET, GPO_INFINITE_TWINKLE, u8OnTime,u8OffTime)
#define DisplayWord(Char0,Char1,Char2) {l_a_u8DisplayString[0]=Char0;l_a_u8DisplayString[1]=Char1;l_a_u8DisplayString[2]=Char2;LcdDisplayNumbers(l_a_u8DisplayString);}
#if DEBUG_PWM
void DebugPwm(void);
#endif
void UartInit(void);
void PwmInit(void);
void ButtonWork(void);
void SelfTest(void);
void TestConnection(void);
void ChargeBattery(void);
void Charge1Step(void);
void Analyze(uint8_t u8Delay, uint16_t u16CompareVoltage);
void DisplayBattery(void);
void DisplayModeIcon(void);
void DisplayVoltage(uint16_t u16Voltage);
void DisplayTemperature(int8_t i8Temperature);
bool bAdcTask(void);
bool bCalculateAverageValue(void);
int8_t i8CalculateTemprature(uint16_t u16Value);
void SetVoltage(uint16_t u16Voltage);
void SetCurrent(uint16_t u16Current);
void SetGaradualCurrent(uint16_t u16Current);
void GraduallyIncreaseCurrent(void);

/*----------------------------------------------
  Public variable declaration
----------------------------------------------*/
code uint16_t c_aaa_u16CompareVoltageTable[][4][11]={{
    {3000,4500,5200,6300,6800,7000,7200,7300,6000,6600,6900},
    {3000,4500,5200,6300,6800,7000,7200,7300,6000,6600,6900},
    {3000,4500,5200,6300,6800,7000,7200,7300,6000,6600,6900},
    {3000,4500,5200,6300,6800,7000,7200,7300,6000,6600,6900}
    }, {
    {3000,9500,10500,12600,13600,14200,14300,14500,12000,13200,13800},
    {3000,9500,10500,12600,13600,14200,14300,14500,12000,13200,13800},
    {3000,9500,10500,12600,13600,14500,14700,14800,12000,13200,13800},
    {3000,9500,10500,12600,13600,14200,14300,14500,12000,13200,13800}
    } };
code uint16_t c_aaa_u16ChargeCurrentTable[][4][10]={{
    {0,1000,2000,2000,2000,2000,2000,1000,0,1000},
    {0,1000,2000,2000,4000,3000,2000,1000,0,1000},
    {0,1000,2000,2000,2000,2000,2000,1000,0,1000},
    {0,1000,2000,2000,2000,2000,2000,1000,0,1000}
    }, {
    {0,1000,2000,2000,2000,2000,2000,1000,0,1000},
    {0,1000,2000,5000,4000,3000,2000,1000,0,1000},
    {0,1000,2000,5000,4000,3000,2000,1000,0,1000},
    {0,1000,2000,1000,1000,1000,1000,1000,0,1000}
    } };
static idata uint8_t l_a_u8DisplayString[3];
static uint16_t l_u16Voltage, l_u16AverageVoltage;
static uint16_t l_u16Current, l_u16AverageCurrent;
static uint16_t l_u16SetVoltage, l_u16StartVoltage;
static uint16_t l_u16SetCurrent=0, l_u16TargetCurrent=0;
static uint8_t l_u8ChargeMode, l_u8BatteryType;
static uint8_t l_u8WorkStep=IDLE_STEP, l_u8WorkStepDone=IDLE_STEP;
static int8_t l_i8Temperature;
static bit l_bBatteryConnect=false, l_bButtonEnable=true;
static bit l_bChargeCurrent=false;
static uint32_t l_u32StartChargeTime, l_u32StepStartTime;

#endif  /* _MAIN_H_ */
