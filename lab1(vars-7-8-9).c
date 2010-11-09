static void Delay(DWORD tick)
{
	while(tick--)
	{
		//Do Nothing
	}
}

void main (void)
{
	// ��������� ����
	SCS |= 0x01; //��������� ������� ����/�����
	FIO0DIR = 0x00000000; // ��� ������� ����� 0 �� ���� ��� ������ ��������� ������ � ��������
	FIO0MASK = 0; //��� ������� ����� 0 �������� � ������� ������
	FIO2DIR = 0x00FF; // ���� 0-7 ����� 2 �� ����� ��� ���������� ������������
	FIO2MASK = 0; //��� ������� ����� 2 �������� � ������� ������
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
			Delay( delay );
			FIO2PIN = 0x00;
			
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
			for (a=8; a>=1; a--) // 0000 -> o000 -> oo00 -> ooo0 -> oooo
			{
				Delay( delay );
				dec = dec*0x02+0x01;
				FIO2PIN = memory-dec;
			}
			Delay( delay );
			FIO2PIN = 0x00;
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
					inc=inc*0x02;
				}
				memory = FIO2PIN; // here we remember current lamp state after cycle
				max--; // decrease total lamps to cycle
				Delay(delay*2);
			}
		}
		Delay(delay*20);
	}
}
