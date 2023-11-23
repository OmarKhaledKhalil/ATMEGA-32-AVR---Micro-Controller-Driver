#ifndef TIMER1_H_
#define TIMER1_H_

#define CPU_FREQ     8
#define OVF_VALUE_T1 65536

typedef enum{
	TIMER1_STOP=0,
	TIMER1_SCALER_1,
	TIMER1_SCALER_8,
	TIMER1_SCALER_64,
	TIMER1_SCALER_256,
	TIMER1_SCALER_1024,
	EXTERNAL0_FALLING,
	EXTERNAL0_RISING
}Timer1Scaler_type;

typedef enum
{
	TIMER1_NORMAL_MODE=0,
	TIMER1_CTC_ICR_TOP_MODE,
	TIMER1_CTC_OCRA_TOP_MODE,
	TIMER1_FASTPWM_ICR_TOP_MODE,
	TIMER1_FASTPWM_OCRA_TOP_MODE

}Timer1Mode_type;

typedef enum
{
	OCRA_DISCONNECTED=0,
	OCRA_TOGGLE,
	OCRA_NON_INVERTING,
	OCRA_INVERTING

}OC1A_Mode_type;

typedef enum
{
	OCRB_DISCONNECTED=0,
	OCRB_TOGGLE,
	OCRB_NON_INVERTING,
	OCRB_INVERTING

}OC1B_Mode_type;


typedef enum{
	RISING=0,
	FALLING
}ICU_Edge_type;

typedef struct
{
	Timer1Mode_type Timer_Mode;
	Timer1Scaler_type Scaler;
	OC1A_Mode_type OCA_Mode;
	OC1B_Mode_type OCB_Mode;
}Timer1_CFG;

typedef enum
{
	ZERO_FREQ=0,
	SIXTEEN_M_FREQ,
	TWO_M_FREQ,
	TWOHUNDRED_FIFTY_K_FREQ,
	SIXTY_TWO_THOUSADN_K_FREQ,
	FIFTEEN_THOUSANDK_FREQ,
	
}Timer1_Freq;

typedef enum
{
	OCA=0,
	OCB
	
}OCA_PIN_Type;

typedef enum
{
	COUNTING_EVENT_UNDONE=0,
	COUNTING_EVENT_DONE=1
	
}Counting_Event_Status;

void Timer1_void_Init(Timer1_CFG*CFG);
void Timer1_void_Start (Timer1_CFG*CFG);
void Timer1_void_Stop (void);
void Timer1_void_SetCounts(u16 value);
void Timer1_void_SetOCRA1_Mode(Timer1_CFG*CFG);
void Timer1_void_SetOCRB1_Mode(Timer1_CFG*CFG);
void Timer1_void_SetOCRA1_Value(u16 value);
void Timer1_void_SetOCRB1_Value(u16 value);
u16 Timer1_u16_GetCounts (void);
u16 Timer1_u16_Get_ICR(void);///////////
void Timer1_void_setDelayTimeMilliSec(u16 delay_time);
void Timer1_void_setFastPWM(Timer1_Freq Freq ,u8 duty_Percentage,OCA_PIN_Type OC_pin);
void Timer1_void_setphaseCorrectPWM(Timer1_Freq Freq ,u8 duty_Percentage,OCA_PIN_Type OC_pin);
void Timer1_void_InputCaptureEdge(ICU_Edge_type edge);//////////////////////////
Counting_Event_Status Timer1_u16_Count_Event(u32*time_calculated,ICU_Edge_type Edge);
Counting_Event_Status Timer1_u16_Count_Pulse(u32*time_calculated,ICU_Edge_type Edge);

void Timer1_void_ICU_InterruptEnable(void);///////////////////
void Timer1_void_ICU_InterruptDisable(void);/////////////
void Timer1_void_OVF_InterruptEnable(void);
void Timer1_void_OVF_InterruptDisable(void);
void Timer1_void_OCA_InterruptEnable(void);
void Timer1_void_OCA_InterruptDisable(void);
void Timer1_void_OCB_InterruptEnable(void);
void Timer1_void_OCB_InterruptDisable(void);

void Timer1_void_OVF_SetCallBack(void(*LocalFptr)(void));
void Timer1_void_OCA_SetCallBack(void(*LocalFptr)(void));
void Timer1_void_OCB_SetCallBack(void(*LocalFptr)(void));
void Timer1_void_ICU_SetCallBack(void(*LocalFptr)(void));




#endif