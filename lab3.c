#include "includes.h"
#include "irq.h"

int a = 0x00;


void InitVIC(void) 
{
  DWORD i = 0;
  DWORD *vect_addr, *vect_cntl;
	
  VICINTENCLEAR  = 0xFFFFFFFF; //������������� VIC
  VICADDRESS     = 0;
  VICINTSELECT   = 0;
	
  for(i = 0; i < VIC_SIZE; i++) //��������� ��������� � 0 �������� 
  {
    vect_addr = (DWORD *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + i * 4);
    vect_cntl = (DWORD *)(VIC_BASE_ADDR + VECT_CNTL_INDEX + i * 4);
    *vect_addr = 0x0;	
    *vect_cntl = 0xF;
  }
}

void InstallIRQ(DWORD IntNumber, void *HandlerAddr, DWORD Priority)
{
  DWORD *vect_addr;
  DWORD *vect_cntl;
	
  VICINTENCLEAR = 1 << IntNumber; //������ ���������� �� ����� ���������
  // ����� ������� ���������� VIC ������
  vect_addr = (DWORD *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + IntNumber * 4);
  vect_cntl = (DWORD *)(VIC_BASE_ADDR + VECT_CNTL_INDEX + IntNumber * 4);
  // ��������� ������� ����������
  *vect_addr = (DWORD)HandlerAddr;
  *vect_cntl = Priority;
  // ���������� ����������
  VICINTENABLE = 1 << IntNumber;
}
////////////////////////����������//////////////////////


__irq __nested __arm void ExternalInterruptsHandler3(void)
{  
  if(IO0INTSTATF & 0x0040) // ���� ���������� ������� �������� �� ������ ������
  {
    FIO2PIN = ++a;// �������� ����������
  }
   if(IO0INTSTATF & 0x0080) // ���� ���������� ������� �������� �� ������ ������
  {
    FIO2PIN = ++a;// �������� ����������
  }
  if(IO0INTSTATF & 0x0200) // ���� ���������� ������� �������� �� ������ ������
  {
    FIO2PIN = ++a;// �������� ����������
  }
   if(IO0INTSTATF & 0x0020) // ���� ���������� ������� �������� �� ������ ������
  { a= 0x00;
    FIO2PIN = a;        // �������� ��� ���������� 
  }
 
  IO0INTCLR  = 0xFFFFFFFF; // ������� ���������� �� GPIO PORT0
  VICADDRESS = 0;
}

void InitExternalInterrupts(void)
{   
  IO0INTENF = 0x0020 | 0x0040| 0x0080| 0x0200; // ���������� �� ����� - ������ �� ����� P0.5 ��� P0.6
  
  // ������� ���������� ��������������� �� EINT3
  InstallIRQ(EINT3_INT, (void *)ExternalInterruptsHandler3, 0x02);
}

int main(void)
{   
  SCS |= 0x20;	          // ��������� ��������� � ��������� ����������� 12���
  while( !(SCS & 0x40) ); // ��������� ������������ ������� ����������

  // ��������� ����
  SCS |= 0x01;      // ��������� ������� ����/�����
  FIO2DIR = 0x00FF; // ���� 0-7 ����� 2 �� ����� ��� ���������� ������������
  FIO2MASK = 0;     // ��� ������� ����� 2 �������� � ������� ������

  FIO2PIN = 0x00;   // ��������� ����������
   	
  InitVIC();                // ������������� ������� ����������
  InitExternalInterrupts(); // ������������� ������� ����������

  while(1) // Loop forever
  {		              
    // ������ �� ������ - ��� ���������� � ������� ��������� ����������
  }
  return 0;
}
