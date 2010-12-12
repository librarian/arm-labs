// Lab2 buttons and encoder
// Copyright (C) 2010 Nikita A Menkovich menkovich@gmail.com
// git repository can be found on: https://github.com/librarian/arm-labs


#include "includes.h";

// total numbers of buttons
#define NUM_BTN (sizeof BTN_addr / sizeof BTN_addr[0]) 
// total size of array div to total size of first element
// BUG: if elements have different size, we could not take number of elements

// configure buttons, if there will be keyboard, we could add addresses easy
static int BTN_addr[] = { 
	0x0020,					// btn1
	0x0040,					// btn2
	0x0080,					// btn3
	0x0200					// btn4
};

int BTN_cur[NUM_BTN];
int BTN_prev[NUM_BTN];


int ENCA_cur, ENCA_prev; //���������� ���������� �������� � ����������� ��������� �������� � 
int ENCB_cur, ENCB_prev; //���������� ���������� �������� � ����������� ��������� �������� B 

int GetEncState()
{
	int state = 0;

	ENCA_cur = (FIO0PIN & 0x00000800) >> 11; //������ �������� ��������� �������� � �������� (P0.11)
	ENCB_cur = (FIO0PIN & 0x00200000) >> 21; //������ �������� ��������� �������� B �������� (P0.21)

	if( (ENCA_cur != ENCA_prev) )	//���� ��������� �������� � �������� ����������
	{
		if( !ENCA_cur ) //���� ������� ��������� �������� � �������� - 0 (������� �� 1)
		{
			if( !ENCB_cur ) //���� ������� �������� B �������� - 0
				state = 1; //���������� ��������� ��������, ��� �������� �� �������
			else
				state = -1; //���������� ��������� ��������, ��� �������� ������ �������
		}
		else	//���� ������� ��������� �������� � �������� - 1 (������� �� 0)
		{
			if( ENCB_cur ) //���� ������� �������� B �������� - 1
				state = 1; //���������� ��������� ��������, ��� �������� �� �������
			else
				state = -1; //���������� ��������� ��������, ��� �������� ������ �������
		}
      
	ENCA_prev = ENCA_cur;//������� ��������� ����������� � ����������,
				//������� ����� ����������� � ��������� ����� ��������
	ENCB_prev = ENCB_cur;

	return state; //���������� ��������� 0 ���� ������� �� ���������, 
					//1 ���� ��������� �� ������� � -1 ���� ������ �������
	}
}

int GetBTNState(int btn)
{
	int state = 0;
	BTN_cur[btn] = FIO0PIN & BTN_addr[btn];
	if( BTN_cur[btn] != BTN_prev[btn]) // changed
	{
		if( !BTN_cur[btn] ) state = 1; // pressed
	}
	BTN_prev[btn] = BTN_cur[btn];
	return state;
}

static void Delay(unsigned ms)
{
	ms = ms/3000; 
	while (ms != 0 ) { ms--; };
}

 
void main (void)
{
	int a			= 1;	// lamp number
	int direction	= 1;	// rotate left 1, rotate right 0
	int move		= 1;	// if lights moving = 1 
	int speed		= 2;	// speed
	int btn			= 0;	// btn number

	FIO0DIR = 0x00000000; // ��� ������� ����� 0 �� ���� ��� ������ ��������� ������ � ��������
	FIO2DIR = 0x00FF; // ���� 0-7 ����� 2 �� ����� ��� ���������� ������������

	ENCA_cur = ENCA_prev = (FIO0PIN & 0x00000800) >> 11; //������ ���������� ��������� �������� � �������� (P0.11)
	ENCB_cur = ENCB_prev = (FIO0PIN & 0x00200000) >> 21; //������ ���������� ��������� �������� B �������� (P0.21)
	
	for (btn = 0; btn <= NUM_BTN; btn++)
	{
		BTN_cur[btn] = BTN_prev[btn] = FIO0PIN & BTN_addr[btn]; // initialize all buttons
	}

	while (1)//Loop forever
	{
		if ( GetBtnState(0) ) move = 0;					// pause
		if ( GetBtnState(1) ) move = 1;					// play
		if ( GetBtnState(2) ) direction = !direction;	// change direction
		if ( GetBtnState(3) ) direction = !direction;	// change direction
		
		switch ( GetEncState() )
		{
			case 1:			// nd4spd
				i = i++;
				break;
			case -1:		// nd4slw
				i = i--;
				break;
		}
		if ( move )
		{	if ( direction )	a <<= 1; // lights move to the right. equal *2
				else			a >>= 1; // lights move to the left. equal *2
		}
		
		if ( ( a & 0x0100 ) && direction ) a = 1;	// catch left and revert
		if ( ( !a ) && !direction ) a = 0x0080;		// catch right and revert
		
		FIO2PIN = a; // stdout
		Delay(i*1000); // delay nearly to 1 sec
	}
}
