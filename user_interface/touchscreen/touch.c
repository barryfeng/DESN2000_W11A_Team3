#include "touch.h"
#include "lpc24xx.h"

#define CS_PIN            0x00100000        //P0.20

static unsigned char touch_read(unsigned char command);

void touch_init(void)
{
	//Implement this as you see fit
	//Remember to setup CS_TP as a GPIO output
	
	//Set GPIO and change value of CP and direction
	PINSEL1 &= ~(0x0 << 8);
	FIO0DIR |= (1 << 20);
	
	//Set up serial clock and MISO and MOSI
	PINSEL0 |= (0x3 << 29);	//Serial Clock
	PINSEL1 |= (0x3 << 2);	//MISO
	PINSEL1 |= (0x3 << 4);	//MOSI
	
	//Chip Select
	S0SPCR = 0x93C;
	S0SPCCR = 0x24;
}

void touch_read_xy(char *x, char* y, char *z1, char *z2)
{
	//Read X co-ordinate from the touch screen controller
	*x = touch_read(0xD8);
	//Read Y co-ordinate from the touch screen controller
	*y = touch_read(0x98);
	//Read Z1 co-ordinate from the touch screen controller
	*z1 = touch_read(0xB8);
	//Read Z2 co-ordinate from the touch screen controller
	*z2 = touch_read(0xC8);
}

static unsigned char touch_read(unsigned char command)
{
	unsigned short result;

	//Set CS_TP pin low to begin SPI transmission
	FIO0PIN &= ~(1 << 20);	//Set SSL
	
	//Transmit command byte on MOSI, ignore MISO (full read write cycle)
	for (S0SPDR = command; (S0SPSR >> 7) != 1;) {}
	
	//Transmit 0x00 on MOSI, read in requested result on MISO (another full read write cycle)
	for (S0SPDR = 0x00; (S0SPSR >> 7) != 1;){}
	result = S0SPDR;

	//Transmission complete, set CS_TP pin back to high
	FIO0PIN |= (1 << 20);
	
	//Return 8 bit result.
	return (unsigned char) result;
}	

