#include "includes.h";

void Delay(int value)
{
    for(int j = 0; j < value; j++)
      for(int i = 0; i < 10; i++)
        i = i;
}

int ENCA_cur, ENCA_prev; //���������� ���������� �������� � ����������� ��������� �������� � 
int ENCB_cur, ENCB_prev; //���������� ���������� �������� � ����������� ��������� �������� B 
int BTN1_cur, BTN1_prev, BTN2_cur, BTN2_prev,BTN3_cur, BTN3_prev, BTN4_cur, BTN4_prev; //���������� ���������� �������� � ����������� ���������
 
void main (void)
{         
        int a=1;//���� �����
        int direction=1;//1-�����  0-������
	int move=1;//���� 1 ��������, ���� 0 ��������� 
        int i = 2;
        
 	// ��������� ����
	SCS |= 0x01; //��������� ������� ����/�����

	FIO0DIR = 0x00000000; // ��� ������� ����� 0 �� ���� ��� ������ ��������� ������ � ��������
	FIO0MASK = 0; //��� ������� ����� 0 �������� � ������� ������

	FIO2DIR = 0x00FF; // ���� 0-7 ����� 2 �� ����� ��� ���������� ������������
	FIO2MASK = 0; //��� ������� ����� 2 �������� � ������� ������

	ENCA_cur = ENCA_prev = (FIO0PIN & 0x00000800) >> 11; //������ ���������� ��������� �������� � �������� (P0.11)
	ENCB_cur = ENCB_prev = (FIO0PIN & 0x00200000) >> 21; //������ ���������� ��������� �������� B �������� (P0.21)
	BTN1_cur = BTN1_prev = FIO0PIN & 0x0020; //������ ���������� ��������� ������ 1
        BTN2_cur = BTN2_prev = FIO0PIN & 0x0040; //������ ���������� ��������� ������ 2
        BTN2_cur = BTN2_prev = FIO0PIN & 0x0080; //������ ���������� ��������� ������ 3
        BTN2_cur = BTN2_prev = FIO0PIN & 0x0200; //������ ���������� ��������� ������ 4
                 
	while (1)//Loop forever
	{
          if( GetBtn1State() ){move = 0;}
	  if( GetBtn2State() ) {move = 1;}
	  if(GetBtn3State()){direction=!direction;}//������ �����������
	  if(GetBtn4State()){direction=!direction;}//������ �����������
          
          switch(GetEncState())
		{	case 1://���������
				i=i++;
				break;
			case -1://����������
				i=i--;
				break;                             
                 }
          
          if(move)
		{	if(direction){a<<=1;}
			else{a>>=1;}
		}
		if((a & 0x0100) && direction){a=1;}//����� ������� ����� ��������� � ������� "�������"
		if((!a) && !direction){a=0x0080;}//����� ������� ������ ��������� � ������� "�������"
		FIO2PIN = a;//����� �� ����������
                Delay(i*10000);
	}        
}
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
        }}
int GetBtn1State()
{
	int state = 0;
	BTN1_cur = FIO0PIN & 0x0020; // ���� Po -5
	if( BTN1_cur != BTN1_prev) //���� ��������� ����������
	{
		if( !BTN1_cur ){state = 1;}//������ ������ � �� ��������
	}
	BTN1_prev = BTN1_cur;
	return state;
}
int GetBtn2State()
{
	int state = 0;
	BTN2_cur = FIO0PIN & 0x0040; // ���� Po -6
	if( BTN2_cur != BTN2_prev) //���� ��������� ����������
	{
		if( !BTN2_cur ){state = 1;}//������ ������ � �� ��������
	}
	BTN2_prev = BTN2_cur;
	return state;
}
int GetBtn3State()
{
	int state = 0;
	BTN3_cur = FIO0PIN & 0x0080; // ���� Po -7
	if( BTN3_cur != BTN3_prev) //���� ��������� ����������
	{
		if( !BTN3_cur ){state = 1;}//������ ������ � �� ��������
	}
	BTN3_prev = BTN3_cur;
	return state;
}
int GetBtn4State()
{
	int state = 0;
	BTN4_cur = FIO0PIN & 0x0200; // ���� Po -9
	if( BTN4_cur != BTN4_prev) //���� ��������� ����������
	{
		if( !BTN4_cur ){state = 1;}//������ ������ � �� ��������
	}
	BTN4_prev = BTN4_cur;
	return state;
}
