#include <reg52.h>

#define lcd_data P0

sbit rs=P2^7;
sbit rw=P2^6;
sbit en=P2^5;

void Uart_Init();
void delay(unsigned int itime);
void send_str (unsigned char *str);
void send_char (unsigned char chr);

sbit Relay1 = P2^0;
sbit Relay2 = P2^1;

char temp;
char incoming_com =0;

void lcd_init();
void cmd(unsigned char a);
void dat(unsigned char b);
void show(unsigned char *s);
void lcd_delay();

void lcd_init()
{
    cmd(0x38);
    cmd(0x0e);
    cmd(0x01);
    cmd(0x06);
    cmd(0x0c);
    cmd(0x80);
}

void cmd(unsigned char a)
{
    lcd_data=a;
    rs=0;
    rw=0;
    en=1;
    lcd_delay();
    en=0;
}

void dat(unsigned char b)
{
    lcd_data=b;
    rs=1;
    rw=0;
    en=1;
    lcd_delay();
    en=0;
}

void show(unsigned char *s)
{
    while(*s) {
        dat(*s++);
    }
}

void lcd_delay()
{
    unsigned int lcd_delay;
    for(lcd_delay=0;lcd_delay<=6000;lcd_delay++);
}

void delay(unsigned int itime)
{
 unsigned int i,j;
 for(i=0;i<itime;i++)
   for(j=0;j<120;j++);
}

void Uart_Init()
{
 SCON = 0x50; // SCON: mode 1, 8-bit UART, enable receive 
 TMOD |= 0x20; // TMOD: timer 1, mode 2, 8-bit 
 TH1 = 0xFD; // TH1: for 9600 baud
 TR1 = 1; //TR1: timer 1 run 
}

void send_str (unsigned char *str)
{
    while(*str)
       {
       send_char(*str++);
       }
  //send_char(0x0D);
}


void send_char (unsigned char chr)
{
 SBUF = chr;
 while (TI==0); //Wait until the character is completely sent 
 TI=0; //Reset the Transmit Interrupt flag 
}

void main()
{
	Uart_Init();
	P2=0x00;
	//int j;
    lcd_init();

	cmd(0x80);
    show("HOME AUTOMATION");
    cmd(0xc0);
    show("USING 8051");
	
	
	while(1)
		
	{	while(!RI);
		incoming_com=SBUF;
		temp = incoming_com;
		RI=0;
		
		if(temp =='A')
		{
			Relay1=1;
			send_str("FAN ON");
			cmd(0x80);
        	show("        FAN      ");
        	cmd(0xc0);
        	show("        ON  ");

		}
		else if(temp =='B')
		{
			Relay1=0;
			send_str("FAN OFF");
			cmd(0x80);
        	show("        FAN       ");
        	cmd(0xc0);
        	show("        OFF   ");


		}
		else if(temp =='C')
		{
			Relay2=1;
			send_str("BULB ON");
			cmd(0x80);
        	show("       BULB       ");
        	cmd(0xc0);
        	show("       ON   ");

		}
		else if(temp =='D')
		{
			Relay2=0;
			send_str("BULB OFF");
			cmd(0x80);
        	show("      BULB      ");
        	cmd(0xc0);
        	show("      OFF      ");


		}
		
		temp=0;
		
	}
}


