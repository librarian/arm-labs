// WH1602D.h
#ifndef _WH1602D_H_
#define _WH1602D_H_

void Delay(int value);
void InitWH1602D( void );
void PutStringLCD( char *pString01, char *pString02, int Length01, int Length02 );

#endif
