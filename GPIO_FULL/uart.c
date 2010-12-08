// UART.c

#include "includes.h"

#define TXSIZE_UART0        0x10        /* TX buffer size */
#define RXSIZE_UART0        0x10        /* RX buffer size */

typedef struct
{
	/*--- General ---*/
	unsigned char EOS;
	
	/*--- Receive buffer ---*/
	unsigned char *pRxBuffer;
	unsigned char RxFirst;
	unsigned char RxLast;
	unsigned char RxCount;
	
	/*--- Transmit buffer ---*/
	unsigned char *pTxBuffer;
	unsigned char TxFirst;
	unsigned char TxLast;
	unsigned char TxCount;

} UARTDataType;

static unsigned char TxBufferCh0[ TXSIZE_UART0 ];
static unsigned char RxBufferCh0[ RXSIZE_UART0 ];
static volatile UARTDataType UART0;

void InitUART0 (void)
{
  	unsigned long Fdiv;
	
	// Disable interrupts
	U0IER = 0;
	
  	// RxD0 and TxD0
	PINSEL0 = 0x00000050;
	
	// 8 bits, no Parity, 1 Stop bit
	U0LCR = 0x03;
	
	// Enable access to Divisor Latches
	U0LCR |= 0x80;
	
	// BaudRate
	Fdiv = ( Fpclk / 16 ) / 115200 ;
	
	U0DLM = Fdiv / 256;             
	U0DLL = Fdiv % 256;
	
	// Disable access to Divisor Latches
	U0LCR &= ~0x80;
		
	// Enable and reset TX and RX FIFO
	U0FCR = 0x81;
	
	//Configure UART0 data block
	UART0.EOS = '\n';
	UART0.pTxBuffer   = TxBufferCh0;
	UART0.TxFirst     = 0x00;
	UART0.TxLast      = 0x00;
	UART0.TxCount     = 0x00;
	UART0.pRxBuffer   = RxBufferCh0;
	UART0.RxFirst     = 0x00;
	UART0.RxLast      = 0x00;
	UART0.RxCount     = 0x00; 
	
	InstallIRQ( UART0_INT, (void *)HandlerUART0, HIGHEST_PRIORITY );
	
	// Enable interrupts, except transmit interrupt
	U0IER = IER_RBR | IER_RLS;  
}

void ResetUART0 ( void )
{
	;
}

void PutCharUART0( unsigned char Data )
{
	unsigned char Index;

	// Wait for an empty space in the Tx FIFO
	
	while( UART0.TxCount == TXSIZE_UART0 )
    	{
        	;
    	}

	if( ( UART0.TxCount == 0 ) && ( U0LSR & 0x20 ) )
	{
		// Send character directly
		U0THR = Data;
	}
	else
	{
		// Enter critical section
	  	__disable_interrupt();
	
		// Store in buffer
		Index = UART0.TxLast;
		UART0.pTxBuffer[ Index ] = Data;
	
		// Modify buffer pointers
		UART0.TxLast = (unsigned char)((UART0.TxLast + 1) % TXSIZE_UART0);
		UART0.TxCount++;
	
		// Enable transmit interrupt
		U0IER_bit.THREIE = 1;
	
		// Exit critical section
		__enable_interrupt();
	}
}

void PutStringUART0( char *pString )
{
	while( *pString )
	{
		PutCharUART0( (unsigned char)(*pString++) );
	}	
	return;
}

void PutDataUART0( unsigned char *pData, unsigned char Length )
{
	while( Length-- )
	{
		PutCharUART0( *pData++ );
	}
	return;
}

signed char GetCharUART0( unsigned char *pChar )
{

	unsigned char Index;

    	// Any characters in the Rx buffer?
		
	if( UART0.RxCount > 0 )
    	{
    		// Enter critical section
        	__disable_interrupt();

        	// Retrieve from buffer
        	Index = UART0.RxFirst;
        	*pChar = UART0.pRxBuffer[ Index ];

        	// Modify buffer pointers
        	UART0.RxFirst = (unsigned char)((UART0.RxFirst + 1) % RXSIZE_UART0);
        	UART0.RxCount --;

        	// Exit critical section
		__enable_interrupt();
    }
    else
    {
        	// The buffer is empty
                return STATUS_EMPTY;
    }

    return STATUS_OK;
}

signed char GetStringUART0( char *pString , unsigned char Size )
{
	unsigned char Count;
	signed char   Status;

	// Receive data bytes
	Count = 0;

	// Wait for received string
	do
	{
        	// Wait for received character
		do
		{
			Status = GetCharUART0( (unsigned char *)pString );
            		if( Status == STATUS_ERROR )
            		{
                		// Append zero termination
			        *pString = '\0';

		                // Problem detected
                		return STATUS_ERROR;
			}
		}
		while( Status != STATUS_OK );
	}
	while( ( *pString++ != UART0.EOS ) && ( ++Count < ( Size-1 ) ) );
	
	// Append zero termination
    	*pString = '\0';
	
	// Determine return code
	if( ++Count == Size )
    	{
        	return STATUS_FULL;
    	}
	
	return STATUS_OK;
}

signed char GetDataUART0( unsigned char *pData, unsigned char Length )
{
	signed char Status;

	// Wait for received data
	do
	{
		// Wait for received character
		do
		{
	    		Status = GetCharUART0( pData );
	    		if( Status == STATUS_ERROR )
	    		{	
				// Problem detected
				return STATUS_ERROR;
	    		}
		} 
		while( Status != STATUS_OK );
	
		*pData++;
	} 
	while( --Length );
	
	return STATUS_OK;
}

void SetEOSUART0( unsigned char Symbol )
{
	// Set end of string symbol
    	UART0.EOS = Symbol;
}

__irq __nested __arm void HandlerUART0( void )
{
	unsigned char Status;
	unsigned char Index;

	//Read the interrupt status code
	Status = U0IIR_bit.IID;

	
	switch(Status)
	{
		//Continue sending data
		case IIR_THRE:
		{
			//Read the line status code. It is necessary to clear interrupt.
			Status = U0LSR;
			while(( Status & LSR_THRE ) && UART0.TxCount )
			{
				//Send a character
				Index = UART0.TxFirst;
				U0THR = UART0.pTxBuffer[ Index ];

				//Modify buffer pointers
				UART0.TxFirst = (unsigned char)((UART0.TxFirst + 1) % TXSIZE_UART0);
           			UART0.TxCount--;

				//Read status and check if FIFO can take more data/
				Status = U0LSR;
			}

			//More data to send?
			if( !UART0.TxCount )
			{
				//Disable UART data register empty interrupt
				U0IER_bit.THREIE = 0;
			}
		}
		break;

		//Error manage
		case IIR_RSL:	
		{
			//Read the line status code. It is necessary to clear interrupt.
			Status = U0LSR;
		}
		break;

		//Receive data
		case IIR_RDA:	
		//Time out
		case IIR_CTI:	
		{
			//Read the line status code. It is necessary to clear interrupt.
			Status = U0LSR;
			while( Status & LSR_DR_VALID )
			{
				//Detect potential errors
				if(( Status & LSR_ERR ) != 0 )
				{
					//Parity, framing, overrun or break indication in the FIFO
					if( Status &  LSR_RXFE_ERROR )
					{
						//Receive error occured
					}
					if( Status &  LSR_FE_ERROR )
					{
						//Framing error occured
					}
					if( Status & LSR_PE_ERROR )
					{
						//Parity error occured
					}
					if( Status & LSR_OE_ERROR )
					{
						//Overrun error occured
					}
					if( Status & LSR_BI_BREAK )
					{
						//Break indication occured
					}
				}

				//Read the character
				Index = UART0.RxLast;
            			UART0.pRxBuffer[ Index ] = U0RBR;

				//Modify buffer pointers
				UART0.RxLast = (unsigned char)((UART0.RxLast + 1) % RXSIZE_UART0);
            			UART0.RxCount++;
				
				if( UART0.RxCount > RXSIZE_UART0 )
				{
					//RX buffer full
					break;
				}
				//Check status line, more data?
				Status = U0LSR;
			}
		}
		break;

	default:
		{
			//Unknown interrupt
		}
		break;
	}
	
	VICADDRESS = 0;
}

signed char GetLenUART0( void )
{
	return UART0.RxCount;
}

void PurgeUART0( void )
{
	UART0.TxFirst = 0x00;
	UART0.TxLast  = 0x00;
	UART0.TxCount = 0x00;
	
	UART0.RxFirst = 0x00;
	UART0.RxLast  = 0x00;
	UART0.RxCount = 0x00;
}

unsigned char GetUART0Message(UARTMessage * RxMessage)
{
	unsigned char UART0Str[MESSAGE_LENGTH];

	if( GetLenUART0() < MESSAGE_LENGTH )
		return 0;
	else
	{
		if( GetDataUART0( UART0Str , MESSAGE_LENGTH ) == STATUS_OK )
		{
			RxMessage->DevID = UART0Str[0];
			RxMessage->CMD   = UART0Str[1];
			RxMessage->MSB16 = (UART0Str[2] | (UART0Str[3] << 8));
			RxMessage->LSB16 = (UART0Str[4] | (UART0Str[5] << 8));
			RxMessage->CRC   = (UART0Str[6] | (UART0Str[7] << 8));

			PurgeUART0();

			if( RxMessage->CRC == 0xB0BA )
			{
				if( RxMessage->DevID == ANAL_WORM_ID )
					return 1;
				else
					return 0;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			PurgeUART0();
			return 0;
		}
	}
}
