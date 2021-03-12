#include "var.h"

u8 button_check[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
u8 button_state[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

u8 button_flag[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
u8 button_count[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

u8 input_count;

void add_input(u8 num)
{
	u8 i;
	for(i = 5; i > 0; i--){
		in_password[i] = in_password[i - 1];
	}
	in_password[0] = num;
	input_count++;
}

void clear_input()
{
	u8 i;
	for(i = 0; i < 6; i++){
		in_password[i] = OFF;
	}
	input_count = 0;
}

void check_button()
{
	u8 i, loc = 0;
	P3 = 0x0f;
	if(P3 != 0x0f){
		if((P3 & 0x01) == 0) loc = 0;
		if((P3 & 0x02) == 0) loc = 4;
		if((P3 & 0x04) == 0) loc = 8;
		if((P3 & 0x08) == 0) loc = 12;
		P3 = 0xf0;
		if((P3 & 0x10) == 0) loc += 3;
		if((P3 & 0x20) == 0) loc += 2;
		if((P3 & 0x40) == 0) loc += 1;
		if((P3 & 0x80) == 0) loc += 0;
		
		for(i = 0; i < 16; i++){
			if(i == loc){
				button_check[i] = 0;
			} else {
				button_check[i] = 1;
			}
		}
	} else {
		for(i = 0; i < 16; i++){
			button_check[i] = 1;
		}
	}
		
	
	
	for(i = 0; i < 16; i++){ 
		if(button_check[i] != button_state[i]){
			button_count[i]++;
		} else {
			button_count[i] = 0;
		}
		
		if(button_count[i] == 5){
			button_state[i] = button_check[i];
			if(button_state[i] == 0){
				button_flag[i] = 1;
			}
			button_count[i] = 0;
		}
	}
}



//INITIAL=1, INPUT, WRONG, WAIT, CHANGE_PASSWORD
void respond_to_button()
{
	u8 i;
	for(i = 0; i < 10; i++){
		if(button_flag[i] == 1){
			if(show_state == INPUT || show_state == CHANGE_PASSWORD){
				add_input(i);
			}
		}
	}

	if(button_flag[13] == 1){//clear
		switch(show_state){
			case INPUT:
				clear_input();
				break;
			case CHANGE_PASSWORD:
				clear_input();
				break;
		}
	}
	if(button_flag[14] == 1){//change
		switch(show_state){
			case WAIT:
				show_state = CHANGE_PASSWORD;
				clear_input();
		}
	}
	if(button_flag[15] == 1){//input
		switch(show_state){
			case INITIAL:
				show_state = INPUT;
				clear_input();
		}
	}
		
	
	for(i = 0; i < 16; i++){
		button_flag[i] = 0;
	}
}