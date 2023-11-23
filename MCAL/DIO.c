#include "StdTypes.h"
#include "Utils.h"

#include "MCU_HW.h"

#include "DIO_Interface.h"
#include "DIO_Private.h"
#include "DIO_Cfg.h"

extern const DIO_PinStatus_type PinsStatusArray[TOTAL_PINS];


void DIO_voidInit(void)
{
	DIO_Pin_type i;
	for (i=PINA0;i<TOTAL_PINS;i++)
	{
		DIO_voidInitPin(i,PinsStatusArray[i]);
	}
}



static void DIO_voidInitPin(DIO_Pin_type pin,DIO_PinStatus_type status)
{
	DIO_Port_type port =pin/8;
	u8 pin_num=pin%8;
	
	switch(status)
	{
		case OUTPUT:
		switch(port)
		{
			case PA:
			SET_BIT(PORTA->DDR.REG,pin_num);
			CLR_BIT(PORTA->PORT.REG,pin_num);
			break;
			case PB:
			SET_BIT(PORTB->DDR.REG,pin_num);
			CLR_BIT(PORTB->PORT.REG,pin_num);
			break;
			case PC:
			SET_BIT(PORTC->DDR.REG,pin_num);
			CLR_BIT(PORTC->PORT.REG,pin_num);
			break;
			case PD:
			SET_BIT(PORTD->DDR.REG,pin_num);
			CLR_BIT(PORTD->PORT.REG,pin_num);
			break;
		}
		break;
		case INFREE:
		switch(port)
		{
			case PA:
			CLR_BIT(PORTA->DDR.REG,pin_num);
			CLR_BIT(PORTA->PORT.REG,pin_num);
			break;
			case PB:
			CLR_BIT(PORTB->DDR.REG,pin_num);
			CLR_BIT(PORTB->PORT.REG,pin_num);
			break;
			case PC:
			CLR_BIT(PORTC->DDR.REG,pin_num);
			CLR_BIT(PORTC->PORT.REG,pin_num);
			break;
			case PD:
			CLR_BIT(PORTD->DDR.REG,pin_num);
			CLR_BIT(PORTD->PORT.REG,pin_num);
			break;
		}
		break;
		
		case INPULL:
		switch(port)
		{
			case PA:
			CLR_BIT(PORTA->DDR.REG,pin_num);
			SET_BIT(PORTA->PORT.REG,pin_num);
			break;
			case PB:
			CLR_BIT(PORTB->DDR.REG,pin_num);
			SET_BIT(PORTB->PORT.REG,pin_num);
			break;
			case PC:
			CLR_BIT(PORTC->DDR.REG,pin_num);
			SET_BIT(PORTC->PORT.REG,pin_num);
			break;
			case PD:
			CLR_BIT(PORTD->DDR.REG,pin_num);
			SET_BIT(PORTD->PORT.REG,pin_num);
			break;
		}
		break;
	}
}



void DIO_voidWritePin(DIO_Pin_type pin,DIO_PinVoltage_type volt)
{
	DIO_Port_type port =pin/8;
	u8 pin_num=pin%8;
	
	
	if (volt==HIGH)
	{
		switch(port)
		{
			case PA:
			
			SET_BIT(PORTA->PORT.REG,pin_num);
			break;
			case PB:
			
			SET_BIT(PORTB->PORT.REG,pin_num);
			break;
			case PC:
			
			SET_BIT(PORTC->PORT.REG,pin_num);
			break;
			case PD:
			
			SET_BIT(PORTD->PORT.REG,pin_num);
			break;
		}
		
	}
	else if (volt==LOW)
	{
		switch(port)
		{
			case PA:
			
			CLR_BIT(PORTA->PORT.REG,pin_num);
			break;
			case PB:
			
			CLR_BIT(PORTB->PORT.REG,pin_num);
			break;
			case PC:
			
			CLR_BIT(PORTC->PORT.REG,pin_num);
			break;
			case PD:
			
			CLR_BIT(PORTD->PORT.REG,pin_num);
			break;
		}
	}
	else
	{
		/* do nothing */
	}
	
}

DIO_PinVoltage_type DIO_ReadPin(DIO_Pin_type pin)
{
	DIO_Port_type port =pin/8;
	u8 pin_num=pin%8;
	DIO_PinVoltage_type volt=HIGH;
	
	switch(port)
	{
		case PA:
		
		volt=READ_BIT(PORTA->PIN.REG,pin_num);
		break;
		case PB:
		
		volt=READ_BIT(PORTB->PIN.REG,pin_num);
		break;
		case PC:
		
		volt=READ_BIT(PORTC->PIN.REG,pin_num);
		break;
		case PD:
		
		volt=READ_BIT(PORTD->PIN.REG,pin_num);
		break;
	}
	
	return volt;
}

void DIO_voidTogglePin(DIO_Pin_type pin)
{
	DIO_Port_type port =pin/8;
	u8 pin_num=pin%8;
	
	switch(port)
	{
		case PA:
		
		TOG_BIT(PORTA->PORT.REG,pin_num);
		break;
		case PB:
		
		TOG_BIT(PORTB->PORT.REG,pin_num);
		break;
		case PC:
		
		TOG_BIT(PORTC->PORT.REG,pin_num);
		break;
		case PD:
		
		TOG_BIT(PORTD->PORT.REG,pin_num);
		break;
	}
}

void DIO_voidWritePort(DIO_Port_type port,u8 value)
{
	switch(port)
	{
		case PA:
		PORTA->PORT.REG=value;
		break;
		case PB:
		PORTB->PORT.REG=value;
		break;
		case PC:
		PORTC->PORT.REG=value;
		break;
		case PD:
		PORTD->PORT.REG=value;
		break;
		
	}
}

u8 DIO_u8ReadPort(DIO_Port_type port)
{
	u8 result=0;
	if(port==PA)
	{
		result=PORTA->PIN.REG;
	}
	else if(port==PB)
	{
		result=PORTB->PIN.REG;
	}
	else if(port==PC)
	{
		result=PORTC->PIN.REG;
	}
	else if(port==PD)
	{
		result=PORTD->PIN.REG;
	}
	return result;
}

