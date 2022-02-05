/*********************************************************/
/***********		Author: ABDULLAH	**************/
/***********		Layer: MCAL			 	**************/
/***********		Component: TWI  		**************/
/***********		Version: 1.00		 	**************/
/*********************************************************/

/*Preprocessor file guard*/
#ifndef TWI_INTERFACE_H_
#define TWI_INTERFACE_H_

typedef enum
{
	NoError,
	StartConditionErr,
	RepeatedStartError,
	SlaveAddressWithWriteErr,
	SlaveAddressWithReadErr,
	MstrWriteByteErr,
	MstrReadByteWithAckErr

}TWI_ErrorStatus;

void TWI_voidMasterInit(void);

void TWI_voidSlaveInit(uint8_t Copy_u8Address);

TWI_ErrorStatus TWI_SendStartCondition(void);

TWI_ErrorStatus TWI_SendRepeatedStart(void);

TWI_ErrorStatus TWI_SendSlaveAddressWithWrite(uint8_t Copy_u8SlaveAdress);

TWI_ErrorStatus TWI_SendSlaveAddressWithRead(uint8_t Copy_u8SlaveAdress);

TWI_ErrorStatus TWI_MstrSendDataByte(uint8_t Copy_u8Data);

TWI_ErrorStatus TWI_MstrReadDataByte(uint8_t *Copy_pu8Data);

void TWI_SendStopCondition(void);


#endif
