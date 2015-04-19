/*
 * Copyright 2015 V-master
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 1000000UL

#define LED_0_ON PORTC|=0b00100000
#define LED_1_ON PORTC|=0b00010000
#define LED_2_ON PORTC|=0b00001000
#define LED_3_ON PORTC|=0b00000100

#define LED_0_OFF PORTC&=0b11011111
#define LED_1_OFF PORTC&=0b11101111
#define LED_2_OFF PORTC&=0b11110111
#define LED_3_OFF PORTC&=0b11111011

#define LAYER1 PORTB=0b00000100
#define LAYER2 PORTB=0b00000010

#define PORTC_MASK_SET 	DDRC=0b00111100
#define PORTB_MASK_SET	DDRB=0b00000110

volatile uint8_t pwm0=0, pwm1=0, pwm2=0, pwm3=0;

#define ILOSC 88
char seqg[ILOSC] = {
		0b00000000,
		0b01010101,
		0b10101010,
		0b11111111,
		0b11111111, //5
		0b10101010,
		0b01010101,
		0b00000000,
		0b01000000,
		0b10010001, //10
		0b11100110,
		0b10111011,
		0b01101110,
		0b00011001,
		0b00000100, //15
		0b00000000,
		0b00000000,
		0b01000000,
		0b10000000,
		0b11000000, //20
		0b10000000,
		0b01000000,
		0b00000000,
		0b00000000,
		0b00000000, //25
		0b00000000,
		0b00000100,
		0b00001000,
		0b00001100,
		0b00001000, //30
		0b00000100,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000, //35
		0b00010000,
		0b00100000,
		0b00110000,
		0b00100001,
		0b00010010, //40
		0b00000011,
		0b00000010,
		0b00000001,
		0b00000000,
		0b01010101, //45
		0b10101010,
		0b01010101,
		0b10101010,
		0b11111111,
		0b10101010, //50
		0b11111111,
		0b10101010,
		0b01010101,
		0b10101010,
		0b01010101, //55
		0b00000000,
		0b01010101,
		0b00000000,
		0b00000000,
		0b00000000, //60
		0b11000000,
		0b10000011,
		0b01000010,
		0b00000001,
		0b00000000, //65
		0b00110000,
		0b00101100,
		0b00011000,
		0b00000100,
		0b00000000, //70
		0b00000000,
		0b01000001,
		0b10000010,
		0b11000011,
		0b00111100, //75
		0b11000011,
		0b00111100,
		0b00101000,
		0b00010100,
		0b00000000, //80
		0b00000000,
		0b00000000,
		0b00000010,
		0b11000000,
		0b00000000, //85
		0b00000000,
		0b00000000,
		0b00000000
	};
char seqd[ILOSC] = {
		0b00000000,
		0b01010101,
		0b10101010,
		0b11111111,
		0b11111111, //5
		0b10101010,
		0b01010101,
		0b00000000,
		0b00000000,
		0b01000000, //10
		0b10010001,
		0b11100110,
		0b10111011,
		0b01101110,
		0b00011001, //15
		0b00000100,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000, //20
		0b00000100,
		0b00001000,
		0b00001100,
		0b00011000,
		0b00100100, //25
		0b00110000,
		0b00100000,
		0b00010000,
		0b00000000,
		0b01000000, //30
		0b10000000,
		0b11000000,
		0b10000001,
		0b01000010,
		0b00000011, //35
		0b00000010,
		0b00000001,
		0b00000000,
		0b00000000,
		0b00000000, //40
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b01010101, //45
		0b10101010,
		0b01010101,
		0b10101010,
		0b11111111,
		0b10101010, //50
		0b11111111,
		0b10101010,
		0b01010101,
		0b10101010,
		0b01010101, //55
		0b00000000,
		0b01010101,
		0b00000000,
		0b00000011,
		0b00110010, //60
		0b00100001,
		0b00010000,
		0b00001100,
		0b11001000,
		0b10000111, //65
		0b01000010,
		0b00000001,
		0b00110000,
		0b00100000,
		0b00010000, //70
		0b00000000,
		0b01000001,
		0b10000010,
		0b11000011,
		0b00111100, //75
		0b11000011,
		0b00111100,
		0b00101000,
		0b00010100,
		0b00000000, //80
		0b00000000,
		0b00010000,
		0b00000000,
		0b00000000,
		0b00000010, //85
		0b01000000,
		0b00000000,
		0b00000000
	};


int main(void)
{
	TCCR2 |= (1<<WGM21);   //tryb CTC
	TCCR2 |= (1<<CS20);	//wlaczony, brak dzielnika
	OCR2   = 50;			//dodatkowy podzial czestotliowsci
	TIMSK |= (1<<OCIE2);	//zezwolenie na przerwanie CompareMatch
	sei();					//wlaczenie przerwan
 
	PORTB_MASK_SET;
	PORTC_MASK_SET;			//ustawienie wyjsc
	
	while(1)
	{

	}
	return 0;
}

ISR(TIMER2_COMP_vect) // timer2 - 20000 razy na sek
{
	static uint8_t cnt;
	static uint8_t count, semicount, layer;
	uint8_t tmp0, tmp1, tmp2, tmp3;
	
	if(cnt>=pwm0) LED_0_OFF; else LED_0_ON;
	if(cnt>=pwm1) LED_1_OFF; else LED_1_ON;
	if(cnt>=pwm2) LED_2_OFF; else LED_2_ON;
	if(cnt>=pwm3) LED_3_OFF; else LED_3_ON;
	
	if(++cnt>100) 
	{					// 200 razy na sek
		cnt-=100;
		semicount++;
		layer = !layer;
		if(layer) 
		{				// 100 razy na sek
			LAYER1; //gora
			tmp0 = seqg[count]&0xC0;
			tmp1 = seqg[count]&0x30;
			tmp2 = seqg[count]&0x0C;
			tmp3 = seqg[count]&0x03;
		}
		else
		{				// 100 razy na sek
			LAYER2; //dol
			tmp0 = seqd[count]&0xC0;
			tmp1 = seqd[count]&0x30;
			tmp2 = seqd[count]&0x0C;
			tmp3 = seqd[count]&0x03;
		}
		switch(tmp0){
			case 0x00: pwm0=0; break;
			case 0xC0: pwm0=100; break;
			case 0x80: pwm0=50; break;
			case 0x40: pwm0=20; break;
		}
		switch(tmp1){
			case 0x00: pwm1=0; break;
			case 0x30: pwm1=100; break;
			case 0x20: pwm1=50; break;
			case 0x10: pwm1=20; break;
		}
		switch(tmp2){
			case 0x00: pwm2=0; break;
			case 0x0C: pwm2=100; break;
			case 0x08: pwm2=50; break;
			case 0x04: pwm2=20; break;
		}
		switch(tmp3){
			case 0x00: pwm3=0; break;
			case 0x03: pwm3=100; break;
			case 0x02: pwm3=50; break;
			case 0x01: pwm3=20; break;
		}
/*		if(tmp0==0xc0) pwm0=100;
		else if(tmp0==0x80) pwm0=50;
		else if(tmp0==0x40) pwm0=20;
		else pwm0=0;
		if(tmp1==0x30) pwm1=100;
		else if(tmp1==0x20) pwm1=50;
		else if(tmp1==0x10) pwm1=20;
		else pwm1=0;
		if(tmp2==0x0C) pwm2=100;
		else if(tmp2==0x08) pwm2=50;
		else if(tmp2==0x04) pwm2=20;
		else pwm2=0;
		if(tmp3==0x03) pwm3=100;
		else if(tmp3==0x02) pwm3=50;
		else if(tmp3==0x01) pwm3=20;
		else pwm3=0;*/
		
		if(semicount>10)	//20 razy na sek
		{
			if(++count>ILOSC) count=0;
			semicount=0;
		}
	}
}