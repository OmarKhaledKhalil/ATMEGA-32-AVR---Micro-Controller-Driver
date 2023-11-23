#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_

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
}ADC_Chanel_type;

typedef enum
{
	VREF_AREF = 0b00000000,
	VREF_VCC  = 0b01000000,
	VREF_256  = 0b11000000
}
ADC_VoltageRef_type;

typedef enum
{
	FREE_RURNINIG        =  0b00000000,
	ANALOG_COMPARATOR    =  0b00100000,
	EXTERNAL_INTERRUPT   =  0b01000000,
	TIMER0_CM            =  0b01100000,
	TIMER0_OVF           =  0b10000000,
	TIMER1_CM            =  0b10100000,
	TIMER1_OVF           =  0b11000000,
	TIMER1_CE            =  0b11100000
}Auto_Trigger_source;

typedef enum
{
	PRE_SCALER_2    =  0b00000000,
	PRE_SCALER_4    =  0b00000010,
	PRE_SCALER_8    =  0b00000011,
	PRE_SCALER_16   =  0b00000100,
	PRE_SCALER_32   =  0b00000101,
	PRE_SCALER_64   =  0b00000110,
	PRE_SCALER_128  =  0b00000111
	
}ADC_PreScalling_type;


typedef struct  
{
	ADC_VoltageRef_type Voltage_Refrence;
	Auto_Trigger_source Auto_Trigger;
}
Sensor_Config;
	
	
void ADC_void_Init( Sensor_Config * Config , ADC_PreScalling_type Pre_Scaller );
u16 ADC_u16_getDigitalValueSynchBlocking( ADC_Chanel_type chanel );
u8 ADC_u8_getDigitalValueSynchNonBlocking(ADC_Chanel_type chanel,u16*data);
void ADC_void_getDigitalValueAsynchCallBack(ADC_Chanel_type channel , void (*Localptr) (void) );
void ADC_StartConverstion(ADC_Chanel_type channel);
u8 ADC_u16_getVoltageValueSynchNonBlocking(ADC_Chanel_type channel,u16 Voltage_Ref,u16 * Voltage_Read);

#endif