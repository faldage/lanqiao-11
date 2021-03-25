#ifndef __VAR_H
#define __VAR_H

#include <STC/STC15F2K60S2.H>
#include "intrins.h"
#include "ds1302.h"
#include "iic.h"

typedef unsigned char u8;
#define OFF 10
#define O 11
#define P 12
#define E 13
#define N 14
#define aaa 15
#define C 16



//main
extern u8 digital_tube[8];

extern u8 in_password[6];
extern u8 password[6];


void ds1302_brust_read();
void LatchControl(u8 num ,u8 value);

//button
void check_button();
void respond_to_button();

extern u8 input_count;

//show
enum states
{
	INITIAL=1, INPUT, WRONG, WAIT, CHANGE_PASSWORD
};
extern enum states show_state;

void change_show();

#endif