#ifndef SPI_INTERFACE_H_
#define SPI_INTERFACE_H_

typedef enum
{
	SLAVE,
	MASTER
}SPI_Mode_type;



typedef enum
{	
NOT_OKAY=0,
OKAY=1

}SPIdata_Status_type;



void SPI_Init(SPI_Mode_type mode);

u8 SPI_SendReceive(u8 data);

void SPI_SendNoBlock(u8 data);

u8 SPI_ReceivePerodic(u8*pdata);

u8 SPI_Receive_NoBlock(void);

SPIdata_Status_type SPI_SendReceive_withTimeout(u8 data_send,u8 *data_receive);

void SPI_Interrupt_Enable(void);

void SPI_SetCallBack(void(*LocalPtr)(void));

#endif