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

	IO0INTCLR  = 0xFFFFFFFF;//РћС‡РёСЃС‚РєР° РїСЂРµСЂС‹РІР°РЅРёР№ РѕС‚ GPIO PORT0
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
   	
	InitVIC(); //Инициализация системы прерываний
	InitExternalInterrupts(); //Инициализация внешних прерываний

	while (1)//Loop forever
	{		              
		//Ничего не делаем - все происходит в функции обработки прерывания
	}
	return 0;
}
