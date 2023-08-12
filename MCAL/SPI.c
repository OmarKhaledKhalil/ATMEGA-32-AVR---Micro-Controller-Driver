#include "StdTypes.h"
#include "MemMap.h"
#include "Utils.h"
#include "SPI_Interface.h"


#include <util/delay.h>

static void (*SPI_Fptr)(void)=NULLPTR;


void SPI_Init(SPI_Mode_type mode)
{
	if (mode==MASTER)
	{
		SET_BIT(SPCR,MSTR);
	}
	else
	{
		CLR_BIT(SPCR,MSTR);
	}
	
	//CLOCK 2 MHZ
	
	// SPI ENABLE
	SET_BIT(SPCR,SPE);
	
}



u8 SPI_SendReceive(u8 data)
{
	SPDR=data;
	while(!READ_BIT(SPSR,SPIF));
	return SPDR;
}


void SPI_SendNoBlock(u8 data)
{
	SPDR=data;
}

u8 SPI_Receive_NoBlock(void)
{
	return SPDR;
}

u8 SPI_ReceivePerodic(u8*pdata)
{
	if (READ_BIT(SPSR,SPIF))
	{
		*pdata=SPDR;
		return 1;
	}
	return 0;
}


void SPI_Interrupt_Enable(void)
{
	SET_BIT(SPCR,SPIE);
}

void SPI_SetCallBack(void(*LocalPtr)(void))
{
	SPI_Fptr=LocalPtr;
}


SPIdata_Status_type SPI_SendReceive_withTimeout(u8 data_send,u8 *data_receive)
{
	SPIdata_Status_type status;
	u8 i=0;
	
	SPDR=data_send;
	
	
    while(!READ_BIT(SPSR,SPIF)&&i<10)
	{
			_delay_us(1);
			i++;
	}
	
	if(i==10)
	{
		status=NOT_OKAY;
	}
	else
	{
		*data_receive=SPDR;
		status=OKAY;
	}
	
	return status;
}


ISR(SPI_STC_vect)
{
	if(SPI_Fptr!=NULLPTR)
	{
		SPI_Fptr();
	}
}