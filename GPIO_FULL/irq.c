#include "includes.h"

void InitVIC(void) 
{
DWORD i = 0;
	DWORD *vect_addr, *vect_cntl;
	
	/* Initialize VIC*/
	VICINTENCLEAR  = 0xFFFFFFFF;
	VICADDRESS     = 0;
	VICINTSELECT   = 0;
	
	/* Set all the vector and vector control register to 0 */
	for ( i = 0; i < VIC_SIZE; i++ )
	{
		vect_addr = (DWORD *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + i * 4);
		vect_cntl = (DWORD *)(VIC_BASE_ADDR + VECT_CNTL_INDEX + i * 4);
		*vect_addr = 0x0;	
		*vect_cntl = 0xF;
	}
}

void InstallIRQ( DWORD IntNumber, void *HandlerAddr, DWORD Priority )
{
	DWORD *vect_addr;
	DWORD *vect_cntl;
	
	// Disable Interrupt
	VICINTENCLEAR = 1 << IntNumber;
	
	// Find first un-assigned VIC address for the handler
	
	vect_addr = (DWORD *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + IntNumber * 4);
	vect_cntl = (DWORD *)(VIC_BASE_ADDR + VECT_CNTL_INDEX + IntNumber * 4);
	
	// Set interrupt vector
	*vect_addr = (DWORD)HandlerAddr;
	*vect_cntl = Priority;
	
	// Enable Interrupt
	VICINTENABLE = 1 << IntNumber;
}
