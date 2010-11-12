#include "includes.h"
#include "irq.h"

void vicInit(void) 
{
	DWORD i = 0;
	DWORD *vect_addr, *vect_cntl;
	
	VICINTENCLEAR	= 0xFFFFFFFF; //Инициализация VIC
	VICADDRESS		= 0;
	VICINTSELECT	= 0;
	
	for ( i = 0; i < VIC_SIZE; i++ ) //Начальная установка в 0 векторов 
	{
		vect_addr = (DWORD *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + i * 4);
		vect_cntl = (DWORD *)(VIC_BASE_ADDR + VECT_CNTL_INDEX + i * 4);
		*vect_addr = 0x0;	
		*vect_cntl = 0xF;
	}
}
void irqAdd( DWORD IntNumber, void *HandlerAddr, DWORD Priority )
{
	DWORD *vect_addr;
	DWORD *vect_cntl;
	
	VICINTENCLEAR = 1 << IntNumber; //Запрет прерывания на время настройки
	// Поиск первого свободного VIC адреса
	vect_addr = (DWORD *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + IntNumber * 4);
	vect_cntl = (DWORD *)(VIC_BASE_ADDR + VECT_CNTL_INDEX + IntNumber * 4);
	//Установка вектора прерывания
	*vect_addr = (DWORD)HandlerAddr;
	*vect_cntl = Priority;
	//Разрешение прерывания
	VICINTENABLE = 1 << IntNumber;
}

void irqInit()
{
	IO0INTENF = 0x0020; //Прерывание по срезу - кнопка на порту P0.5
	
	//Внешнее прерывание устанавливается на EINT3
	irqAdd( EINT3_INT, (void *)irqBtn, 0x02 );
}

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
	SCS |= 0x20;	// Разрешить генератор с кварцевым резонатором 12МГц
	while( !(SCS & 0x40) );	//Подождать стабилизации частоты генератора

	// Настроить порт
	SCS |= 0x01; //Разрешить быстрый ввод/вывод
	FIO2DIR = 0x00FF; // Биты 0-7 порта 2 на вывод для управления светодиодами
	FIO2MASK = 0; //Все разряды порта 2 работают в быстром режиме

	FIO2PIN = 0x00; // Выключить светодиоды
   	
	vicInit(); //Инициализация системы прерываний
	irqInit(); //Инициализация внешних прерываний

	while (1)//Loop forever
	{		              
		//Ничего не делаем - все происходит в функции обработки прерывания
	}
	return 0;
}

__irq __nested __arm void irqBtnCnt()
{
	int counter, i, display;
	if(!counter) counter = 0;
	switch(IO0INTSTATF) {
		case 0x0020:
			counter = 0;
		default:
			counter++;
	}
	FIO2PIN = counter;

	IO0INTCLR  = 0xFFFFFFFF;//Очистка прерываний от GPIO PORT0
	VICADDRESS = 0;
}




/*
