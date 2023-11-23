#include "StdTypes.h"
#include "Utils.h"
#include "MCU_HW.h"

#include "Timer1.h"

/*************************Pointer to functions to be assigned to ISR*********************************/
static void (*Timer1_OVF_Fptr)(void)=NULLPTR;
static void (*Timer1_OCA_Fptr)(void)=NULLPTR;
static void (*Timer1_OCB_Fptr)(void)=NULLPTR;
static void (*Timer1_ICU_Fptr)(void)=NULLPTR;
/******************************************************************************************/

static u16 Timer_Pre_Scaler;  //To save Prescaling Choosed Value in it.
static volatile u32 Interrupt_Count;
static volatile u32 T1_Event;
static volatile u32 T2_Event;
static volatile u8 Flag_Event;
static volatile u16 Counter_OVF;
static volatile u8 Counter_OVF_For_Events;

static volatile u32 T1_Pulse;
static volatile u32 T2_Pulse;
static volatile u8 Flag_Pulse;
static volatile u8 Counter_OVF_For_Pulse;
static ICU_Edge_type EDGE;
/*************************************************************************/
/*timer 1 functions*/


void Timer1_void_Init(Timer1_CFG*CFG)
{
	switch (CFG->Timer_Mode)
	{
		case TIMER1_NORMAL_MODE:
		CLR_BIT(TCCR1A,WGM10);
		CLR_BIT(TCCR1A,WGM11);
		CLR_BIT(TCCR1B,WGM12);
		CLR_BIT(TCCR1B,WGM13);
		break;
		case TIMER1_CTC_ICR_TOP_MODE:
		CLR_BIT(TCCR1A,WGM10);
		CLR_BIT(TCCR1A,WGM11);
		SET_BIT(TCCR1B,WGM12);
		SET_BIT(TCCR1B,WGM13);
		break;
		
		case TIMER1_CTC_OCRA_TOP_MODE:
		CLR_BIT(TCCR1A,WGM10);
		CLR_BIT(TCCR1A,WGM11);
		SET_BIT(TCCR1B,WGM12);
		CLR_BIT(TCCR1B,WGM13);
		break;
		
		case TIMER1_FASTPWM_ICR_TOP_MODE:
		CLR_BIT(TCCR1A,WGM10);
		SET_BIT(TCCR1A,WGM11);
		SET_BIT(TCCR1B,WGM12);
		SET_BIT(TCCR1B,WGM13);
		break;
		
		case TIMER1_FASTPWM_OCRA_TOP_MODE:
		SET_BIT(TCCR1A,WGM10);
		SET_BIT(TCCR1A,WGM11);
		SET_BIT(TCCR1B,WGM12);
		SET_BIT(TCCR1B,WGM13);
		break;
	}

	TCCR1B&=0XF8;
	TCCR1B|=CFG->Scaler;
	
	switch( CFG->Scaler )
	{
		case TIMER1_STOP:
		Timer_Pre_Scaler=0;
		break;
		case TIMER1_SCALER_1:
		
		Timer_Pre_Scaler=1;
		break;
		case TIMER1_SCALER_8:
		
		Timer_Pre_Scaler=8;
		break;
		case TIMER1_SCALER_64:
		
		Timer_Pre_Scaler=64;
		break;
		case TIMER1_SCALER_256:
		
		Timer_Pre_Scaler=256;
		break;
		case TIMER1_SCALER_1024:
		
		Timer_Pre_Scaler=1024;
		break;
		
	}


}

void Timer1_void_Start (Timer1_CFG*CFG)
{
	TCCR1B&=  0XF8 ;       //0b11111000 (Masking to clean).
	TCCR1B|= CFG->Scaler;  //Assigning the Pre-Scaler.
	
	switch( CFG->Scaler )
	{
		case TIMER1_STOP:
		
		Timer_Pre_Scaler=0;
		break;
		
		case TIMER1_SCALER_1:
		
		Timer_Pre_Scaler=1;
		break;
		
		case TIMER1_SCALER_8:
		
		Timer_Pre_Scaler=8;
		break;
		
		case TIMER1_SCALER_64:
		
		Timer_Pre_Scaler=64;
		break;
		
		case TIMER1_SCALER_256:
		
		Timer_Pre_Scaler=256;
		break;
		
		case TIMER1_SCALER_1024:
		
		Timer_Pre_Scaler=1024;
		break;
		
	}
}

void Timer1_void_Stop (void)
{
	TCCR1B|= TIMER1_STOP;  //Assigning the Pre-Scaler.
}

void Timer1_void_SetCounts(u16 value)
{
	TCNT1=value;
}


void Timer1_void_SetOCRA1_Mode(Timer1_CFG*CFG)
{
	switch (CFG->OCA_Mode)
	{
		case OCRA_DISCONNECTED:
		CLR_BIT(TCCR1A,COM1A0);
		CLR_BIT(TCCR1A,COM1A1);
		break;
		case OCRA_TOGGLE:
		SET_BIT(TCCR1A,COM1A0);
		CLR_BIT(TCCR1A,COM1A1);
		break;
		case OCRA_NON_INVERTING:
		CLR_BIT(TCCR1A,COM1A0);
		SET_BIT(TCCR1A,COM1A1);
		break;
		case OCRA_INVERTING:
		SET_BIT(TCCR1A,COM1A0);
		SET_BIT(TCCR1A,COM1A1);
		break;
	}
}
void Timer1_void_SetOCRB1_Mode(Timer1_CFG*CFG)
{
	switch (CFG->OCB_Mode)
	{
		case OCRB_DISCONNECTED:
		CLR_BIT(TCCR1A,COM1B0);
		CLR_BIT(TCCR1A,COM1B1);
		break;
		case OCRB_TOGGLE:
		SET_BIT(TCCR1A,COM1B0);
		CLR_BIT(TCCR1A,COM1B1);
		break;
		case OCRB_NON_INVERTING:
		CLR_BIT(TCCR1A,COM1B0);
		SET_BIT(TCCR1A,COM1B1);
		break;
		case OCRB_INVERTING:
		SET_BIT(TCCR1A,COM1B0);
		SET_BIT(TCCR1A,COM1B1);
		break;
	}
}
void Timer1_void_InputCaptureEdge(ICU_Edge_type edge)
{
	if(edge==RISING)
	SET_BIT(TCCR1B,ICES1);
	
	else if(edge==FALLING)
	CLR_BIT(TCCR1B,ICES1);
}

void Timer1_void_SetOCRA1_Value(u16 value)
{
	OCR1A=value;
}
void Timer1_void_SetOCRB1_Value(u16 value)
{
	OCR1B=value;
}

u16 Timer1_u16_Get_ICR(void)
{
	return ICR1;
}

u16 Timer1_u16_GetCounts (void)
{
	return TCNT1;
}

void Timer1_void_setDelayTimeMilliSec(u16 delay_time)
{
		u8 ticktime=Timer_Pre_Scaler*10/CPU_FREQ;
		
		u16 OV_Interrupt_Time=ticktime*OVF_VALUE_T1/10;
		
		Interrupt_Count=delay_time*1000/OV_Interrupt_Time;
}

void Timer1_void_setFastPWM(Timer1_Freq Freq ,u8 duty_Percentage,OCA_PIN_Type OC_pin)
{
		TCCR1B&=  0XF8 ;       //0b11111000 (Masking to clean).
		TCCR1B|= Freq  ;       //Assigning the Pre-Scaler.
		
		switch(Freq)
		{
			case ZERO_FREQ :
			Timer_Pre_Scaler = 0 ;
			break;
			case SIXTEEN_M_FREQ :
			Timer_Pre_Scaler = 1 ;
			break;
			case TWO_M_FREQ :
			Timer_Pre_Scaler = 8 ;
			break;
			case TWOHUNDRED_FIFTY_K_FREQ :
			Timer_Pre_Scaler = 64 ;
			break;
			case SIXTY_TWO_THOUSADN_K_FREQ :
			Timer_Pre_Scaler = 256 ;
			break;
			case FIFTEEN_THOUSANDK_FREQ :
			Timer_Pre_Scaler = 1024 ;
			break;
		}
		
		switch(OC_pin)
		{
			case OCA :
			OCR1A=((u32)(OVF_VALUE_T1*duty_Percentage))/100;
			
			case OCB :
			OCR1B=((u32)(OVF_VALUE_T1*duty_Percentage))/100;

		}
		
}
void Timer1_void_setphaseCorrectPWM(Timer1_Freq Freq ,u8 duty_Percentage,OCA_PIN_Type OC_pin)
{
		TCCR1B&=  0XF8 ;       //0b11111000 (Masking to clean).
		TCCR1B|= Freq  ;       //Assigning the Pre-Scaler.
		
		switch(Freq)
		{
			case ZERO_FREQ :
			Timer_Pre_Scaler = 0 ;
			break;
			case SIXTEEN_M_FREQ :
			Timer_Pre_Scaler = 1 ;
			break;
			case TWO_M_FREQ :
			Timer_Pre_Scaler = 8 ;
			break;
			case TWOHUNDRED_FIFTY_K_FREQ :
			Timer_Pre_Scaler = 64 ;
			break;
			case SIXTY_TWO_THOUSADN_K_FREQ :
			Timer_Pre_Scaler = 256 ;
			break;
			case FIFTEEN_THOUSANDK_FREQ :
			Timer_Pre_Scaler = 1024 ;
			break;
		}
		
		switch(OC_pin)
		{
			case OCA :
			OCR1A=((u32)(OVF_VALUE_T1*duty_Percentage/2))/100;
			
			case OCB :
			OCR1B=((u32)(OVF_VALUE_T1*duty_Percentage/2))/100;

		}
		
		
}

static void Read_ICU_PER_EVENT(void)
{
	if(Flag_Event==0)
	{
		T1_Event= ICR1 ;
		Flag_Event++;
	}
	else if ( Flag_Event==1)
	{
		T2_Event= ICR1 ;
		Flag_Event++;
	}
}

Counting_Event_Status Timer1_u16_Count_Event(u32*time_calculated,ICU_Edge_type Edge)
{
	
	if(Flag_Event==0)
	{
		Timer1_void_InputCaptureEdge(Edge);
		Timer1_void_ICU_SetCallBack(Read_ICU_PER_EVENT);
		Timer1_void_ICU_InterruptEnable();
				
	}
	
	else if(Flag_Event==2)
	{
		*time_calculated= ((u32)(T2_Event-T1_Event)+((u32)Counter_OVF_For_Events*OVF_VALUE_T1))*((u32)Timer_Pre_Scaler/CPU_FREQ);
		T2_Event=0;
		T1_Event=0;
		Counter_OVF_For_Events=0;
		Flag_Event=0;
		return COUNTING_EVENT_DONE;
	}
	 return COUNTING_EVENT_UNDONE;
}

static void Read_ICU_PER_Pulse(void)
{
	if(Flag_Pulse==0)
	{
		EDGE=1^EDGE;
		T1_Pulse= ICR1 ;
		Timer1_void_InputCaptureEdge(EDGE);
		Flag_Pulse++;
	}
	else if ( Flag_Pulse==1)
	{
		T2_Pulse= ICR1 ;
		Flag_Pulse++;
	}
}


Counting_Event_Status Timer1_u16_Count_Pulse(u32*time_calculated,ICU_Edge_type Edge)
{
	if(Flag_Pulse==0)
	{
		EDGE=Edge;
		Timer1_void_InputCaptureEdge(EDGE);
		Timer1_void_ICU_SetCallBack(Read_ICU_PER_Pulse);
		Timer1_void_ICU_InterruptEnable();
		
	}
	
	else if(Flag_Pulse==2)
	{
		*time_calculated= ((u32)(T2_Pulse-T1_Pulse)+((u32)Counter_OVF_For_Pulse*OVF_VALUE_T1))*((u32)Timer_Pre_Scaler/CPU_FREQ);
		T2_Pulse=0;
		T1_Pulse=0;
		Counter_OVF_For_Pulse=0;
		Flag_Pulse=0;
		return COUNTING_EVENT_DONE;
	}
	return COUNTING_EVENT_UNDONE;
}

/****************************Timer 1 Interrupt functions**************************************/

void Timer1_void_ICU_InterruptEnable(void)
{
	SET_BIT(TIMSK,TICIE1);
}
void Timer1_void_ICU_InterruptDisable(void)
{
	CLR_BIT(TIMSK,TICIE1);
}
void Timer1_void_OVF_InterruptEnable(void)
{
	SET_BIT(TIMSK,TOIE1);
}
void Timer1_void_OVF_InterruptDisable(void)
{
	CLR_BIT(TIMSK,TOIE1);
}
void Timer1_void_OCA_InterruptEnable(void)
{
	SET_BIT(TIMSK,OCIE1A);
}
void Timer1_void_OCA_InterruptDisable(void)
{
	CLR_BIT(TIMSK,OCIE1A);
}
void Timer1_void_OCB_InterruptEnable(void)
{
	SET_BIT(TIMSK,OCIE1B);
}
void Timer1_void_OCB_InterruptDisable(void)
{
	CLR_BIT(TIMSK,OCIE1B);
}

/*********************************Timer 1 Call Back functions*****************************************/

void Timer1_void_OVF_SetCallBack(void(*LocalFptr)(void))
{
	Timer1_OVF_Fptr=LocalFptr;
}
void Timer1_void_OCA_SetCallBack(void(*LocalFptr)(void))
{
	Timer1_OCA_Fptr=LocalFptr;
}
void Timer1_void_OCB_SetCallBack(void(*LocalFptr)(void))
{
	Timer1_OCB_Fptr=LocalFptr;
}
void Timer1_void_ICU_SetCallBack(void(*LocalFptr)(void))
{
	Timer1_ICU_Fptr=LocalFptr;
}

/*********************************Timer 1 ISR functions*********************************************/

ISR(TIMER1_OVF_vect)
{
	Counter_OVF++;
	Counter_OVF_For_Events++; 
	if(Timer1_OVF_Fptr!=NULLPTR)
	{
		Timer1_OVF_Fptr();
	}
}
ISR(TIMER1_OCA_vect)
{
	if(Timer1_OCA_Fptr!=NULLPTR)
	{
		Timer1_OCA_Fptr();
	}
}
ISR(TIMER1_OCB_vect)
{
	if(Timer1_OCB_Fptr!=NULLPTR)
	{
		Timer1_OCB_Fptr();
	}
}
ISR(TIMER1_ICU_vect)
{
	if(Timer1_ICU_Fptr!=NULLPTR)
	{
		Timer1_ICU_Fptr();
	}
}