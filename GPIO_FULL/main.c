#include "includes.h"
#include "irq.h"


__irq __nested __arm void irqBtn()
{
	int inc;
	switch(IO0INTSTATF) {
		case 0x0020:
			inc = 0x01;
		case 0x0040:
			inc = 0x02;
		case 0x0080:
			inc = 0x04;
		case 0x0200:
			inc = 0x08;
		default:
			FIO2PIN = 0x00;
	}
	FIO2PIN = FIO2PIN+inc;

	IO0INTCLR  = 0xFFFFFFFF;//Очистка прерываний от GPIO PORT0
	VICADDRESS = 0;
}



int main (void)
{   
	SCS |= 0x20;	// ��������� ��������� � ��������� ����������� 12���
	while( !(SCS & 0x40) );	//��������� ������������ ������� ����������

	// ��������� ����
	SCS |= 0x01; //��������� ������� ����/�����
	FIO2DIR = 0x00FF; // ���� 0-7 ����� 2 �� ����� ��� ���������� ������������
	FIO2MASK = 0; //��� ������� ����� 2 �������� � ������� ������

	FIO2PIN = 0x00; // ��������� ����������
   	
	InitVIC(); //������������� ������� ����������
	InitExternalInterrupts(); //������������� ������� ����������

	while (1)//Loop forever
	{		              
		//������ �� ������ - ��� ���������� � ������� ��������� ����������
	}
	return 0;
}
