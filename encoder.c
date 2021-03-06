﻿int ENCA_cur, ENCA_prev; //Ãëîáàëüíûå ïåðåìåííûå òåêóùåãî è ïðåäûäóùåãî ñîñòîÿíèé êîíòàêòà À 
int ENCB_cur, ENCB_prev; //Ãëîáàëüíûå ïåðåìåííûå òåêóùåãî è ïðåäûäóùåãî ñîñòîÿíèé êîíòàêòà B 
void main (void)
{			  
 	// Íàñòðîèòü ïîðò
	SCS |= 0x01; //Ðàçðåøèòü áûñòðûé ââîä/âûâîä

	FIO0DIR = 0x00000000; // Âñå ðàçðÿäû ïîðòà 0 íà ââîä äëÿ ÷òåíèÿ ñîñòîÿíèÿ êíîïîê è ýíêîäåðà
	FIO0MASK = 0; //Âñå ðàçðÿäû ïîðòà 0 ðàáîòàþò â áûñòðîì ðåæèìå

	FIO2DIR = 0x00FF; // Áèòû 0-7 ïîðòà 2 íà âûâîä äëÿ óïðàâëåíèÿ ñâåòîäèîäàìè
	FIO2MASK = 0; //Âñå ðàçðÿäû ïîðòà 2 ðàáîòàþò â áûñòðîì ðåæèìå

	ENCA_cur = ENCA_prev = (FIO0PIN & 0x00000800) >> 11; //×òåíèå íà÷àëüíîãî ñîñòîÿíèÿ 
//êîíòàêòà À ýíêîäåðà (P0.11)
	ENCB_cur = ENCB_prev = (FIO0PIN & 0x00200000) >> 21; //×òåíèå íà÷àëüíîãî ñîñòîÿíèÿ 
//êîíòàêòà B ýíêîäåðà (P0.21)
	int status;
	int counter =0;
	while (1)//Loop forever
	{
		status = GetEncState();
		if( status == 1 )
		{
			counter++;
			FIO2PIN = Scale(counter); //Âêë. ïåðâûé ñâåòîäèîä åñëè ýíêîäåð âðàùàåòñÿ ïî ÷àñîâîé
		}
		else
		if( status == -1 )
		{
			counter--;
			FIO2PIN = Scale(counter); //Âûêë. ïåðâûé ñâåòîäèîä åñëè ýíêîäåð âðàùàåòñÿ ïðîòèâ ÷àñîâîé
		}
		Delay( 1 ); //Çàäåðæêà ìåæäó îïðîñàìè ñîñòîÿíèÿ >10 ìñåê
	}
}
int Scale(int counter)
{
  int a;
  int result = 0;
  for (a=1; a<=counter; a++)
  {
	result = result*0x02+0x01;
  }
  return result;
}
int Scale2(int counter)
{
  int a;
  int result = 0;
  result = 0x01;
  for (a=1; a<=counter; a++)
  {
	result = result*0x02;
  }
  return result;
}
int GetEncState()
{
	int state = 0;

	ENCA_cur = (FIO0PIN & 0x00000800) >> 11; //×òåíèå òåêóùåãî ñîñòîÿíèÿ êîíòàêòà À ýíêîäåðà (P0.11)
	ENCB_cur = (FIO0PIN & 0x00200000) >> 21; //×òåíèå òåêóùåãî ñîñòîÿíèÿ êîíòàêòà B ýíêîäåðà (P0.21)

	if( (ENCA_cur != ENCA_prev) )	//Åñëè ñîñòîÿíèå êîíòàêòà À ýíêîäåðà èçìåíèëîñü
	{
		if( !ENCA_cur ) //Åñëè òåêóùåå ñîñòîÿíèå êîíòàêòà À ýíêîäåðà - 0 (ïåðåõîä èç 1)
		{
			if( !ENCB_cur ) //Åñëè òåêóùåå êîíòàêòà B ýíêîäåðà - 0
				state = 1; //Ïåðåìåííàÿ ñîñòîÿíèÿ îçíà÷àåò, ÷òî âðàùåíèå ïî ÷àñîâîé
			else
				state = -1; //Ïåðåìåííàÿ ñîñòîÿíèÿ îçíà÷àåò, ÷òî âðàùåíèå ïðîòèâ ÷àñîâîé
		}
		else	//Åñëè òåêóùåå ñîñòîÿíèå êîíòàêòà À ýíêîäåðà - 1 (ïåðåõîä èç 0)
		{
			if( ENCB_cur ) //Åñëè òåêóùåå êîíòàêòà B ýíêîäåðà - 1
				state = 1; //Ïåðåìåííàÿ ñîñòîÿíèÿ îçíà÷àåò, ÷òî âðàùåíèå ïî ÷àñîâîé
			else
				state = -1; //Ïåðåìåííàÿ ñîñòîÿíèÿ îçíà÷àåò, ÷òî âðàùåíèå ïðîòèâ ÷àñîâîé
		}
	}

	ENCA_prev = ENCA_cur;//Òåêóùåå ñîñòîÿíèå ñîõðàíÿåòñÿ â ïåðåìåííûõ,
				//êîòîðûå áóäóò ïðåäûäóùèìè â ñëåäóþùåì öèêëå ïðîâåðêè
	ENCB_prev = ENCB_cur;

	return state; //Âîçâðàùàåì ñîñòîÿíèå 0 åñëè ýíêîäåð íå âðàùàåòñÿ, 
					//1 åñëè âðàùàåòñÿ ïî ÷àñîâîé è -1 åñëè ïðîòèâ ÷àñîâîé
}
