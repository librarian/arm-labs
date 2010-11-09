int ENCA_cur, ENCA_prev; // pin A encoder state
int ENCB_cur, ENCB_prev; // pin B encoder state
int BTN_cur, BTN_prev; // pin btn status
void main (void)
{
	SCS |= 0x01;
	FIO0DIR = 0x00000000; 
	FIO2DIR = 0x00FF; 
	FIO2MASK = 0; 
	while (1)//Loop forever
	{
		int a;
		int freeze;
		int status;
		int top, left, right, bottom;
		int memory; // variable that set from which step we will begin scale;
		int speed = 3;
		int delay;
		left = GetBTNState(1);
		top	= GetBTNState(2);
		if(!memory) memory = 1; //in the beggining memory os not set
		if (left) // btn 1 pressed
		{
			a = memory; // set step
			while (a <= 9) // infinite loop, really
			{
				status = GetEncState(); // take status of encoder
				top	= GetBTNState(2); // stop or not?
				if (status == 1) speed++; // speed increase
				if (status == -1) speed--; // speed decrease
				delay = power(10,speed); // set delay between taking status, turning on lights, etc
				// BUG:
				// It is better to realise getting status on interrupts, because 
				// we will get it only when delay ended
				FIO2PIN = Scale(a); 
				Delay(delay);
				if (top) // btn 2 pressed
				{
					memory = a; // remember step we are stopped
					break; // get out of loop
				}
				a++;
				if (a == 8) a = 1; // if we on the end of scale, we will move to begin of it
			}
		}
		Delay(1); // immediate start if button pressed
	}
}
int GetEncState()
{
	int state = 0;

	ENCA_cur = (FIO0PIN & 0x00000800) >> 11; //������ �������� ��������� �������� � �������� (P0.11)
	ENCB_cur = (FIO0PIN & 0x00200000) >> 21; //������ �������� ��������� �������� B �������� (P0.21)

	if( (ENCA_cur != ENCA_prev) )	//���� ��������� �������� � �������� ����������
	{
		if( !ENCA_cur )		//���� ������� ��������� �������� � �������� - 0 (������� �� 1)
		{
			if( !ENCB_cur )	//���� ������� �������� B �������� - 0
				state = 1;	//���������� ��������� ��������, ��� �������� �� �������
			else
				state = -1;	//���������� ��������� ��������, ��� �������� ������ �������
		}
		else				//���� ������� ��������� �������� � �������� - 1 (������� �� 0)
		{
			if( ENCB_cur )	//���� ������� �������� B �������� - 1
				state = 1;	//���������� ��������� ��������, ��� �������� �� �������
			else
				state = -1;	//���������� ��������� ��������, ��� �������� ������ �������
		}
	}

	ENCA_prev = ENCA_cur;	//������� ��������� ����������� � ����������,
							//������� ����� ����������� � ��������� ����� ��������
	ENCB_prev = ENCB_cur;

	return state;	//���������� ��������� 0 ���� ������� �� ���������, 
					//1 ���� ��������� �� ������� � -1 ���� ������ �������
}

int getBTNState(int btn)
{
	int state = 0;
	switch (btn) 
	{
		case 1:
			BTN_cur = FIO0PIN & 0x0020; // button 1
		case 2:
			BTN_cur = FIO0PIN & 0x0040; // button 2
		case 3:
			BTN_cur = FIO0PIN & 0x0080; // button 3
		case 4:
			BTN_cur = FIO0PIN & 0x0200; // button 4
		default: 
			BTN_cur = FIO0PIN & 0x0020; // default button 1
	}
	if( BTN_cur != BTN_prev) // state changed
	{
		if( !BTN_cur ) // if zero, button pressed
			state = 1; 
	}
	BTN_prev = BTN_cur;	 // remember current button state
	return state; // 0 if do not pressed, 1 if pressed
}


int Scale(int counter)
{
	int a;
	int result = 0x00;
	int result_prev = 0x03;
	for (a=1; a<=counter; a++)
	{
		result = result+result_prev;
		result_prev= result;
	}
	return result;
}

double power(double x, long n) //rescursive power() function from wiki
{
	double tmp; 
	if(n == 0) return 1;
	if(n < 0) return power ( 1 / x, -n);
	if(n % 2) return x * power (x, n - 1);
	return power(x * x, n / 2);
 }

