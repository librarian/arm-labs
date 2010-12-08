// Lab2 diodes
// Copyright (C) 2010 Nikita A Menkovich menkovich@gmail.com
// git repository can be found on: https://github.com/librarian/arm-labs
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#include "includes.h"

static void Delay(unsigned ms)
{
	ms = ms/3000; 
	while (ms != 0 )  { ms--; };
}

void main (void)
{
	// Настроить порт
	SCS |= 0x01; //Разрешить быстрый ввод/вывод
	FIO0DIR = 0x00000000; // Все разряды порта 0 на ввод для чтения состояния кнопок и энкодера
	FIO0MASK = 0; //Все разряды порта 0 работают в быстром режиме
	FIO2DIR = 0x00FF; // Биты 0-7 порта 2 на вывод для управления светодиодами
	FIO2MASK = 0; //Все разряды порта 2 работают в быстром режиме
	int task;
	int a;
	int delay;
	task = 8;
	while (1)//Loop forever
	{
		if (task == 7)
		{
			int memory;
			int dec;
			FIO2PIN = 0x00; // Turn off lights
			delay = 100000; // Base delay
			for (a=1; a<=8; a++) // oooo -> ooo0 -> oo00 -> o000 -> 0000
			{
				Delay( delay );
				FIO2PIN = FIO2PIN*0x02+0x01; // level^2+1
			}
			Delay( delay*3 );
			memory = 0xFF;
			dec=0x100;
			for (a=8; a>=1; a--) // 0000 -> o000 -> oo00 -> ooo0 -> oooo
			{
				Delay( delay );
				dec = dec/0x02;
				memory = memory - dec;
				FIO2PIN = memory;
			}
		} else if (task == 8)
		{
			int memory;
			int dec;
			FIO2PIN = 0x00;
			delay = 100000;
			for (a=1; a<=8; a++) // oooo -> ooo0 -> oo00 -> o000 -> 0000
			{
				Delay( delay );
				FIO2PIN = FIO2PIN*0x02+0x01;
			}
			Delay( delay*3 );
			memory = 0xFF;
			dec=0x00;
			for (a=8; a>=1; a--) // 0000 -> 000o -> 00oo -> 0ooo -> oooo
			{
				Delay( delay );
				dec = dec*0x02+0x01;
				FIO2PIN = memory-dec;
			}
		} else if (task == 9)
		{ 
			int memory;
			int inc;
			int i;
			int max = 8;
			FIO2PIN = 0x00;
			delay = 100000;
			Delay(delay);
			memory = 0x00;
			for (i=1; i<=8; i++) 
			// oooo -> ooo0 -> oo0o -> o0oo -> 0ooo -> 0oo0 ->
			// -> 0o0o -> 00oo -> 00o0 -> 000o -> 0000
			{
				inc = 0x01;
				// we will descrease max value, because we
				// need to add lamps in cycle to an existed one's
				for (a=1; a<=max; a++)
				{
					Delay( delay );
					FIO2PIN = memory+inc;
					inc= inc << 1; // it is much more faster than inc*2
				}
				memory = FIO2PIN; // here we remember current lamp state after cycle
				max--; // decrease total iterations of cycle
				Delay(delay*2);
			}
		}
		Delay(delay*20);
	}
}
