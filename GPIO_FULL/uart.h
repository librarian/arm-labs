
/*--- Avoid including this file more than once ---*/
#ifndef _IS_INCLUDED_USART0_H
#define _IS_INCLUDED_USART0_H

#define STATUS_OK                1
#define STATUS_DATA              2
#define STATUS_RTR               3

#define STATUS_ERROR            -1
#define STATUS_FULL             -2
#define STATUS_EMPTY            -3
#define STATUS_BUSY             -4

/* Uart Interrupt Enable Type*/
#define IER_RBR                  0x1
#define IER_THRE                 0x2
#define IER_RLS                  0x4

/* Uart Interrupt Identification */
#define IIR_RSL                  0x3
#define IIR_RDA                  0x2
#define IIR_CTI                  0x6
#define IIR_THRE                 0x1

/* Uart line status register bit descriptions */
#define LSR_RXFE                 0x80   /* Bit 7: RXFE (Receive error) */
#define LSR_RXFE_OK              0x00   /* No receive error in FIFO mode or Always 0 in non FIFO mode */
#define LSR_RXFE_ERROR           0x01   /* Parity, framing or break indication in the FIFO */

#define LSR_TEMT                 0x40   /* Bit 6: TEMT (Transmit empty) */
#define LSR_TEMT_TRANSMIT        0x00   /* Transmission in progress */
#define LSR_TEMT_IDLE            0x01   /* Transmitter idle */

#define LSR_THRE                 0x20   /* Bit 5: THRE (Transmit holding register empty) */
#define LSR_THRE_VALID           0x00   /* Valid data in transmit register */
#define LSR_THRE_EMPTY           0x01   /* Empty */

#define LSR_BI                   0x10   /* Bit 4: BI (Break indicator) */
#define LSR_BI_NO                0x00   /* No break */
#define LSR_BI_BREAK             0x01   /* Break condition detected on receive input line */

#define LSR_FE                   0x08   /* Bit 3: FE (Framing error) */
#define LSR_FE_OK                0x00   /* No framing error */
#define LSR_FE_ERROR             0x01   /* Framing error occured */

#define LSR_PE                   0x04   /* Bit 2: PE (Parity error) */
#define LSR_PE_OK                0x00   /* No parity error */
#define LSR_PE_ERROR             0x01   /* Parity error occured */

#define LSR_OE                   0x02   /* Bit 1: OE (Overrun error) */
#define LSR_OE_OK                0x00   /* No overrun error */
#define LSR_OE_ERROR             0x01   /* Overrun error occured */

#define LSR_DR                   0x01   /* Bit 0: DR (Receiver data ready) */
#define LSR_DR_EMPTY             0x00   /* Empty */
#define LSR_DR_VALID             0x01   /* Valid data in receive buffer register */

#define LSR_ERR			(LSR_RXFE | LSR_BI | LSR_FE | LSR_PE | LSR_OE)

#define MESSAGE_LENGTH	         8
#define ANAL_WORM_ID             69

#define TXSIZE_UART0             0x10   /* TX buffer size */
#define RXSIZE_UART0             0x10   /* RX buffer size */

typedef struct
{
	unsigned char  DevID;
	unsigned char  CMD;
	unsigned short MSB16;
	unsigned short LSB16;
	unsigned short CRC;
}UARTMessage;

void          InitUART0       ( void );
void          ResetUART0      ( void );
void          PutCharUART0    ( unsigned char Data );
void          PutStringUART0  ( char *pString );
void          PutDataUART0    ( unsigned char *pData, unsigned char Length );
signed char   GetCharUART0    ( unsigned char *pChar );
signed char   GetStringUART0  ( char *pString , unsigned char Size );
signed char   GetDataUART0    ( unsigned char *pData, unsigned char Length );
void          SetEOStrUART0   ( unsigned char Symbol );
__irq __nested __arm 
void          HandlerUART0    ( void );
signed char   GetLenUART0     ( void );
void          PurgeUART0      ( void );
unsigned char GetUART0Message ( UARTMessage * RxMessag );

#endif