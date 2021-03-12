#include "var.h"

void change_show()
{
	u8 i, temp;
	//INITIAL=1, INPUT, WRONG, WAIT, CHANGE_PASSWORD
	//led 5 digital
	if(show_state == INITIAL){
		LatchControl(4, 0xff);
		for(i = 0; i < 8; i++){
			digital_tube[i] = OFF;
		}
	} else if(show_state == INPUT){
		LatchControl(4, 0xbf);
		digital_tube[7] = aaa;
		digital_tube[6] = OFF;
		for(i = 0; i <6; i++){
				digital_tube[i] =  in_password[i];
			
		}
	} else if(show_state == WRONG){
		LatchControl(4, 0xfe);
		//digital_tube as before
	} else if(show_state == WAIT){
		LatchControl(4, 0xff);
		digital_tube[7] = 0;
		digital_tube[6] = OFF;
		digital_tube[5] = OFF;
		digital_tube[4] = OFF;
		digital_tube[3] = O;
		digital_tube[2] = P;
		digital_tube[1] = E;
		digital_tube[0] = N;
	} else if(show_state == CHANGE_PASSWORD){
		LatchControl(4, 0x7f);
		digital_tube[7] = aaa;
		digital_tube[6] = C;
		for(i = 0; i <6; i++){
			digital_tube[i] = in_password[i];
		}
	} else {
		LatchControl(4, 0xff);
		for(i = 0; i < 8; i++){
			digital_tube[i] = 8;
		}
	}
	
	if(show_state == WAIT){
		LatchControl(5, 0);
	} else {
		LatchControl(5, 0x1f);
	}
}