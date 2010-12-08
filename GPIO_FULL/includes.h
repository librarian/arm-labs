#ifndef __INCLUDE_H 
#define __INCLUDE_H

#include  <intrinsics.h>
#include "NXP/iolpc2368.h"
#include <stdio.h>
#include "type.h"
#include "irq.h"
#include "uart.h"
#include "bit.h"
#include "WH1602D.h"
#include "ExtInterrupts.h"
#include "Timers.h"

/* PLL Setting Table Matrix */
/* 	
	Main Osc.	CCLKCFG		Fcco		Fcclk 		M 	N
	12Mhz		29		300Mhz		10Mhz		24	1
	12Mhz		35		360Mhz		10Mhz		14	0					
	12Mhz		27		336Mhz		12Mhz		13	0
	12Mhz		14		300Mhz		20Mhz		24	1		
	12Mhz		17		360Mhz		20Mhz		14	0
	12Mhz		13		336Mhz		24Mhz		13	0
	12Mhz		11		300Mhz		25Mhz		24	1   
	12Mhz		9		300Mhz		30Mhz		24	1
	12Mhz		11		360Mhz		30Mhz		14	0
	12Mhz		9		320Mhz		32Mhz		39	2
	12Mhz		9		350Mhz		35Mhz		174	11
	12Mhz		7		312Mhz		39Mhz		12	0
 	12Mhz		8		360Mhz		40Mhz		14	0 
	12Mhz		7		360Mhz		45Mhz		14	0
	12Mhz		6		336Mhz		48Mhz		13	0  
	12Mhz		5		300Mhz		50Mhz		24	1       
	12Mhz		5		312Mhz		52Mhz		12	0
	12Mhz		5		336Mhz		56Mhz		13	0		
 	12Mhz		4		300Mhz		60Mhz		24	1	selected
  	12Mhz		4		320Mhz		64Mhz		39	2
	12Mhz		4		350Mhz		70Mhz		174	11
	12Mhz		4		360Mhz		72Mhz		14	0		
	12Mhz		3		300Mhz		75Mhz		24	1
	12Mhz		3		312Mhz		78Mhz		12	0  
	12Mhz		3		320Mhz		80Mhz		39	2
	12Mhz		3		336Mhz		84Mhz		13	0 
*/

/*
Fcco = [2 x M x Fosc] / N = 300000000
M = PLL_MSEL + 1
N = PLL_NSEL + 1

Fcclk = Fcco / (CCLKDivValue + 1) = 300000000 / (4 + 1) = 60000000
*/

#define PLL_MSEL	24
#define PLL_NSEL	1
#define Fosc	        12000000
#define Fcco	        300000000
#define Fcclk	        60000000
#define CCLKDivValue    4
/* APB clock frequence , must be 1/2/4 multiples of ( Fcclk/4 ). 
   If USB is enabled, the minimum APB must be greater than 16Mhz */ 
#define Fpclk	        (Fcclk / 4)

#endif
