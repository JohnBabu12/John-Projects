#include "types.h"
#include"defines.h"
#include"delay.h"
#include<lpc21xx.h>
#include"Lcd_macro.h"
#define LCD_DATA 8
#define LCD_RS 16
#define LCD_RW 18
#define LCD_EN 17
void Write_lcd(u8 dat)
{
WRITEBYTE(IOPIN0,LCD_DATA,dat);
SSETBIT(IOSET0,LCD_EN);
delay_us(1);
SCLRBIT(IOCLR0,LCD_EN);
delay_ms(2);
}
void Cmd_lcd(u8 cmd)
{
SCLRBIT(IOCLR0,LCD_RS);
Write_lcd(cmd);
}
void INIT_lcd(void)
{
WRITEBYTE(IODIR0,LCD_DATA,0XFF);
SETBIT(IODIR0,LCD_RS);
SETBIT(IODIR0,LCD_RW);
SETBIT(IODIR0,LCD_EN);
delay_ms(40);
Cmd_lcd(0x30);
Cmd_lcd(0x30);
Cmd_lcd(0x30);
Cmd_lcd(MODE_8BIT_2LINE);
Cmd_lcd(DISP_ON_CUR_BLINK);
Cmd_lcd(CLEAR_LCD);
Cmd_lcd(SHIFT_CUR_RIGHT);
}
void Char_lcd(u8 dat)
{
SSETBIT(IOSET0,LCD_RS);
Write_lcd(dat);
}
void Str_lcd(s8 *ptr)
{
while(*ptr)
Char_lcd(*ptr++);
}
void Itoa_lcd(u32 n)
{
u8 a[10];
s32 i=0;
if(n==0)
Char_lcd('0');
else
{
while(n)
{
a[i++]=(n%10)+48;
n=n/10;
}
for(--i;i>=0;i--)
{
Char_lcd(a[i]);
}
}
}
void Itoan_lcd(s32 n)
{
if(n<0)
{
Char_lcd('-');
n=-n;
}
Itoa_lcd(n);
}
void F32_lcd(f32 n,u8 ndp)
{
u32 i,j;
if(n<0.0)
{
Char_lcd('-');
n=-n;
}
i=n;
Itoa_lcd(i);
Char_lcd('.');
for(j=0;j<ndp;j++)
{
n=(n-i)*10;
i=n;
Char_lcd(i+48);
}
}
void Bin_lcd(u32 n,u8 nbd)
{
s32 i;
if(nbd<=16)
{
Cmd_lcd(GOTO_LINE1_POS0);
for(i=(nbd-1);i>=0;i--)
Char_lcd(((n>>i)&1)+48);
}
else
{
Cmd_lcd(GOTO_LINE1_POS0);
for(i=nbd-1;i>=nbd-16;i--)
{
Char_lcd(((n>>i)&1)+48);
}
Cmd_lcd(GOTO_LINE2_POS0);
for(;i>=0;i--)
Char_lcd(((n>>i)&1)+48);
}
}
void Hex_lcd(u32 n)
{
s8 a[8],t;
s32 i;
if(n==0)
Char_lcd('0');
else
{
while(n)
{
t=n%16;
t=(t<10)? t+48 : ((t-10)+'A');
a[i]=t;
n=n/16;
i++;
}
for(--i;i>=0;i--)
Char_lcd(a[i]);
}
}
void Build_CG_ram(u8* p,u32 nbytes)
{
	u32 j=0;
	Cmd_lcd(GOTO_CGRAM_START);
	for(j=0;j<nbytes;j++)
	{
		Char_lcd(p[j]);
	}
	Cmd_lcd(GOTO_LINE1_POS0);
}

