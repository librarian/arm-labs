#include "includes.h";

void Delay(int value)
{
    for(int j = 0; j < value; j++)
      for(int i = 0; i < 10; i++)
        i = i;
}

int ENCA_cur, ENCA_prev; //Глобальные переменные текущего и предыдущего состояний контакта А 
int ENCB_cur, ENCB_prev; //Глобальные переменные текущего и предыдущего состояний контакта B 
int BTN1_cur, BTN1_prev, BTN2_cur, BTN2_prev,BTN3_cur, BTN3_prev, BTN4_cur, BTN4_prev; //Глобальные переменные текущего и предыдущего состояний
 
void main (void)
{         
        int a=1;//само число
        int direction=1;//1-влево  0-вправо
	int move=1;//Если 1 движение, если 0 остановка 
        int i = 2;
        
 	// Настроить порт
	SCS |= 0x01; //Разрешить быстрый ввод/вывод

	FIO0DIR = 0x00000000; // Все разряды порта 0 на ввод для чтения состояния кнопок и энкодера
	FIO0MASK = 0; //Все разряды порта 0 работают в быстром режиме

	FIO2DIR = 0x00FF; // Биты 0-7 порта 2 на вывод для управления светодиодами
	FIO2MASK = 0; //Все разряды порта 2 работают в быстром режиме

	ENCA_cur = ENCA_prev = (FIO0PIN & 0x00000800) >> 11; //Чтение начального состояния контакта А энкодера (P0.11)
	ENCB_cur = ENCB_prev = (FIO0PIN & 0x00200000) >> 21; //Чтение начального состояния контакта B энкодера (P0.21)
	BTN1_cur = BTN1_prev = FIO0PIN & 0x0020; //Чтение начального состояния кнопки 1
        BTN2_cur = BTN2_prev = FIO0PIN & 0x0040; //Чтение начального состояния кнопки 2
        BTN2_cur = BTN2_prev = FIO0PIN & 0x0080; //Чтение начального состояния кнопки 3
        BTN2_cur = BTN2_prev = FIO0PIN & 0x0200; //Чтение начального состояния кнопки 4
                 
	while (1)//Loop forever
	{
          if( GetBtn1State() ){move = 0;}
	  if( GetBtn2State() ) {move = 1;}
	  if(GetBtn3State()){direction=!direction;}//Меняем направление
	  if(GetBtn4State()){direction=!direction;}//Меняем направление
          
          switch(GetEncState())
		{	case 1://УСКОРЕНИЕ
				i=i++;
				break;
			case -1://ЗАМЕДЛЕНИЕ
				i=i--;
				break;                             
                 }
          
          if(move)
		{	if(direction){a<<=1;}
			else{a>>=1;}
		}
		if((a & 0x0100) && direction){a=1;}//ловим крайнее левое состояние и смещаем "каретку"
		if((!a) && !direction){a=0x0080;}//ловим крайнее правое состояние и смещаем "каретку"
		FIO2PIN = a;//Вывод на светодиоды
                Delay(i*10000);
	}        
}
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
        }}
int GetBtn1State()
{
	int state = 0;
	BTN1_cur = FIO0PIN & 0x0020; // Порт Po -5
	if( BTN1_cur != BTN1_prev) //Если состояние изменилось
	{
		if( !BTN1_cur ){state = 1;}//Кнопка нажата а не отпущена
	}
	BTN1_prev = BTN1_cur;
	return state;
}
int GetBtn2State()
{
	int state = 0;
	BTN2_cur = FIO0PIN & 0x0040; // Порт Po -6
	if( BTN2_cur != BTN2_prev) //Если состояние изменилось
	{
		if( !BTN2_cur ){state = 1;}//Кнопка нажата а не отпущена
	}
	BTN2_prev = BTN2_cur;
	return state;
}
int GetBtn3State()
{
	int state = 0;
	BTN3_cur = FIO0PIN & 0x0080; // Порт Po -7
	if( BTN3_cur != BTN3_prev) //Если состояние изменилось
	{
		if( !BTN3_cur ){state = 1;}//Кнопка нажата а не отпущена
	}
	BTN3_prev = BTN3_cur;
	return state;
}
int GetBtn4State()
{
	int state = 0;
	BTN4_cur = FIO0PIN & 0x0200; // Порт Po -9
	if( BTN4_cur != BTN4_prev) //Если состояние изменилось
	{
		if( !BTN4_cur ){state = 1;}//Кнопка нажата а не отпущена
	}
	BTN4_prev = BTN4_cur;
	return state;
}
