#ifndef TIMER0_H_
#define TIMER0_H_

#define CPU_FREQ  16
#define OVF_VALUE_T0 256

typedef enum{
	TIMER0_STOP=0,
	TIMER0_SCALER_1,
	TIMER0_SCALER_8,
	TIMER0_SCALER_64,
	TIMER0_SCALER_256,
	TIMER0_SCALER_1024,
	EXTERNALl_FALLING,
	EXTERNAL_RISING
}Timer0Scaler_type;

typedef enum
{
	TIMER0_NORMAL_MODE=0,
	TIMER0_PHASECORRECT_MODE,
	TIMER0_CTC_MODE,
	TIMER0_FASTPWM_MODE

}Timer0Mode_type;


typedef enum
{
	OC0_DISCONNECTED=0,
	OC0_TOGGLE,
	OC0_NON_INVERTING,
	OC0_INVERTING

}OC0Mode_type;

typedef struct  
{
	Timer0Mode_type Timer_Mode;
	Timer0Scaler_type Scaler;
	OC0Mode_type OC_Mode;
}Timer0_CFG;

typedef enum
{
	ZERO_FREQ0=0,
	SIXTEEN_M_FREQ0,
	TWO_M_FREQ0,
	TWOHUNDRED_FIFTY_K_FREQ0,
	SIXTY_TWO_THOUSADN_K_FREQ0,
	FIFTEEN_THOUSANDK_FREQ0,
	
}Timer0_Freq;

void Timer0_void_Init(Timer0_CFG*CFG);
void Timer0_void_Start (Timer0_CFG*CFG);
void Timer0_void_Stop (void);
void Timer0_void_SetCounts (u8 value);
void Timer0_void_SetOCR(u8 value);
u8 Timer0_u8_GetCounts (void);
void Timer0_void_setDelayTimeMilliSec(u16 delay_time);
void Timer0_void_setFastPWM(Timer0_Freq Freq ,u8 duty_Percentage);
void Timer0_void_setphaseCorrectPWM(Timer0_Freq Freq ,u8 duty_Percentage);

void Timer0_void_OV_InterruptEnable(void);
void Timer0_void_OV_InterruptDisable(void);
void Timer0_void_OC_InterruptEnable(void);
void Timer0_void_OC_InterruptDisable(void);

void Timer0_void_OVF_SetCallBack(void(*LocalFptr)(void));
void Timer0_void_OC_SetCallBack(void(*LocalFptr)(void));

/*********************************************************************************************************/



#endif