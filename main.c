#include <LPC21xx.h>

#include "lvd4bitheader.h"

#include "types.h"

#include "i2c.h"

#include "i2c_eeprom.h"

#include "delay.h"

#include <string.h>



#define SW1 16

#define SW2 17

#define LED (1 << 18)


int main()

{

	char str[] = "BIDIRECTIONAL VISITOR COUNTER";

    char id[] = "V24CE2S3";

    unsigned char incount = 0, outcount = 0, Total = 0;

    PINSEL0 = 0;

    IODIR0 |= LED;

    LCD_INIT();

	init_i2c();

    LCD_COMMAND(0xC4);

    LCD_STR(id);

    LCD_SCROLL(str);

    delay_milliseconds(200);

    LCD_COMMAND(0x01);

    LCD_COMMAND(0X80);

    LCD_STR("IN: ");

    LCD_COMMAND(0X88);

    LCD_STR("OUT: ");

	LCD_COMMAND(0XC4);

	LCD_STR("TOTAL: ");

	i2c_eeprom_write(0X50,0X00,'0');

	//incount = i2c_eeprom_read(0X50,0X00);

	//outcount = i2c_eeprom_read(0x50,0x01);

	LCD_COMMAND(0X83);

	LCD_DATA((incount / 10) + 48);

  	LCD_DATA((incount % 10) + 48);

	LCD_COMMAND(0X8C);

    LCD_DATA((outcount / 10) + 48);

    LCD_DATA((outcount % 10) + 48);

	while(1)

    {

        if(((IOPIN0 >> SW1) & 1) == 0)

        {

	    	delay_milliseconds(200);

            incount++;

            LCD_COMMAND(0X83);

            LCD_DATA((incount / 10) + 48);

            LCD_DATA((incount % 10) + 48);

        }

		if(((IOPIN0 >> SW2) & 1) == 0)

        {

	    	delay_milliseconds(200);

            outcount++;

            LCD_COMMAND(0X8C);

            LCD_DATA((outcount / 10) + 48);

            LCD_DATA((outcount % 10) + 48);

        }

        Total = incount - outcount;

		LCD_COMMAND(0XCA);

		LCD_DATA((Total / 10) + 48);

		LCD_DATA((Total % 10) + 48); 

		if(Total == 0)

        IOSET0 = LED;  

        else

        IOCLR0 = LED;

		if(Total == 0)

		{

			incount = 0;

			LCD_COMMAND(0X83);

            LCD_DATA((incount / 10) + 48);

            LCD_DATA((incount % 10) + 48);

			outcount = 0;

			LCD_COMMAND(0X8C);

            LCD_DATA((outcount / 10) + 48);

            LCD_DATA((outcount % 10) + 48);

		}

		

		i2c_eeprom_write(0X50,0X00,incount);

		i2c_eeprom_write(0x50,0x01,outcount);

	}

}


	


