// IxtInt.c

#include "includes.h"

OnExternalInterrupt3 *OnExternalInterrupt3Instance;

__irq __nested __arm void ExternalInterruptsHandler3()
{       
        if(IO0INTSTATR & BIT_11)
        {         
          if(FIO0PIN & BIT_21)
            OnExternalInterrupt3Instance(RotaryEncoderLeft);
          else
            OnExternalInterrupt3Instance(RotaryEncoderRight);
        }
          
        if(IO0INTSTATF & BIT_8)
          OnExternalInterrupt3Instance(ButtonOkPressed);
        
        if(IO0INTSTATF & BIT_6)
          OnExternalInterrupt3Instance(ButtonUpPressed);
          
        if(IO0INTSTATF & BIT_9)
          OnExternalInterrupt3Instance(ButtonDownPressed);
          
        if(IO0INTSTATF & BIT_5)
          OnExternalInterrupt3Instance(ButtonLeftPressed);
          
        if(IO0INTSTATF & BIT_7)
          OnExternalInterrupt3Instance(ButtonRightPressed);     
        
        //Clear any interrupt for GPIO PORT0
        IO0INTCLR  = 0xFFFFFFFF;
        VICADDRESS = 0;
}

void InitExternalInterrupts()
{   
    //Interrupt disable for falling edge
    IO0INTENF = (BIT_5 | BIT_6 | BIT_7 | BIT_8 | BIT_9);
    
    //Interrupt enable for rising edge
    IO0INTENR = BIT_11;
      
    //Only EINT3 channel is shared with GPIO interrupts
    InstallIRQ( EINT3_INT, (void *)ExternalInterruptsHandler3, 0x02 );
}

void InstallExternalInterruptHandler(OnExternalInterrupt3 *handler)
{
   OnExternalInterrupt3Instance = handler;    
}
