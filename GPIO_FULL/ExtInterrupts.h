// ExtInt.h
#ifndef _EXT_INT_H_
#define _EXT_INT_H_

//EXTINT3 Sources
typedef enum 
{
    RotaryEncoderLeft = 0,
    RotaryEncoderRight,
    ButtonOkPressed,
    ButtonUpPressed,
    ButtonDownPressed,
    ButtonLeftPressed,
    ButtonRightPressed
}   ExternalInterrupt3Sources;

//EXTINT3 Event Handler Signature
typedef void (OnExternalInterrupt3)(ExternalInterrupt3Sources src);

void InitExternalInterrupts();

void InstallExternalInterruptHandler(OnExternalInterrupt3 *handler);

#endif
