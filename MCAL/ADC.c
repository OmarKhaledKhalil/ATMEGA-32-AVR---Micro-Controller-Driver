#include "StdTypes.h"
#include "MCU_HW.h"
#include "Utils.h"
#include "ADC_Private.h"
#include "ADC_CFG.h"
#include "ADC_Interface.h"

/*************************Pointer to function to be assigned to ISR*********************************/
static void (*ADC_PTR) (void)=NULLPTR;


static u8 ADCflag = 1;


void ADC_void_Init( Sensor_Config * Config , ADC_PreScalling_type Pre_Scaller )
{
	/* VREF From Sensor Config Struct */
	ADMUX=ADMUX&0b00111111; //clear VREF BITS
	ADMUX=ADMUX|Config->Voltage_Refrence;
	
	/* Trigger Mode From Sensor Config Struct */
	SFIOR=SFIOR&0b00011111; //clear AutoTrigger BITS
	SFIOR=SFIOR|Config->Auto_Trigger;
	
	/*Adjustment From Config .h Adjustment MACRO*/
	ADMUX=ADMUX&0b11011111; //clear ADJUSTMENT BIT
	ADMUX=ADMUX|ADJUSTMENT;
	
	/* Clock From Interface .h Prescaller Enum */
	ADCSRA=ADCSRA&0b11111000; //clear PRESCALER BITS
	ADCSRA=ADCSRA|Pre_Scaller;
	
	/*ADC_ENABLE*/
	SET_BIT(ADCSRA,ADEN);
}


u16 ADC_u16_getDigitalValueSynchBlocking( ADC_Chanel_type chanel )
{
	/*select channel using Mux*/
	
	chanel=chanel&0x1F; //input protection 
	ADMUX=ADMUX&0b11100000; // clear channels pins
	ADMUX=ADMUX|chanel;
	
	/*start conversion*/
	
	SET_BIT(ADCSRA,ADSC);
	
	/*wait tell end of conversion*/
	
	while(READ_BIT(ADCSRA,ADSC)); // wait till conversion time end using busy wait 
	
	return ADC;
}

u8 ADC_u8_getDigitalValueSynchNonBlocking(ADC_Chanel_type chanel,u16*data)
{
	ADC_StartConverstion(chanel);
	
	if(!READ_BIT(ADCSRA,ADSC))
	{
		
		u16 read=ADCL;

		u8*p=&read;

		*(p+1)=ADCH;
		
		*data=read;
		
		ADCflag = 1 ;
		
		return 1 ; 
	}
	
	else
	{
		return 0 ;
	}
}

void ADC_void_getDigitalValueAsynchCallBack(ADC_Chanel_type channel , void (*Localptr) (void) )
{
	/*Call Back Function Setter*/
	ADC_PTR=Localptr;
	
	/*Enable Interrupt*/
	SET_BIT(ADCSRA,ADIE);
	
	ADC_StartConverstion(channel);
	
}

 void ADC_StartConverstion(ADC_Chanel_type channel)
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




ISR(ADC_vect)
{
	if(ADC_PTR!=NULLPTR)
	{
		ADC_PTR();
	}
}

u8 ADC_u16_getVoltageValueSynchNonBlocking(ADC_Chanel_type channel,u16 Voltage_Ref,u16 * Voltage_Read)
{
	
	u16 memory_read;
	
	if(ADC_u8_getDigitalValueSynchNonBlocking(channel,&memory_read))
	{
			*Voltage_Read =(memory_read*(u32)Voltage_Ref)/RESOLUTION;
			
			return 1;
	}
	else
	{
		return 0 ;
	}
}