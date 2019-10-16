#include <stdio.h>
#include <stdlib.h>
#include "mraa.h"
#include <unistd.h>
#include <python2.7/Python.h>

#define addr 0x7C

mraa_gpio_context reset;
mraa_gpio_context sda;
mraa_gpio_context scl;

mraa_gpio_context scl2;
mraa_gpio_context sda2;
mraa_gpio_context pin0;
mraa_gpio_context pin1;
mraa_gpio_context pin2;
mraa_gpio_context pin4;
mraa_gpio_context pin7;
mraa_gpio_context pin8;
mraa_gpio_context pin12;
mraa_gpio_context pin13;
mraa_gpio_context pwm0;
mraa_gpio_context pwm1;
mraa_gpio_context pwm2;
mraa_gpio_context pwm3;

mraa_gpio_context button1;
mraa_gpio_context button2;
mraa_gpio_context button3;
mraa_gpio_context button4;
mraa_gpio_context led5;

void delay_time (unsigned int time_end)
{
	unsigned int index;
	for (index = 0; index < time_end; index++);
}

void i2c_start (void)
{
	mraa_gpio_write(sda, 1);
	mraa_gpio_write(scl, 1);
	delay_time (100);
	mraa_gpio_write(sda, 0);
	mraa_gpio_write(scl, 0);
}

void i2c_stop (void)
{
	mraa_gpio_write(scl, 0);
	mraa_gpio_write(sda, 0);
	mraa_gpio_write(scl, 1);
	delay_time (100);
	mraa_gpio_write(sda, 1);
}

void i2c_write (unsigned char output_data)
{
	unsigned char index;

	for(index = 0; index < 8; index++)  	// Send 8 bits to the I2C Bus
	{
        if (output_data & 0x80)
		{
			mraa_gpio_write(sda, 1);
		}
		else
		{
			mraa_gpio_write(sda, 0);
		}
		output_data <<= 1;            		// Shift the byte by one bit
		mraa_gpio_write(scl, 1);
		delay_time (100);
		mraa_gpio_write(scl, 0);
	}

	mraa_gpio_write(scl, 1);
	delay_time (100);
	mraa_gpio_write(scl, 0);
}

void LCD_Print (char* str)
{
	i2c_start ();
   	i2c_write (addr);
	i2c_write (0x40);
	for (; *str != '\0'; str ++)
	{
		i2c_write (*str);
	}
	i2c_stop();
}

void init_LCD (void)
{
	i2c_start ();
   	i2c_write (addr);
   	i2c_write (0x00);
   	i2c_write (0x38);
	i2c_stop();

	i2c_start ();
   	i2c_write (addr);
   	i2c_write (0x00);
   	i2c_write (0x39);
	i2c_stop();

	i2c_start ();
   	i2c_write (addr);
   	i2c_write (0x00);
   	i2c_write (0x14);
	i2c_stop();

	i2c_start ();
   	i2c_write (addr);
   	i2c_write (0x00);
   	i2c_write (0x79);
	i2c_stop();

	i2c_start ();
   	i2c_write (addr);
   	i2c_write (0x00);
   	i2c_write (0x50);
	i2c_stop();

	i2c_start ();
   	i2c_write (addr);
   	i2c_write (0x00);
   	i2c_write (0x6C);
	i2c_stop();

	delay_time (300);

	i2c_start ();
   	i2c_write (addr);
   	i2c_write (0x00);
   	i2c_write (0x0C);
	i2c_stop();

	i2c_start ();
   	i2c_write (addr);
   	i2c_write (0x00);
   	i2c_write (0x01);
	i2c_stop();
}

void clear_LCD (void)
{
	i2c_start ();
   	i2c_write (addr);
   	i2c_write (0x00);
   	i2c_write (0x01);
	i2c_stop();
}

void home_LCD (void)
{
	i2c_start ();
   	i2c_write (addr);
   	i2c_write (0x00);
   	i2c_write (0x02);
	i2c_stop();
}

int file_exist (char *filename)
{
  struct stat   buffer;
  return (stat (filename, &buffer) == 0);
}

int main(int argc, char** argv)
{
    mraa_init();
    char* buf[1024];

    int flagConnect = 0;
	reset = mraa_gpio_init(36);
	sda = mraa_gpio_init(7);
	scl = mraa_gpio_init(19);

	scl2 = mraa_gpio_init(6);
	sda2 = mraa_gpio_init(8);
	pin13 = mraa_gpio_init(37);
	pin12 = mraa_gpio_init(50);
	pin8 = mraa_gpio_init(38);
	pin7 = mraa_gpio_init(51);
	pin4 = mraa_gpio_init(25);
	pin2 = mraa_gpio_init(13);
	pin1 = mraa_gpio_init(35);
	pin0 = mraa_gpio_init(26);
	pwm0 = mraa_gpio_init(20);
	pwm1 = mraa_gpio_init(14);
	pwm2 = mraa_gpio_init(0);
	pwm3 = mraa_gpio_init(21);

	button1 = mraa_gpio_init(33);
	button2 = mraa_gpio_init(46);
	button3 = mraa_gpio_init(32);
	button4 = mraa_gpio_init(45);
	led5 = mraa_gpio_init(48);

	mraa_gpio_dir(reset, MRAA_GPIO_OUT);
	mraa_gpio_dir(sda, MRAA_GPIO_OUT);
	mraa_gpio_dir(scl, MRAA_GPIO_OUT);

	mraa_gpio_dir(scl2, MRAA_GPIO_OUT);
	mraa_gpio_dir(sda2, MRAA_GPIO_OUT);
	mraa_gpio_dir(pin13, MRAA_GPIO_OUT);
	mraa_gpio_dir(pin12, MRAA_GPIO_OUT);
	mraa_gpio_dir(pin8, MRAA_GPIO_OUT);
	mraa_gpio_dir(pin7, MRAA_GPIO_OUT);
	mraa_gpio_dir(pin4, MRAA_GPIO_OUT);
	mraa_gpio_dir(pin2, MRAA_GPIO_OUT);
	mraa_gpio_dir(pin1, MRAA_GPIO_OUT);
	mraa_gpio_dir(pin0, MRAA_GPIO_OUT);
	mraa_gpio_dir(pwm0, MRAA_GPIO_OUT);
	mraa_gpio_dir(pwm1, MRAA_GPIO_OUT);
	mraa_gpio_dir(pwm2, MRAA_GPIO_OUT);
	mraa_gpio_dir(pwm3, MRAA_GPIO_OUT);
	mraa_gpio_dir(button1, MRAA_GPIO_IN);
	mraa_gpio_dir(button2, MRAA_GPIO_IN);
	mraa_gpio_dir(button3, MRAA_GPIO_IN);
	mraa_gpio_dir(button4, MRAA_GPIO_IN);
	mraa_gpio_dir(led5, MRAA_GPIO_OUT);

	mraa_gpio_write(reset, 0);
	delay_time (500);
	mraa_gpio_write(reset, 1);
	delay_time (500);

	init_LCD ();

	//home_LCD();
	//clear_LCD();
	LCD_Print (argv[1]);
	usleep(0);

}
