

#include "primitive_data_types.h"
#include "BIT_MATH.h"
#include "TWI_config.h"
#include "TWI_interface.h"
#include "TWI_private.h"
#include "TWI_register.h"
#include "DIO_private.h"
void TWI_voidMasterInit(void)
{
	/*Set frequency to 400kbps*/
	CLR_BIT(TWSR,TWSR_TWPS0);
	CLR_BIT(TWSR,TWSR_TWPS1);

	TWBR=2;

	/*Enable TWI */
	TWCR= (1<<TWCR_TWEA )| (1<<TWCR_TWEN) ;

}

void TWI_voidSlaveInit(uint8_t Copy_u8Address)
{
	/*Set the slave address to the 7MSB of address register*/
	TWAR = Copy_u8Address<<1;

	   /*Enable TWI */
	SET_BIT(TWCR,TWCR_TWEN);
}

TWI_ErrorStatus TWI_SendStartCondition(void)
{
	TWI_ErrorStatus Local_Error= NoError;

	/*Set the start condition*/
//	SET_BIT(TWCR,TWCR_TWSTA);

	/*Clear interrupt flag to start the previous operation*/
	//SET_BIT(TWCR,TWCR_TWINT);
	TWCR = (1<<TWCR_TWINT)|(1<<TWCR_TWSTA)|(1<<TWCR_TWEN)|(1<<TWCR_TWEA );
	/*Polling on the interrupt flag*/
	while((GET_BIT(TWCR,TWCR_TWINT))==0);

	/*check on the operation status ID*/
	if((TWSR & 0xF8) != START_ACK)
	{
		Local_Error= StartConditionErr;
	}
	else
	{
		/*Do nothing*/
	}
	return Local_Error;
}

TWI_ErrorStatus TWI_SendRepeatedStart(void)
{
	TWI_ErrorStatus Local_Error= NoError;

	/*Set the start condition*/
	//SET_BIT(TWCR,TWCR_TWSTA);

	/*Clear interrupt flag to start the previous operation*/
	//SET_BIT(TWCR,TWCR_TWINT);
	TWCR=(1<<TWCR_TWINT) |(1<<TWCR_TWSTA)|(1<<TWCR_TWEN)|(1<<TWCR_TWEA );
	/*Polling on the interrupt flag*/
	while((GET_BIT(TWCR,TWCR_TWINT))==0);

	/*check on the operation status ID*/
	if((TWSR & 0xF8) != REP_START_ACK)
	{
		Local_Error= RepeatedStartError;
	}
	else
	{
		/*Do nothing*/
	}
	return Local_Error;
}

TWI_ErrorStatus TWI_SendSlaveAddressWithWrite(uint8_t Copy_u8SlaveAdress)
{
	TWI_ErrorStatus Local_Error= NoError;

	/*Clear Start condition*/
	//CLR_BIT(TWCR,TWCR_TWSTA);

	/*set the 7 bit slave address on the bus with write request*/
	TWDR= Copy_u8SlaveAdress<<1;

	/*Clear interrupt flag to start the previous operation*/
	//SET_BIT(TWCR,TWCR_TWINT);
	TWCR = (1<<TWCR_TWINT) | (1<<TWCR_TWEN)|(1<<TWCR_TWEA );
	/*Polling on the interrupt flag*/
	while((GET_BIT(TWCR,TWCR_TWINT))==0);

	/*check on the operation status ID*/
	if((TWSR & 0xF8) != SLAVE_ADD_AND_WR_ACK)
	{
		Local_Error= SlaveAddressWithWriteErr;
	}
	else
	{
		/*Do nothing*/
	}
	return Local_Error;
}

TWI_ErrorStatus TWI_SendSlaveAddressWithRead(uint8_t Copy_u8SlaveAdress)
{
	TWI_ErrorStatus Local_Error= NoError;

	/*Clear Start condition*/
	//CLR_BIT(TWCR,TWCR_TWSTA);

	/*set the 7 bit slave address on the bus with Read request*/
	TWDR= (Copy_u8SlaveAdress<<1) |(1<<0);

	/*Clear interrupt flag to start the previous operation*/
	//SET_BIT(TWCR,TWCR_TWINT);
	TWCR = (1<<TWCR_TWINT) | (1<<TWCR_TWEN)|(1<<TWCR_TWEA );
	/*Polling on the interrupt flag*/
	while((GET_BIT(TWCR,TWCR_TWINT))==0);

	/*check on the operation status ID*/
	if((TWSR & 0xF8) != SLAVE_ADD_AND_RD_ACK)
	{
		Local_Error= SlaveAddressWithReadErr;
	}
	else
	{
		/*Do nothing*/
	}
	return Local_Error;
}

TWI_ErrorStatus TWI_MstrSendDataByte(uint8_t Copy_u8Data)
{
	TWI_ErrorStatus Local_Error= NoError;

	/*Write the data in the data register*/
	TWDR= Copy_u8Data;


	/*Clear interrupt flag to start the previous operation*/
	//SET_BIT(TWCR,TWCR_TWINT);

	TWCR = (1<<TWCR_TWINT) | (1<<TWCR_TWEN)|(1<<TWCR_TWEA );
	/*Polling on the interrupt flag*/
	while((GET_BIT(TWCR,TWCR_TWINT))==0);

	/*check on the operation status ID*/
	if((TWSR & 0xF8) != MSTR_WR_BYTE_ACK)
	{
		Local_Error= MstrWriteByteErr;
	}
	else
	{
		/*Do nothing*/
	}

	return Local_Error;
}

TWI_ErrorStatus TWI_MstrReadDataByte(uint8_t *Copy_pu8Data)
{
	TWI_ErrorStatus Local_Error= NoError;

	/*Enable Acknowledge bit generation of master receiver*/
	SET_BIT(TWCR,TWCR_TWEA);

	/*Polling on the interrupt flag*/
	while((GET_BIT(TWCR,TWCR_TWINT))==0);

	/*check on the operation status ID*/
	if((TWSR & 0xF8) != MSTR_RD_BYTE_WITH_ACK)
	{
		Local_Error= MstrReadByteWithAckErr;
	}
	else
	{
		*Copy_pu8Data= TWDR;
	}
	return Local_Error;
}

void TWI_SendStopCondition(void)
{
	/*Set the stop condition bit*/
	SET_BIT(TWCR,TWCR_TWSTO);

	/*Clear interrupt flag to start the previous operation*/
	SET_BIT(TWCR,TWCR_TWINT);
}



void TWI_SlaveRead(void)
{
	TWCR = (1 << TWCR_TWINT) |  (1 << TWCR_TWEN);
	while((GET_BIT(TWCR,TWCR_TWINT))==0);

}

