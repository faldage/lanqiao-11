#include "var.h"

volatile int SysTick = 0;

u8 digital_tube[8] = {OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF};
u8 tab[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xff, 0xC0, 0x8c, 0x86, 0xc8, 0xbf, 0xc9};
/////////////0	1			2			3			4		5			6		7		8			9		off			o		p			e			n			-			c

u8 in_password[6] = {OFF, OFF, OFF, OFF, OFF, OFF};
u8 password[6] = {8, 8, 8, 8, 8, 8};

u8 door_state = CLOSE;
enum states show_state = INITIAL;

u8 time_hour;
u8 time_min;
u8 time_seconds;

void Timer0Init(void)
{
	AUXR |= 0x80;
	TMOD &= 0xF0;
	TL0 = 0xCD;
	TH0 = 0xD4;
	TF0 = 0;
	TR0 = 1;
}

void LatchControl(u8 num ,u8 value)
{
	P0 = value;
	_nop_();
	P2 = (P2 & 0x1f) | (num<<5);
	_nop_();
	P2 &= 0x1f;
	_nop_();
}

u8 index = 0;

void func() interrupt 1
{
	SysTick++;
	LatchControl(6, 1<<(7 - index));
	/*
	switch(index){
		case 0:
			LatchControl(7, tab[time_seconds % 10]);
			break;
		case 1:
			LatchControl(7, tab[time_seconds / 10]);
			break;
		case 2:
			LatchControl(7, tab[OFF]);
			break;
		case 3:
			LatchControl(7, tab[time_min / 10]);
			break;
		case 4:
			LatchControl(7, tab[time_min % 10]);
			break;
		case 5:
			LatchControl(7, tab[OFF]);
			break;
		case 6:
			LatchControl(7, tab[time_hour % 10]);
			break;
		case 7:
			LatchControl(7, tab[time_hour / 10]);
			break;
		
	}
	*/
	LatchControl(7, tab[digital_tube[index]]);
	
	
	index = (index + 1) % 8;
}

u8 time_is_up()
{
	if(time_hour != 0 || time_min != 0){
		return 1;
	}
	if(time_seconds >= 5){
		return 1;
	}
	return 0;
}

void read_password()
{
//eeprom
	
}

void write_password()
{
//eeprom
	u8 i;
	for(i = 0; i < 6; i++){
		password[i] = in_password[i];
	}
}

u8 check_password()
{
	u8 i;
	read_password();//
	for(i = 0; i < 6; i++){
		if(in_password[i] != password[i]){
			return 0;
		}
	}
	return 1;
}

void time_init()
{
	Ds1302_Single_Byte_Write(0x80, 0);
	Ds1302_Single_Byte_Write(0x82, 0);
	Ds1302_Single_Byte_Write(0x84, 0);
}

void change_state()
{
	//INITIAL=1, INPUT, CHANGE_PASSWORD
	if(show_state == INPUT && input_count == 6){
		u8 a;
		a = check_password();
		if(a == 1){//right
			show_state = WAIT;
			time_init();
		} else {
			show_state = WRONG;
			time_init();
		}
	}
	if(show_state == CHANGE_PASSWORD && input_count == 6){
		write_password();
		show_state = INITIAL;
	}
	if(show_state == WRONG && time_is_up() == 1){
		show_state = INITIAL;
	}
	if(show_state == WAIT && time_is_up() == 1){
		show_state = INITIAL;
	}
	check_button();
	respond_to_button();
}

void main()
{
	// x101 1111
	LatchControl(5, 0x1f);
	LatchControl(4, 0xff);
	
	Timer0Init();
	time_init();
	EA = 1;
	ET0 = 1;
	//write initial password
	while(1){
		
		int tickBkp = SysTick;
		
		if(tickBkp % 50 == 0){
			ds1302_brust_read();
		}
		
		if(tickBkp % 10 == 0){
			change_state();
			change_show();
		}
		
		while(tickBkp == SysTick);
	}
}

u8 bcd2time(u8 bcd)
{
	return ((bcd >> 4) * 10) + (bcd & 0x0f);
}

void ds1302_brust_read()
{
	RST_CLR;			/*RST脚置低，实现DS1302的初始化*/
	SCK_CLR;			/*SCK脚置低，实现DS1302的初始化*/

	RST_SET;	/*启动DS1302总线,RST=1电平置高 */	 
	Write_Ds1302_Byte(0xbf); /*写入目标地址：addr,保证是读操作,写之前将最低位置高*/
	time_seconds=bcd2time(Read_Ds1302_Byte()); /*从DS1302中读出一个字节的数据*/		
	time_min=bcd2time(Read_Ds1302_Byte());
	time_hour=bcd2time(Read_Ds1302_Byte());
	RST_CLR;	/*停止DS1302总线*/
}