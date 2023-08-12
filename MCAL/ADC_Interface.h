#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_

#define VREF_AREF 0b00000000
#define VREF_VCC  0b01000000
#define VREF_256  0b11000000

#define LEFT_ADJUSTMENT  0b00000000
#define RIGHT_ADJUSTMENT 0b00100000

#define PRE_SCALER_2     0b00000000
#define PRE_SCALER_4     0b00000010	
#define PRE_SCALER_8     0b00000011
#define PRE_SCALER_16    0b00000100
#define PRE_SCALER_32    0b00000101
#define PRE_SCALER_64    0b00000110
#define PRE_SCALER_128   0b00000111

#define CONVERSION_TIME_MICRO 250

//Select  Voltage Reference : 1-VREF_AREF , 2-VREF_VCC , 3-VREF_256

#define VOLTAGE_REF VREF_VCC

//Select Adjustment : LEFT_ADJUSTMENT or RIGHT_ADJUSMENT

#define ADJUSTMENT  LEFT_ADJUSTMENT

//Select Pre-scaler Factor : PRE_SCALER_2,PRE_SCALER_4,PRE_SCALER_16,PRE_SCALER_32,PRE_SCALER_64,PRE_SCALER_128

#define PRE_SCALER  PRE_SCALER_64



typedef enum
{
	CH_0,
	CH_1,
	CH_2,
	CH_3,
	CH_4,
	CH_5,
	CH_6,
	CH_7
	}ADC_Channel_type;
	
	#define RESOLUTION 1024
	#define VOLTAGE_REFRENCE_VALUE_MV 5000
	
void ADC_Init(void);
u16 ADC_Read(ADC_Channel_type channel);

u16 ADC_ReadVolt(ADC_Channel_type channel);
void ADC_StartConverstion(ADC_Channel_type channel);
u16 ADC_GetRead(void);
u8 ADC_GetRead_Periodic(u16*data);



#endif