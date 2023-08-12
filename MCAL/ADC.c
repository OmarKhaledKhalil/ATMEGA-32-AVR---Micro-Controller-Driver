#include "StdTypes.h"
#include "MemMap.h"
#include "Utils.h"
#include "ADC_Interface.h"

static u8 ADCflag = 1;

void ADC_Init(void)
{
	/* VREF */
	ADMUX=ADMUX&0b00111111; //clear VREF BITS
	ADMUX=ADMUX|VOLTAGE_REF;
	
	/*Adjustment*/
	ADMUX=ADMUX&0b11011111; //clear ADJUSTMENT BIT
	ADMUX=ADMUX|ADJUSTMENT;
	
	/* Clock */
	ADCSRA=ADCSRA&0b11111000; //clear PRESCALER BITS
	ADCSRA=ADCSRA|PRE_SCALER;
	
	/*ADC_ENABLE*/
	SET_BIT(ADCSRA,ADEN);
}


u16 ADC_Read(ADC_Channel_type channel)
{
	/*select channel using Mux*/
	
	channel=channel&0x1F; //input protection 
	ADMUX=ADMUX&0b11100000; // clear channels pins
	ADMUX=ADMUX|channel;
	
	/*start conversion*/
	
	SET_BIT(ADCSRA,ADSC);
	
	/*wait tell end of conversion*/
	
	while(READ_BIT(ADCSRA,ADSC)); // wait till converstion time end using busy wait 
	
	/*get reading from 2 regs

	u16 read=ADCL;

	u8*p=&read;

     *(p+1)=ADCH;*/
	
	return ADC;
}

void ADC_StartConverstion(ADC_Channel_type channel)
{
	if(ADCflag==1)
	{
		/*select channel using MUX*/
		
		channel=channel&0x1F; //input protection
		ADMUX=ADMUX&0b11100000; // clear channels pins
		ADMUX=ADMUX|channel;
		
		/*start conversion*/
		
		SET_BIT(ADCSRA,ADSC);
		
		ADCflag=0;
	}
		
}

u16 ADC_GetRead(void)
{
		while(READ_BIT(ADCSRA,ADSC)); // wait till converstion time end using busy wait
		
		/*get reading from 2 regs*/

		u16 read=ADCL;

		u8*p=&read;

		*(p+1)=ADCH;
		
		return read;
}

u8 ADC_GetRead_Periodic(u16*data)
{
	if(!READ_BIT(ADCSRA,ADSC))
	{
		
		u16 read=ADCL;

		u8*p=&read;

		*(p+1)=ADCH;
		
		*data=read;
		
		ADCflag=1;
		
		return 1;
	}
	else
	{
		return 0;
	}
}


u16 ADC_ReadVolt(ADC_Channel_type channel)
{
	u16 memory_read=ADC_Read(channel);
	
	u16 volt =(memory_read*(u32)VOLTAGE_REFRENCE_VALUE_MV)/RESOLUTION;
	
	return volt;
}