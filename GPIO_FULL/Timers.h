// ExtInt.h
#ifndef _TIMER_H_
#define _TIMER_H_

static void (*timer0_handler)();
void        EnableTimer0();
void        DisableTimer0();
void        InitTimer0(void(*handler)());

#endif
