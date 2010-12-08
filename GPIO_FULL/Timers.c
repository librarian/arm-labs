// Timer.c

#include "includes.h"

__irq __nested __arm void TimerHandler0()
{     
        T0IR       = BIT_0;
        VICADDRESS = 0;
        
        timer0_handler();
}

void InitTimer0(void(*handler)())
{
    //Disable Timer0
    T0TCR &= ~BIT_0;
  
    //Configure Interrupt Register
    T0IR   = BIT_0;
    
    //Configure Count Control Register
    T0CTCR = 0;
    
    //Configure Prescale Counter Register
    T0PC   = 0;
    
    //Configure Prescale Counter Register
    //15000000 - 1Hz
    //1500000  - 10Hz
    //150000   - 100Hz
    //15000    - 1kHz
    
    //5Hz
    T0MR0  = 3000000;
    
    //Configure Match Control Register
    T0MCR  = (BIT_0 | BIT_1);
    
    timer0_handler = handler;
  
    InstallIRQ( TIMER0_INT, (void *)TimerHandler0, 0x03 );
}

void DisableTimer0()
{
    T0TCR &= ~BIT_0;
}

void EnableTimer0()
{
    T0TCR = BIT_0;
}