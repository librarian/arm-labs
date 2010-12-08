// DA136A.c

#include "includes.h"

void InitDA136A()
{
    FIO0DIR |= BIT_9; 
    FIO0SET |= BIT_9;  //CLK1
    FIO0DIR &= ~BIT_8; //DATA1
    
    FIO0DIR |= BIT_7; 
    FIO0SET |= BIT_7;  //CLK2
    FIO0DIR &= ~BIT_6; //DATA2   
}

void GetEncodersData(float* alfa, float* beta)
{
   // Enter critical section
   __disable_interrupt();
   
   WORD _alfa = 0x00;
   WORD _beta = 0x00;
   
   //Latch Data
   Delay(5);
   FIO0CLR |= (BIT_9 | BIT_7);
   Delay(5);
   FIO0SET |= (BIT_9 | BIT_7);
      
   for(int i = 0; i < 12; i++)
   {
      Delay(5);
      FIO0CLR |= (BIT_9 | BIT_7);
      Delay(5);
      
      if(FIO0PIN & BIT_8)
        _alfa  |= (1 << (11 - i));
      
      if(FIO0PIN & BIT_6)
        _beta |= (1 << (11 - i));
      
      FIO0SET |= (BIT_9 | BIT_7); 
   }
   
   Delay(50);
   
   *alfa = 360.0 * (float)_alfa / 4095.0;
   *beta = 346.9 - 360.0 * (float)_beta / 4095.0;
   
   // Exit critical section
   __enable_interrupt();
}
