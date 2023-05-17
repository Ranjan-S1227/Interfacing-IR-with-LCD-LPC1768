#include <lpc17xx.h>
#include <stdio.h>

#define RS_CTRL 0x08000000 //P0.27
#define EN_CTRL 0x10000000 //P0.28
#define DT_CTRL 0x07800000 //P0.23 to P0.26 data lines
#define LED_PINS 0x00000FF0 // P.04 to P0.11

unsigned long int temp1=0, temp2=0,i,j ;
unsigned char flag1 =0, flag2 =0;
unsigned char msg[] = {" OBJECT DETECTED!    "};
unsigned char msg2[] = {" NO OBJ DETECTED!"};

void lcd_write(void);
void port_write(void);
void delay_lcd(unsigned int);
unsigned long int init_command[] = {0x30,0x30,0x30,0x20,0x28,0x0c,0x06,0x01,0x80};

int main(void)
{
SystemInit();
SystemCoreClockUpdate();
LPC_GPIO0->FIODIR = DT_CTRL | RS_CTRL | EN_CTRL | LED_PINS ; //Config output
flag1 =0;//Command
for (i=0; i<9;i++)
{
temp1 = init_command[i];
lcd_write(); //send Init commands to LCD
}
flag1 =1;//Data
i =0;



LPC_GPIO1->FIODIR=0xFF7FFFFF; //Set P1.23 as input rest all as output


while(1){
	temp1=0x80;
	flag1=0;
	lcd_write();
	flag1=1;
	
LPC_GPIO1->FIOMASK=0xFF7FFFFF;
	
if(LPC_GPIO1->FIOPIN==0x00000000)
{
	
	i=0;
	while (msg[i++] != '!')
{
temp1 = msg[i];
	
lcd_write();//Send data bytes
} 
//LPC_GPIO0->FIOSET = LED_PINS;
}

else{
	i=0;
	//temp1=0x01;
	//flag1=0;
	//lcd_write();
	//flag1=1;

while (msg2[i++] != '!')
{
temp1 = msg2[i];
lcd_write();//Send data bytes
}
//LPC_GPIO0->FIOCLR =LED_PINS;

}
}

}

void lcd_write(void)
{
flag2 = (flag1 == 1) ? 0 :((temp1 == 0x30) || (temp1 == 0x20)) ? 1 : 0;//If command is 0x30 (Working in 8-bit mode initially), send ‘3’ on D7-D4(D3-D0 already grounded)
temp2 = temp1 & 0xf0;//
temp2 = temp2 << 19;//data lines from 23 to 26. Shift left (26-8+1) times so that higher digit is sent on P0.26 to P0.23
port_write(); // Output the higher digit on P0.26-P0.23
if (!flag2) // Other than command 0x30, send the lower 4-bt also
{
temp2 = temp1 & 0x0f; //26-4+1
temp2 = temp2 << 23;
port_write(); // Output the lower digit on P0.26-P0.23
}
}

void port_write(void)
{
LPC_GPIO0->FIOPIN = temp2;
if (flag1 == 0)
LPC_GPIO0->FIOCLR = RS_CTRL; // Select command register
else
LPC_GPIO0->FIOSET = RS_CTRL; //Select data register
LPC_GPIO0->FIOSET = EN_CTRL; //Apply -ve edge on Enable
delay_lcd(25);
LPC_GPIO0->FIOCLR = EN_CTRL;
delay_lcd(500000);
}

void delay_lcd(unsigned int r1)
{
unsigned int r;
for(r=0;r<r1;r++);
return;
}
