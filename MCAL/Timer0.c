#include "StdTypes.h"
#include "Utils.h"
#include "MCU_HW.h"

#include "Timer0.h"

/*************************Pointer to functions to be assigned to ISR*********************************/
static void (*Timer0_OVF_Fptr) (void)=NULLPTR;
static void (*Timer0_OC_Fptr)  (void)=NULLPTR;
/******************************************************************************************/

static u16 Timer_Pre_Scaler;  //To save Prescaling Choosed Value in it.
static volatile u32 Interrupt_Count;

/*timer 0 functions*/
void Timer0_void_Init(Timer0_CFG*CFG)
{
	
	switch ( CFG->Timer_Mode )
	{
		case TIMER0_NORMAL_MODE:
		CLR_BIT(TCCR0,WGM00);
		CLR_BIT(TCCR0,WGM01);
		break;
		case TIMER0_PHASECORRECT_MODE:
		SET_BIT(TCCR0,WGM00);
		CLR_BIT(TCCR0,WGM01);
		break;
		case TIMER0_CTC_MODE:
		CLR_BIT(TCCR0,WGM00);
		SET_BIT(TCCR0,WGM01);
		break;
		case TIMER0_FASTPWM_MODE:
		SET_BIT(TCCR0,WGM00);
		SET_BIT(TCCR0,WGM01);
		break;
	}
	
	switch ( CFG->OC_Mode )
	{
		case OC0_DISCONNECTED:
		CLR_BIT(TCCR0,COM00);
		CLR_BIT(TCCR0,COM01);
		break;
		case OC0_TOGGLE:
		SET_BIT(TCCR0,COM00);
		CLR_BIT(TCCR0,COM01);
		break;
		case OC0_NON_INVERTING:
		CLR_BIT(TCCR0,COM00);
		SET_BIT(TCCR0,COM01);
		break;
		case OC0_INVERTING:
		SET_BIT(TCCR0,COM00);
		SET_BIT(TCCR0,COM01);
		break;
	}
	
	TCCR0&=  0XF8 ;       //0b11111000 (Masking to clean).
	TCCR0|= CFG->Scaler;  //Assigning the Pre-Scaler.
	
	switch( CFG->Scaler )
	{
		case TIMER0_STOP:
			Timer_Pre_Scaler=0;
		break;
		case TIMER0_SCALER_1:
			
			Timer_Pre_Scaler=1;
		break;
		case TIMER0_SCALER_8:
			
			Timer_Pre_Scaler=8;
		break;
		case TIMER0_SCALER_64:
		
			Timer_Pre_Scaler=64;
		break;
		case TIMER0_SCALER_256:
		   
		    Timer_Pre_Scaler=256;
		break;
		case TIMER0_SCALER_1024:
		 
		    Timer_Pre_Scaler=1024;
		break;
		
	}

	
	
}

void Timer0_void_Init_PreBuild(void)
{
	
	//Choosing Mode (Normal Mode)
		CLR_BIT(TCCR0,WGM00);
		CLR_BIT(TCCR0,WGM01);
		
		
	//Choosing OC PIN (OC0 Toggle)
		SET_BIT(TCCR0,COM00);
		CLR_BIT(TCCR0,COM01);
		
	//Choosing PreScaler
	    TCCR0&=  0XF8 ;       //0b11111000 (Masking to clean).
	    TCCR0|= 2;  //Assigning the Pre-Scaler.
	
	
}

void Timer0_void_Start (Timer0_CFG*CFG)
{ 
	TCCR0&=  0XF8 ;       //0b11111000 (Masking to clean).
	TCCR0|= CFG->Scaler;  //Assigning the Pre-Scaler.
	
	switch( CFG->Scaler )
	{
		case TIMER0_STOP:
		
		Timer_Pre_Scaler=0;
		break;
		
		case TIMER0_SCALER_1:
		
		Timer_Pre_Scaler=1;
		break;
		
		case TIMER0_SCALER_8:
		
		Timer_Pre_Scaler=8;
		break;
		
		case TIMER0_SCALER_64:
		
		Timer_Pre_Scaler=64;
		break;
		
		case TIMER0_SCALER_256:
		
		Timer_Pre_Scaler=256;
		break;
		
		case TIMER0_SCALER_1024:
		
		Timer_Pre_Scaler=1024;
		break;
		
	}
}
void Timer0_void_Stop (void)
{
	TCCR0|= TIMER0_STOP;  //Assigning the Pre-Scaler.
}

void Timer0_void_OV_InterruptEnable(void)
{
	SET_BIT(TIMSK,TOIE0);
}
void Timer0_void_OV_InterruptDisable(void)
{
	CLR_BIT(TIMSK,TOIE0);
}
void Timer0_void_OC_InterruptEnable(void)
{
	SET_BIT(TIMSK,OCIE0);
}
void Timer0_void_OC_InterruptDisable(void)
{
	CLR_BIT(TIMSK,OCIE0);
}
void Timer0_void_SetCounts (u8 value)
{
	TCNT0 = value ;
}
void Timer0_void_SetOCR(u8 value)
{
	OCR0 = value  ;
}

u8 Timer0_u8_GetCounts (void)
{
	return TCNT0 ;
}
void Timer0_void_setDelayTimeMilliSec(u16 delay_time)
{
		u8 ticktime=Timer_Pre_Scaler*10/CPU_FREQ;
		
		u16 OV_Interrupt_Time=ticktime*OVF_VALUE_T0/10;
		
		Interrupt_Count=delay_time*1000/OV_Interrupt_Time;
}

void Timer0_void_setFastPWM(Timer0_Freq Freq ,u8 duty_Percentage)
{
	
	TCCR0&=  0XF8 ;       //0b11111000 (Masking to clean).
	TCCR0|= Freq  ;       //Assigning the Pre-Scaler.
	
	switch(Freq)
	{
		case ZERO_FREQ0 :
		Timer_Pre_Scaler = 0 ;
		break;
		case SIXTEEN_M_FREQ0 :
		Timer_Pre_Scaler = 1 ;
		break;
		case TWO_M_FREQ0 :
		Timer_Pre_Scaler = 8 ;
		break;
		case TWOHUNDRED_FIFTY_K_FREQ0 :
		Timer_Pre_Scaler = 64 ;
		break;
		case SIXTY_TWO_THOUSADN_K_FREQ0 :
		Timer_Pre_Scaler = 256 ;
		break;
		case FIFTEEN_THOUSANDK_FREQ0 :
		Timer_Pre_Scaler = 1024 ;
		break;
	}
	
	
	
	OCR0=((u32)(255*duty_Percentage))/100;
	
}

void Timer0_void_setphaseCorrectPWM(Timer0_Freq Freq ,u8 duty_Percentage)
{
	
	TCCR0&=  0XF8 ;       //0b11111000 (Masking to clean).
	TCCR0|= Freq  ;       //Assigning the Pre-Scaler.
	
	switch(Freq)
	{
		case ZERO_FREQ0 :
		Timer_Pre_Scaler = 0 ;
		break;
		case SIXTEEN_M_FREQ0 :
		Timer_Pre_Scaler = 1 ;
		break;
		case TWO_M_FREQ0 :
		Timer_Pre_Scaler = 8 ;
		break;
		case TWOHUNDRED_FIFTY_K_FREQ0 :
		Timer_Pre_Scaler = 64 ;
		break;
		case SIXTY_TWO_THOUSADN_K_FREQ0 :
		Timer_Pre_Scaler = 256 ;
		break;
		case FIFTEEN_THOUSANDK_FREQ0 :
		Timer_Pre_Scaler = 1024 ;
		break;
	}
	OCR0=((u32)(OVF_VALUE_T0*duty_Percentage))/100;
}

void Timer0_void_OVF_SetCallBack(void(*LocalFptr)(void))
{
	Timer0_OVF_Fptr=LocalFptr;
}

void Timer0_void_OC_SetCallBack(void(*LocalFptr)(void))
{
	Timer0_OC_Fptr=LocalFptr;
}
/*************************************************************************/


ISR( TIMER0_OVF_vect )
{
	/*static u32 c=0;
	
	if( c == Interrupt_Count )
	{
		c=0;
		*/
		if( Timer0_OVF_Fptr != NULLPTR )
		{
			Timer0_OVF_Fptr();
		}
		/*
	}
	
	c++;
	*/
}


ISR( TIMER0_OC_vect )
{
	
}