#include "includes.h"
#include "irq.h"

int a = 0x00;


void InitVIC(void) 
{
  DWORD i = 0;
  DWORD *vect_addr, *vect_cntl;
	
  VICINTENCLEAR  = 0xFFFFFFFF; //Инициализация VIC
  VICADDRESS     = 0;
  VICINTSELECT   = 0;
	
  for(i = 0; i < VIC_SIZE; i++) //Начальная установка в 0 векторов 
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
	
  VICINTENCLEAR = 1 << IntNumber; //Запрет прерывания на время настройки
  // Поиск первого свободного VIC адреса
  vect_addr = (DWORD *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + IntNumber * 4);
  vect_cntl = (DWORD *)(VIC_BASE_ADDR + VECT_CNTL_INDEX + IntNumber * 4);
  // Установка вектора прерывания
  *vect_addr = (DWORD)HandlerAddr;
  *vect_cntl = Priority;
  // Разрешение прерывания
  VICINTENABLE = 1 << IntNumber;
}
////////////////////////изменяемое//////////////////////


__irq __nested __arm void ExternalInterruptsHandler3(void)
{  
  if(IO0INTSTATF & 0x0040) // Если прерывание вызвано нажатием на вторую кнопку
  {
    FIO2PIN = ++a;// Включить светодиоды
  }
   if(IO0INTSTATF & 0x0080) // Если прерывание вызвано нажатием на вторую кнопку
  {
    FIO2PIN = ++a;// Включить светодиоды
  }
  if(IO0INTSTATF & 0x0200) // Если прерывание вызвано нажатием на вторую кнопку
  {
    FIO2PIN = ++a;// Включить светодиоды
  }
   if(IO0INTSTATF & 0x0020) // Если прерывание вызвано нажатием на первую кнопку
  { a= 0x00;
    FIO2PIN = a;        // Включить все светодиоды 
  }
 
  IO0INTCLR  = 0xFFFFFFFF; // Очистка прерываний от GPIO PORT0
  VICADDRESS = 0;
}

void InitExternalInterrupts(void)
{   
  IO0INTENF = 0x0020 | 0x0040| 0x0080| 0x0200; // Прерывание по срезу - кнопка на порту P0.5 или P0.6
  
  // Внешнее прерывание устанавливается на EINT3
  InstallIRQ(EINT3_INT, (void *)ExternalInterruptsHandler3, 0x02);
}

int main(void)
{   
  SCS |= 0x20;	          // Разрешить генератор с кварцевым резонатором 12МГц
  while( !(SCS & 0x40) ); // Подождать стабилизации частоты генератора

  // Настроить порт
  SCS |= 0x01;      // Разрешить быстрый ввод/вывод
  FIO2DIR = 0x00FF; // Биты 0-7 порта 2 на вывод для управления светодиодами
  FIO2MASK = 0;     // Все разряды порта 2 работают в быстром режиме

  FIO2PIN = 0x00;   // Выключить светодиоды
   	
  InitVIC();                // Инициализация системы прерываний
  InitExternalInterrupts(); // Инициализация внешних прерываний

  while(1) // Loop forever
  {		              
    // Ничего не делаем - все происходит в функции обработки прерывания
  }
  return 0;
}
