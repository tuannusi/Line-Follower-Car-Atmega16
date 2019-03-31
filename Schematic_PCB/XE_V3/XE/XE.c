#include "function.h"

int check_crossline( void );
int check_rightline( void );
int check_leftline( void );

uint8_t  pattern;
uint16_t cnt1, pulse_v;
extern uint16_t pulse_ratio;

int main(void)
{
	INIT();
	sel_mode();
	///////////////////////////////////////////////////////////////////////
	while(1)
	{
		if (get_button(BTN1)) break;
		if (get_button(BTN0)) pulse_v=0;
		led7(pulse_v);
		sensor_cmp(0xff);
	}
	//////////////////////////////////////////////////////////////////////////
	
	while(1)
	{
		pattern = 1;
		
		while(1)
		{
			switch(pattern)
				{
					case 1:
					if( check_crossline() ) //Be 90
					{
						pattern = 21;
						break;
					}
					if( check_rightline() ) // Chuyen lan phai
					{
						pulse_v = 0;
						cnt1 = 0;
						pattern = 51;
						break;
					}
					if( check_leftline() )  // �Chuyen lan trai
					{
						pulse_v = 0;
						cnt1 = 0;
						pattern = 61;
						break;
					}
					led7(10);
					switch(sensor_cmp(0b01111110))
					{
						case 0b00011000:	// Chay thang
						handle( 0 );
						speed( 100 ,100 );
						break;
						
						//lech phai
						case 0b00011100:
						case 0b00001000:
						speed(100,90);
						handle(14);
						break;
						
						case 0b00001100:
						speed(100,80);
						handle(22);
						break;
						
						case 0b00001110:
						case 0b00000100:
						speed(100,70);
						handle(35);
						break;
						
						case 0b00000110:
						speed(100,60);
						handle(55);
						break;
						
						case 0b00000010:
						speed(100,40);
						handle(80);
						pattern=11;	//lech phai goc lon
						break;
						//////////////////////////////////////////////////////////////////////////
						
						//lech trai
						case 0b00111000:
						case 0b00010000:
						speed(90,100);
						handle(-14);
						break;
						
						case 0b00110000:
						speed(80,100);
						handle(-22);
						break;
						
						case 0b01110000:
						case 0b00100000:
						speed(70,100);
						handle(-36);
						break;
						
						case 0b01100000:
						speed(60,100);
						handle(-55);
						break;
						
						case 0b01000000:
						speed(40,100);
						handle(-80);
						pattern=12; //lech trai goc lon
						cnt1 = 0;
						pulse_v = 0;
						break;
						
						default:
						break;
					}
					break;
					
					case 11:
					led7(11);
					switch (sensor_cmp(0b11111111))
					{
						case 0b00000011:
						handle(60);
						speed(90,50);
						break;
						
						case 0b00000110:
						handle(55);
						speed(90,50);
						break;
						
						case 0b00001100:
						handle(40);
						speed(90,60);
						break;
						
						case 0b00000001:
						handle(90);
						speed(90, 30);
						break;
						
						case 0b10000001:
						handle(90);
						speed(90, 20);
						break;
						
						case 0b10000000:
						handle(100);
						speed(90, 10);
						break;
						
						case 0b11000000:
						handle(105);
						speed(90, 0);
						break;
						
						case 0b01100000:
						case 0b11100000:
						handle(120);
						speed(90, 0);
						break;
						
						case 0b00110000:
						case 0b11110000:
						handle(130);
						speed(90, -10);
						break;
						
						case 0b00010000:
						case 0b00001000:
						case 0b00000100:
						case 0b00011000:
						pattern = 1;
						pulse_v = 0;
						cnt1 = 0;
						led7(10);
						break;
						
						default:
						break;
					}
					break;
					
					case 12:
					led7(12);
					switch (sensor_cmp(0b11111111))
					{
						case 0b11000000:
						handle(-60);
						speed(50,90);
						break;
						
						case 0b01100000:
						handle(-55);
						speed(50,90);
						break;
						
						case 0b00110000:
						handle(-40);
						speed(60,90);
						break;
						
						case 0b10000000:
						handle(-90);
						speed(30, 90);
						break;
						
						case 0b10000001:
						handle(-90);
						speed(20, 90);
						break;
						
						case 0b00000001:
						handle(-85);
						speed(10, 90);
						break;
						
						case 0b00000011:
						handle(-105);
						speed(0, 90);
						break;
						
						case 0b00000110:
						case 0b00000111:
						handle(-120);
						speed(0, 90);
						break;
						
						case 0b00001100:
						case 0b00001111:
						handle(-130);
						speed(-10, 90);
						break;
						
						case 0b00001000:
						case 0b00010000:
						case 0b00100000:
						case 0b00011000:
						pattern = 1;
						pulse_v = 0;
						cnt1 = 0;
						led7(10);
						break;
						
						default:
						break;
					}
					break;

					default:
					pattern = 1;
					break;
				}
		}
		
	}
}

ISR(TIMER0_COMP_vect)
{
	cnt1++;
	cal_ratio();
	print();			//Quét LED7 đoạn
}
ISR(INT0_vect)
{
	pulse_v++;
	pulse_ratio++;
}
int check_crossline( void )
{
	int ret=0;
	if(( sensor_cmp(0b11111111)==0b11111111) || (sensor_cmp(0b01111110)==0b01111110))ret = 1;
	return ret;
}
int check_rightline( void )
{
	int ret=0;
	if( (sensor_cmp(0b00001111) == 0b00001111) || (sensor_cmp(0b00011111)==0b00011111))  ret = 1;
	return ret;
}
int check_leftline( void )
{
	int ret=0;
	if( (sensor_cmp(0b11110000) == 0b11110000) || (sensor_cmp(0b11111000)== 0b11111000)) ret = 1;
	return ret;
}