// WH1602D.c

#include "includes.h"

//Generate delay in ~ value * 1us;
void Delay(int value)
{
    for(int j = 0; j < value; j++)
      for(int i = 0; i < 10; i++)
        i = i;
}

void SetDataBusDirection(BOOL dir)
{
    if(dir)
    {
        FIO1DIR |= BIT_1;  //DB0  
        FIO1DIR |= BIT_4;  //DB1
        FIO1DIR |= BIT_8;  //DB2
        FIO1DIR |= BIT_9;  //DB3
        FIO1DIR |= BIT_10; //DB4
        FIO1DIR |= BIT_14; //DB5
        FIO1DIR |= BIT_15; //DB6
        FIO1DIR |= BIT_16; //DB7
    }
    else
    {
        FIO1DIR &= ~BIT_1;  //DB0  
        FIO1DIR &= ~BIT_4;  //DB1
        FIO1DIR &= ~BIT_8;  //DB2
        FIO1DIR &= ~BIT_9;  //DB3
        FIO1DIR &= ~BIT_10; //DB4
        FIO1DIR &= ~BIT_14; //DB5
        FIO1DIR &= ~BIT_15; //DB6
        FIO1DIR &= ~BIT_16; //DB7
    }
}

void SetDataBusValue( BYTE value)
{
    if(value & (1 << 0))
      FIO1SET |= BIT_1;
    else
      FIO1CLR |= BIT_1;
    
    if(value & (1 << 1))
      FIO1SET |= BIT_4;
    else
      FIO1CLR |= BIT_4;
    
    if(value & (1 << 2))
      FIO1SET |= BIT_8;
    else
      FIO1CLR |= BIT_8;
    
    if(value & (1 << 3))
      FIO1SET |= BIT_9;
    else
      FIO1CLR |= BIT_9;
    
    if(value & (1 << 4))
      FIO1SET |= BIT_10;
    else
      FIO1CLR |= BIT_10;
    
    if(value & (1 << 5))
      FIO1SET |= BIT_14;
    else
      FIO1CLR |= BIT_14;
    
    if(value & (1 << 6))
      FIO1SET |= BIT_15;
    else
      FIO1CLR |= BIT_15;
    
    if(value & (1 << 7))
      FIO1SET |= BIT_16;
    else
      FIO1CLR |= BIT_16;   
}

//Write To Instruction Register
void WriteToIR(BYTE value)
{
    FIO0CLR |= BIT_3; //RS -> 0
    FIO0CLR |= BIT_2; //RW -> 0
    
    SetDataBusDirection(TRUE);
    SetDataBusValue(value);
        
    Delay(1);
    FIO1SET |= BIT_0; //E  -> 1
    Delay(1);
    FIO1CLR |= BIT_0; //E  -> 0
    Delay(1);
}

//Write To Data Register
void WriteToDR(BYTE value)
{
    FIO0SET |= BIT_3; //RS -> 1
    FIO0CLR |= BIT_2; //RW -> 0
    
    SetDataBusDirection(TRUE);
    SetDataBusValue(value);
    
    Delay(1);
    FIO1SET |= BIT_0; //E  -> 1
    Delay(1);
    FIO1CLR |= BIT_0; //E  -> 0
    Delay(1);
        
    Delay(50);
}

void InitWH1602D( void )
{
    FIO0DIR |= BIT_3; //RS
    FIO0DIR |= BIT_2; //RW
    FIO1DIR |= BIT_0; //E
    
    FIO0CLR |= BIT_3 | BIT_2; 
    FIO1CLR |= BIT_0;
     
    //Power On  
    //Wait for more than 30 ms
    Delay(50000);
    
    //Function set
    WriteToIR(0x3C);
    Delay(50);
    
    //Display ON/OFF Control
    WriteToIR(0x0C);
    Delay(50);
    
    //Display Clear
    WriteToIR(0x01);
    Delay(2000);
    
    //Entry Mode Set
    WriteToIR(0x06);
    Delay(50);
}

void PutStringLCD( char *pString01, char *pString02, int Length01, int Length02 )
{
        // Enter critical section
	__disable_interrupt();
                
        //Display Clear
        WriteToIR(0x01);
        Delay(2000);
                
        while( Length01-- )
	{
		WriteToDR((BYTE)(*pString01++));
	}   
        
        //Entry Mode Set
        WriteToIR(0xC0);
        Delay(50);
        
        while( Length02-- )
	{
		WriteToDR((BYTE)(*pString02++));
	}
        
        // Exit critical section
        __enable_interrupt();
}
