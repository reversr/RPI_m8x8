////////////////////////////////////////////////////////////////
// m8x8.c
//
// https://github.com/reversr/RPI_m8x8.git
//
// Example program for bcm2835 library && Max7219 LED Matrix
// Printing some geometric figures and alphanumeric chars
//
// After installing bcm2835, you can build this with
// make
// or
// gcc -o m8x8 m8x8.c -lbcm2835
// execute via
// sudo ./m8x8
// if you run this without sudo/root rights you will have
// Segmentation fault
//
// Used materials:
// http://pro-diod.ru/electronica/max7219-max7221-drajver-dlya-svetodiodnoj-indikacii.html
// MAX7219 Init Explanation
// http://www.14core.com/how-to-wire-8x8-matrix-led-with-max7219-on-arduino/
// 8×8 Matrix LED Row/Col & Bitwise explained
//
// This is a modified version of m8x8.c from
// https://s3-ap-northeast-1.amazonaws.com/sain-amzn/20/20-011-232/20-011-232.zip
//
//				IMPORTANT:
// I AM NOT RESPONCIBLE FOR ANY DAMAGE TO YOUR RASPBERRY PI,
// PC OR WHATEVER ELSE, USE THIS CODE AT YOUR OWN RISK.
//
////////////////////////////////////////////////////////////////

/*
	     define from bcm2835.h
		 3.3V | | 5V
    RPI_V2_GPIO_P1_03 | | 5V
    RPI_V2_GPIO_P1_05 | | GND
       RPI_GPIO_P1_07 | | RPI_GPIO_P1_08
		  GND | | RPI_GPIO_P1_10
       RPI_GPIO_P1_11 | | RPI_GPIO_P1_12
    RPI_V2_GPIO_P1_13 | | GND
       RPI_GPIO_P1_15 | | RPI_GPIO_P1_16
		  VCC | | RPI_GPIO_P1_18
       RPI_GPIO_P1_19 | | GND
       RPI_GPIO_P1_21 | | RPI_GPIO_P1_22
       RPI_GPIO_P1_23 | | RPI_GPIO_P1_24
		  GND | | RPI_GPIO_P1_26

::if your raspberry Pi is version 1 or rev 1 or rev A
RPI_V2_GPIO_P1_03->RPI_GPIO_P1_03
RPI_V2_GPIO_P1_05->RPI_GPIO_P1_05
RPI_V2_GPIO_P1_13->RPI_GPIO_P1_13:
*/

#include <bcm2835.h>
#include <stdio.h>

//#define Max7219_pinCLK  RPI_GPIO_P1_11
#define Max7219_pinCS  RPI_GPIO_P1_24
//#define Max7219_pinDIN  RPI_V2_GPIO_P1_13

#define uchar unsigned char
#define uint unsigned int
#define AMOUNT_OF_CHARS 38
#define AMOUNT_OF_CHARS1 9
#define AMOUNT_OF_CHARS2 17
#define SMALL_DELAY 100
#define NORMAL_DELAY 500
#define LARGE_DELAY 1000

uchar alphanum_1[AMOUNT_OF_CHARS][8] = {
{0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00},//-
{0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x10},//+
{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//0
{0x10,0x30,0x50,0x10,0x10,0x10,0x10,0x7C},//1
{0x3E,0x02,0x02,0x3E,0x20,0x20,0x3E,0x00},//2
{0x00,0x7C,0x04,0x04,0x7C,0x04,0x04,0x7C},//3
{0x08,0x18,0x28,0x48,0xFE,0x08,0x08,0x08},//4
{0x3C,0x20,0x20,0x3C,0x04,0x04,0x3C,0x00},//5
{0x3C,0x20,0x20,0x3C,0x24,0x24,0x3C,0x00},//6
{0x3E,0x22,0x04,0x08,0x08,0x08,0x08,0x08},//7
{0x00,0x3E,0x22,0x22,0x3E,0x22,0x22,0x3E},//8
{0x3E,0x22,0x22,0x3E,0x02,0x02,0x02,0x3E},//9
{0x08,0x14,0x22,0x3E,0x22,0x22,0x22,0x22},//A
{0x3C,0x22,0x22,0x3E,0x22,0x22,0x3C,0x00},//B
{0x3C,0x40,0x40,0x40,0x40,0x40,0x3C,0x00},//C
{0x7C,0x42,0x42,0x42,0x42,0x42,0x7C,0x00},//D
{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x7C},//E
{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x40},//F
{0x3C,0x40,0x40,0x40,0x40,0x44,0x44,0x3C},//G
{0x44,0x44,0x44,0x7C,0x44,0x44,0x44,0x44},//H
{0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x7C},//I
{0x3C,0x08,0x08,0x08,0x08,0x08,0x48,0x30},//J
{0x00,0x24,0x28,0x30,0x20,0x30,0x28,0x24},//K
{0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7C},//L
{0x81,0xC3,0xA5,0x99,0x81,0x81,0x81,0x81},//M
{0x00,0x42,0x62,0x52,0x4A,0x46,0x42,0x00},//N
{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//O
{0x3C,0x22,0x22,0x22,0x3C,0x20,0x20,0x20},//P
{0x1C,0x22,0x22,0x22,0x22,0x26,0x22,0x1D},//Q
{0x3C,0x22,0x22,0x22,0x3C,0x24,0x22,0x21},//R
{0x00,0x1E,0x20,0x20,0x3E,0x02,0x02,0x3C},//S
{0x00,0x3E,0x08,0x08,0x08,0x08,0x08,0x08},//T
{0x42,0x42,0x42,0x42,0x42,0x42,0x22,0x1C},//U
{0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18},//V
{0x00,0x49,0x49,0x49,0x49,0x2A,0x1C,0x00},//W
{0x00,0x41,0x22,0x14,0x08,0x14,0x22,0x41},//X
{0x41,0x22,0x14,0x08,0x08,0x08,0x08,0x08},//Y
{0x00,0x7F,0x02,0x04,0x08,0x10,0x20,0x7F},//Z
};

uchar square_1[AMOUNT_OF_CHARS1][8] = {
{0xFF,0x81,0x81,0x81,0x81,0x81,0x81,0xFF},//Square
{0x00,0x7E,0x42,0x42,0x42,0x42,0x7E,0x00},//Square
{0x00,0x00,0x3C,0x24,0x24,0x3C,0x00,0x00},//Square
{0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00},//Square
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//Empty
{0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00},//Square
{0x00,0x00,0x3C,0x24,0x24,0x3C,0x00,0x00},//Square
{0x00,0x7E,0x42,0x42,0x42,0x42,0x7E,0x00},//Square
{0xFF,0x81,0x81,0x81,0x81,0x81,0x81,0xFF},//Square
};

uchar diagonal_1[AMOUNT_OF_CHARS2][8] = {
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80},//1
{0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0},//2
{0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0x60},//3
{0x00,0x00,0x00,0x00,0x80,0xC0,0x60,0x30},//4
{0x00,0x00,0x00,0x80,0xC0,0x60,0x30,0x18},//5
{0x00,0x00,0x80,0xC0,0x60,0x30,0x18,0x0C},//6
{0x00,0x80,0xC0,0x60,0x30,0x18,0x0C,0x06},//7
{0x80,0xC0,0x60,0x30,0x18,0x0C,0x06,0x03},//8
{0xC0,0x60,0x30,0x18,0x0C,0x06,0x03,0x01},//9
{0x60,0x30,0x18,0x0C,0x06,0x03,0x01,0x00},//10
{0x30,0x18,0x0C,0x06,0x03,0x01,0x00,0x00},//11
{0x18,0x0C,0x06,0x03,0x01,0x00,0x00,0x00},//12
{0x0C,0x06,0x03,0x01,0x00,0x00,0x00,0x00},//13
{0x06,0x03,0x01,0x00,0x00,0x00,0x00,0x00},//14
{0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00},//15
{0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//16
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//17
};

void Delay_xms(uint x)
{
	bcm2835_delay(x);
}

void Write_Max7219_byte(uchar DATA)
{
	bcm2835_gpio_write(Max7219_pinCS,LOW);
	bcm2835_spi_transfer(DATA);
}

void my_Write_Max7219(unsigned char address,unsigned char dat)
{
	bcm2835_gpio_write(Max7219_pinCS,LOW);
	Write_Max7219_byte(address);	  //
	Write_Max7219_byte(dat);	      //
	bcm2835_gpio_write(Max7219_pinCS,HIGH);
}

void Init_MAX7219(void)
{
	my_Write_Max7219(0x09,0x00);//decode: BCD | 0x00 - decode off, 0x01 - decode on
	my_Write_Max7219(0x0A,0x00);//Brightness: D0=D1=D2=D3=0=minimum, D0=D1=D2=D3=1=maximum [0x00 - min, 0x03 = middle, 0x07 = max]
	my_Write_Max7219(0x0B,0x07);//Amount of shown points, 0x07 - all 8 points
	my_Write_Max7219(0x0C,0x01);//Sleep mode: 0x00 = sleep, 0x01 = normal
	my_Write_Max7219(0x0D,0x00);//Unused
	my_Write_Max7219(0x0E,0x00);//Unused
	my_Write_Max7219(0x0F,0x00);//Debug : 0x00 = off, 0x01 = on
}

void Max7219_Clear()
{
	my_Write_Max7219(1,0x00);
	my_Write_Max7219(2,0x00);
	my_Write_Max7219(3,0x00);
	my_Write_Max7219(4,0x00);
	my_Write_Max7219(5,0x00);
	my_Write_Max7219(6,0x00);
	my_Write_Max7219(7,0x00);
	my_Write_Max7219(8,0x00);
}

void write_digits(int add1, int add2)
{
	uchar c1 = 0x00;
	uchar c2 = 0x00;
	uchar c3 = 0x00;
	uchar c4 = 0x00;
	uchar c5 = 0x00;

	if(add1 == -1)
	{
		//Nothing - empty symbol
	}
	else if(add1 == 0)//0
	{
		c1 += 0x70;
		c2 += 0x50;
		c3 += 0x50;
		c4 += 0x50;
		c5 += 0x70;
	}
	else if(add1 == 1)//1
	{
		c1 = 0x20;
		c2 = 0x20;
		c3 = 0x60;
		c4 = 0x20;
		c5 = 0x70;
	}
	else if(add1 == 2)//2
	{
		c1 += 0x70;
		c2 += 0x10;
		c3 += 0x70;
		c4 += 0x40;
		c5 += 0x70;
	}
	else if(add1 == 3)//3
	{
		c1 += 0x70;
		c2 += 0x10;
		c3 += 0x30;
		c4 += 0x10;
		c5 += 0x70;
	}
	else if(add1 == 4)//4
	{
		c1 += 0x50;
		c2 += 0x50;
		c3 += 0x70;
		c4 += 0x10;
		c5 += 0x10;
	}
	else if(add1 == 5)//5
	{
		c1 += 0x70;
		c2 += 0x40;
		c3 += 0x70;
		c4 += 0x10;
		c5 += 0x70;
	}
	else if(add1 == 6)//6
	{
		c1 += 0x70;
		c2 += 0x40;
		c3 += 0x70;
		c4 += 0x50;
		c5 += 0x70;
	}
	else if(add1 == 7)//7
	{
		c1 += 0x70;
		c2 += 0x10;
		c3 += 0x20;
		c4 += 0x20;
		c5 += 0x20;
	}
	else if(add1 == 8)//8
	{
		c1 += 0x70;
		c2 += 0x50;
		c3 += 0x70;
		c4 += 0x50;
		c5 += 0x70;
	}
	else if(add1 == 9)//9
	{
		c1 += 0x70;
		c2 += 0x50;
		c3 += 0x70;
		c4 += 0x10;
		c5 += 0x70;
	}


	if(add2 == -1)
	{
		//Nothing - empty symbol
	}
	else if(add2 == 0)//0
	{
		c1 += 0x07;
		c2 += 0x05;
		c3 += 0x05;
		c4 += 0x05;
		c5 += 0x07;
	}
	else if(add2 == 1)//1
	{
		c1 += 0x02;
		c2 += 0x02;
		c3 += 0x06;
		c4 += 0x02;
		c5 += 0x07;
	}
	else if(add2 == 2)//2
	{
		c1 += 0x07;
		c2 += 0x01;
		c3 += 0x07;
		c4 += 0x04;
		c5 += 0x07;
	}
	else if(add2 == 3)//3
	{
		c1 += 0x07;
		c2 += 0x01;
		c3 += 0x03;
		c4 += 0x01;
		c5 += 0x07;
	}
	else if(add2 == 4)//4
	{
		c1 += 0x05;
		c2 += 0x05;
		c3 += 0x07;
		c4 += 0x01;
		c5 += 0x01;
	}
	else if(add2 == 5)//5
	{
		c1 += 0x07;
		c2 += 0x04;
		c3 += 0x07;
		c4 += 0x01;
		c5 += 0x07;
	}
	else if(add2 == 6)//6
	{
		c1 += 0x07;
		c2 += 0x04;
		c3 += 0x07;
		c4 += 0x05;
		c5 += 0x07;
	}
	else if(add2 == 7)//7
	{
		c1 += 0x07;
		c2 += 0x01;
		c3 += 0x02;
		c4 += 0x02;
		c5 += 0x02;
	}
	else if(add2 == 8)//8
	{
		c1 += 0x07;
		c2 += 0x05;
		c3 += 0x07;
		c4 += 0x05;
		c5 += 0x07;
	}
	else if(add2 == 9)//9
	{
		c1 += 0x07;
		c2 += 0x05;
		c3 += 0x07;
		c4 += 0x01;
		c5 += 0x07;
	}

	my_Write_Max7219(2,c1);
	my_Write_Max7219(3,c2);
	my_Write_Max7219(4,c3);
	my_Write_Max7219(5,c4);
	my_Write_Max7219(6,c5);

	if(add1 == -1)
	{
		add1 = 0;
	}

	if(add2 == -1)
	{
		add2 = 0;
	}

	printf("%d%d\n", add1, add2);
}

void full_wave()
{
	uint c_delay = 150;//ms

	my_Write_Max7219(1,0xFF);
	Delay_xms(c_delay);
	my_Write_Max7219(2,0xFF);
	Delay_xms(c_delay);
	my_Write_Max7219(3,0xFF);
	Delay_xms(c_delay);
	my_Write_Max7219(4,0xFF);
	Delay_xms(c_delay);
	my_Write_Max7219(5,0xFF);
	Delay_xms(c_delay);
	my_Write_Max7219(6,0xFF);
	Delay_xms(c_delay);
	my_Write_Max7219(7,0xFF);
	Delay_xms(c_delay);
	my_Write_Max7219(8,0xFF);
	Delay_xms(c_delay);

	my_Write_Max7219(8,0x00);
	Delay_xms(c_delay);
	my_Write_Max7219(7,0x00);
	Delay_xms(c_delay);
	my_Write_Max7219(6,0x00);
	Delay_xms(c_delay);
	my_Write_Max7219(5,0x00);
	Delay_xms(c_delay);
	my_Write_Max7219(4,0x00);
	Delay_xms(c_delay);
	my_Write_Max7219(3,0x00);
	Delay_xms(c_delay);
	my_Write_Max7219(2,0x00);
	Delay_xms(c_delay);
	my_Write_Max7219(1,0x00);
	Delay_xms(c_delay);
}

void running_maze()
{
	uint c_delay = 80;//ms

	my_Write_Max7219(1,0x80);//128
	Delay_xms(c_delay);
	my_Write_Max7219(1,0x40);//64
	Delay_xms(c_delay);
	my_Write_Max7219(1,0x20);//32
	Delay_xms(c_delay);
	my_Write_Max7219(1,0x10);//16
	Delay_xms(c_delay);
	my_Write_Max7219(1,0x08);//8
	Delay_xms(c_delay);
	my_Write_Max7219(1,0x04);//4
	Delay_xms(c_delay);
	my_Write_Max7219(1,0x02);//2
	Delay_xms(c_delay);
	my_Write_Max7219(1,0x01);//1
	Delay_xms(c_delay);	 //Turn down
	my_Write_Max7219(2,0x01);//1
	my_Write_Max7219(1,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(3,0x01);//1
	my_Write_Max7219(2,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(4,0x01);//1
	my_Write_Max7219(3,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(5,0x01);//1
	my_Write_Max7219(4,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(6,0x01);//1
	my_Write_Max7219(5,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(7,0x01);//1
	my_Write_Max7219(6,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(8,0x01);//1
	my_Write_Max7219(7,0x00);//Clearing previous
	Delay_xms(c_delay);	 //Turn Left
	my_Write_Max7219(8,0x02);//2
	Delay_xms(c_delay);
	my_Write_Max7219(8,0x04);//4
	Delay_xms(c_delay);
	my_Write_Max7219(8,0x08);//8
	Delay_xms(c_delay);
	my_Write_Max7219(8,0x10);//16
	Delay_xms(c_delay);
	my_Write_Max7219(8,0x20);//32
	Delay_xms(c_delay);
	my_Write_Max7219(8,0x40);//64
	Delay_xms(c_delay);
	my_Write_Max7219(8,0x80);//128
	Delay_xms(c_delay);	 //Turn Up
	my_Write_Max7219(7,0x80);//128
	my_Write_Max7219(8,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(6,0x80);//128
	my_Write_Max7219(7,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(5,0x80);//128
	my_Write_Max7219(6,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(4,0x80);//128
	my_Write_Max7219(5,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(3,0x80);//128
	my_Write_Max7219(4,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(2,0x80);//128
	my_Write_Max7219(3,0x00);//Clearing previous
	Delay_xms(c_delay);	 //Turn right
	my_Write_Max7219(2,0x40);//64
	Delay_xms(c_delay);
	my_Write_Max7219(2,0x20);//32
	Delay_xms(c_delay);
	my_Write_Max7219(2,0x10);//16
	Delay_xms(c_delay);
	my_Write_Max7219(2,0x08);//8
	Delay_xms(c_delay);
	my_Write_Max7219(2,0x04);//4
	Delay_xms(c_delay);
	my_Write_Max7219(2,0x02);//2
	Delay_xms(c_delay);	 //Turn Down
	my_Write_Max7219(3,0x02);//2
	my_Write_Max7219(2,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(4,0x02);//2
	my_Write_Max7219(3,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(5,0x02);//2
	my_Write_Max7219(4,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(6,0x02);//2
	my_Write_Max7219(5,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(7,0x02);//2
	my_Write_Max7219(6,0x00);//Clearing previous
	Delay_xms(c_delay);	 //Turn left
	my_Write_Max7219(7,0x04);//4
	Delay_xms(c_delay);
	my_Write_Max7219(7,0x08);//8
	Delay_xms(c_delay);
	my_Write_Max7219(7,0x10);//16
	Delay_xms(c_delay);
	my_Write_Max7219(7,0x20);//32
	Delay_xms(c_delay);
	my_Write_Max7219(7,0x40);//64
	Delay_xms(c_delay);	 //Turn up
	my_Write_Max7219(6,0x40);//64
	my_Write_Max7219(7,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(5,0x40);//64
	my_Write_Max7219(6,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(4,0x40);//64
	my_Write_Max7219(5,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(3,0x40);//64
	my_Write_Max7219(4,0x00);//Clearing previous
	Delay_xms(c_delay);	 //Turn right
	my_Write_Max7219(3,0x20);//32
	Delay_xms(c_delay);
	my_Write_Max7219(3,0x10);//16
	Delay_xms(c_delay);
	my_Write_Max7219(3,0x08);//8
	Delay_xms(c_delay);
	my_Write_Max7219(3,0x04);//4
	Delay_xms(c_delay);	 //Turn down
	my_Write_Max7219(4,0x04);//4
	my_Write_Max7219(3,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(5,0x04);//4
	my_Write_Max7219(4,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(6,0x04);//4
	my_Write_Max7219(5,0x00);//Clearing previous
	Delay_xms(c_delay);	 //Turn left
	my_Write_Max7219(6,0x08);//8
	Delay_xms(c_delay);
	my_Write_Max7219(6,0x10);//16
	Delay_xms(c_delay);
	my_Write_Max7219(6,0x20);//32
	Delay_xms(c_delay);	 //Turn up
	my_Write_Max7219(5,0x20);//32
	my_Write_Max7219(6,0x00);//Clearing previous
	Delay_xms(c_delay);
	my_Write_Max7219(4,0x20);//32
	my_Write_Max7219(5,0x00);//Clearing previous
	Delay_xms(c_delay);	 //Turn right
	my_Write_Max7219(4,0x10);//16
	Delay_xms(c_delay);
	my_Write_Max7219(4,0x08);//8
	Delay_xms(c_delay);	 //Turn down
	my_Write_Max7219(5,0x08);//8
	my_Write_Max7219(4,0x00);//Clearing previous
	Delay_xms(c_delay);      //Turn left
	my_Write_Max7219(5,0x10);//16
	Delay_xms(c_delay * 2);//Finish
}

void cycle_random(uint amount_todo)
{
	uchar chrand[1][10] = {0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0xFF};//random values of 0 ... 255 bit
	uint amount_done = 0;

	for(amount_done = 0; amount_done < amount_todo; amount_done ++)
	{
		my_Write_Max7219(1, chrand[1][rand() % 10]);
		my_Write_Max7219(2, chrand[1][rand() % 10]);
		my_Write_Max7219(3, chrand[1][rand() % 10]);
		my_Write_Max7219(4, chrand[1][rand() % 10]);
		my_Write_Max7219(5, chrand[1][rand() % 10]);
		my_Write_Max7219(6, chrand[1][rand() % 10]);
		my_Write_Max7219(7, chrand[1][rand() % 10]);
		my_Write_Max7219(8, chrand[1][rand() % 10]);
		Delay_xms(NORMAL_DELAY);
	}
}

int main(void)
{
	uchar i,j;
	if (!bcm2835_init())
	{
		return 1;
	}

	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);	// SPI Bit order = 1 [MSB First]
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);			// SPI Data Mode: CPOL=0,CPHA=0;
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);	// Clock Divider=256=1.024us=976.5625kHz 
	bcm2835_gpio_fsel(Max7219_pinCS, BCM2835_GPIO_FSEL_OUTP);	// Mode for RPI_GPIO_P1_24(wiringPi_PORT=10)=0x01 (Output)

	Delay_xms(50);
	Init_MAX7219();

	Max7219_Clear();//Clear all of the past

	//Random points (amount)
	cycle_random(10);//10 times cycle
	Delay_xms(LARGE_DELAY);
	Max7219_Clear();

	//Running maze from 1 point
	running_maze();

	Delay_xms(LARGE_DELAY);
	Max7219_Clear();

	//diagonal wave
	for(j = 0;j < AMOUNT_OF_CHARS2; j++)
	{
		for(i = 1;i < 9;i++)
		{
			my_Write_Max7219(i, diagonal_1[j][i-1]);
		}
		Delay_xms(NORMAL_DELAY);
	}

	Delay_xms(LARGE_DELAY);
	Max7219_Clear();

	//counter from 00 to 99
	uint k = 0, l = 0;
	for(k = 0; k <= 9; k++)
	{
		for(l = 0; l <= 9; l++)
		{
			write_digits(k, l);
			Delay_xms(SMALL_DELAY);
		}
	}

	Delay_xms(LARGE_DELAY);
	Max7219_Clear();

	//Wave from top to bottom
	full_wave();
	Delay_xms(LARGE_DELAY);
	Max7219_Clear();

	//Growing square
	for(j = 0;j < AMOUNT_OF_CHARS1; j++)
	{
		for(i = 1;i < 9;i++)
		{
			 my_Write_Max7219(i, square_1[j][i-1]);
		}
		Delay_xms(NORMAL_DELAY);
	}

	Delay_xms(LARGE_DELAY);
	Max7219_Clear();
	Delay_xms(LARGE_DELAY);

	//Alphanumeric chars
	for(j = 0;j < AMOUNT_OF_CHARS; j++)
	{
		for(i = 1;i < 9;i++)
		{
			my_Write_Max7219(i,alphanum_1[j][i-1]);
		}
		Delay_xms(NORMAL_DELAY);
	}

	Max7219_Clear();
	bcm2835_spi_end();
	bcm2835_close();
	printf("Program finished\n");
	return 0;
}
