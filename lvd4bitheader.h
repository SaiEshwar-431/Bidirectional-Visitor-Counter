#define LCD_D 0xff<<4
#define RS 1<<12
#define E 1<<13

void LCD_INIT(void);
void LCD_COMMAND(unsigned char cmd);
void LCD_DATA(unsigned char d);
void delay_milliseconds(unsigned int ms);
void LCD_STR(unsigned char *c);
void LCD_SCROLL(char *s);

void LCD_INIT(void) 
{
    PINSEL0 = 0;
    IODIR0 |= LCD_D | RS | E;

    LCD_COMMAND(0x01);
    LCD_COMMAND(0x02);
    LCD_COMMAND(0x0C);
    LCD_COMMAND(0x38);
    LCD_COMMAND(0x80);
}

void LCD_COMMAND(unsigned char cmd) 
{
    IOCLR0 = LCD_D;
    IOSET0 = cmd <<4;
    IOCLR0 = RS;
    IOSET0 = E;
    delay_milliseconds(2);
    IOCLR0 = E;
}

void LCD_DATA(unsigned char d) 
{
    IOCLR0 = LCD_D;
    IOSET0 = d << 4;
    IOSET0 = RS;
    IOSET0 = E;
    delay_milliseconds(2);
    IOCLR0 = E;
}

void delay_milliseconds(unsigned int ms) 
{
    T0PR = 15000 - 1;
    T0TCR = 0x01;
    while (T0TC < ms);
    T0TCR = 0x03;
    T0TCR = 0x00;
}

void LCD_STR(unsigned char *c) 
{
    while (*c)
    LCD_DATA(*c++);
}

void LCD_SCROLL(char *s) 

{
	int len = 0, i, j;
	while(s[len] != '\0') 
	len++;
	for (i= 0; i <= len; i++) 
	{
		LCD_COMMAND(0x80);
		for (j= 0; j < 16; j++) 
		{
			if ((i + j) < len)
			LCD_DATA(s[i + j]);
			else
			LCD_DATA(' ');
		}
		delay_milliseconds(300);
	}
}


