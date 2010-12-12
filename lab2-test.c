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


int ENCA_cur, ENCA_prev; //Глобальные переменные текущего и предыдущего состояний контакта А 
int ENCB_cur, ENCB_prev; //Глобальные переменные текущего и предыдущего состояний контакта B 

int GetEncState()
{
	int state = 0;

	ENCA_cur = (FIO0PIN & 0x00000800) >> 11; //Чтение текущего состояния контакта А энкодера (P0.11)
	ENCB_cur = (FIO0PIN & 0x00200000) >> 21; //Чтение текущего состояния контакта B энкодера (P0.21)

	if( (ENCA_cur != ENCA_prev) )	//Если состояние контакта А энкодера изменилось
	{
		if( !ENCA_cur ) //Если текущее состояние контакта А энкодера - 0 (переход из 1)
		{
			if( !ENCB_cur ) //Если текущее контакта B энкодера - 0
				state = 1; //Переменная состояния означает, что вращение по часовой
			else
				state = -1; //Переменная состояния означает, что вращение против часовой
		}
		else	//Если текущее состояние контакта А энкодера - 1 (переход из 0)
		{
			if( ENCB_cur ) //Если текущее контакта B энкодера - 1
				state = 1; //Переменная состояния означает, что вращение по часовой
			else
				state = -1; //Переменная состояния означает, что вращение против часовой
		}
      
	ENCA_prev = ENCA_cur;//Текущее состояние сохраняется в переменных,
				//которые будут предыдущими в следующем цикле проверки
	ENCB_prev = ENCB_cur;

	return state; //Возвращаем состояние 0 если энкодер не вращается, 
					//1 если вращается по часовой и -1 если против часовой
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

	FIO0DIR = 0x00000000; // Все разряды порта 0 на ввод для чтения состояния кнопок и энкодера
	FIO2DIR = 0x00FF; // Биты 0-7 порта 2 на вывод для управления светодиодами

	ENCA_cur = ENCA_prev = (FIO0PIN & 0x00000800) >> 11; //Чтение начального состояния контакта А энкодера (P0.11)
	ENCB_cur = ENCB_prev = (FIO0PIN & 0x00200000) >> 21; //Чтение начального состояния контакта B энкодера (P0.21)
	
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
