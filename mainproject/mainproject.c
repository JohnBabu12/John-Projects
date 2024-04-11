#include<lpc21xx.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"i2c_defines.h"
#include"delay.h"
#include"keypad.h"
#include"lcd.h"
#include"uart0.h"
#include"i2c.h"
#include"i2c_eeprom.h"
#include"types.h"
#include"Lcd_macro.h"
#include"defines.h"
void UART0_Str(u8 *);  
void funpancard(void);
void funatmcard	(void);
void fundrivinglicense(void);
void funvotingcard(void);
void rtc_init(void);
void InitUART0(void); 
extern s8 buff[11];
extern u8 i,r_flag;
const u8 prolut[]={0X0E,0X11,0X11,0X1F,0X11,0X11,0X0A,0X04,0X1F,0X1F,0X1F,0X1F,0X10,0X10,0X10,0X10,0X00,0X04,0X02,0X1F,0X02,0X04,0X00,0X00,0X0F,0X0F,0X10,0X10,0X0F,0X0F,0X00};
u16 n;
u32 a;
u32 flagreg=0;
u8 n1,n2;
u16 z;
u8 x,y;
u8 kpmlut[4][4]={{'1','2','3','!'},{'4','5','6','@'},{'7','8','9','#'},{'~','0','$','&'}}; 
const u16 date=1;
const u16 month=1;
const u16 year=2026;
u8 ptr[20],*p;
u8 menu1(void);
u8* Read_num1(void)
{
  u8 k=0;
	static u8 str[10];
	Cmd_lcd(0xc0);
	do
	{
		str[k]=keyscan();
		if(str[k]=='&')
		{
			k--;
			str[k]=0;
			Cmd_lcd(0x01);
			Str_lcd("ENTER THE AMOUNT");
			Cmd_lcd(GOTO_LINE2_POS0);
			Str_lcd(str);
			k--;
		}
		else
		Char_lcd(str[k]);
	}while(isdigit(str[k++]));
	str[--k]='\0';
	//Str_lcd(str);
	return str;
}
u8* Read_num(void)
{
	u16 i;
	u8 k=0;
	static u8 str[10];
	Cmd_lcd(0xc0);
	do
	{
		str[k]=keyscan();
		if(str[k]=='&')
		{
			k--;
			str[k]=0;
			Cmd_lcd(0x01);
			Str_lcd("ENTER THE PASSWORD");
			Cmd_lcd(GOTO_LINE2_POS0);
			for(i=0;i<k;i++)
				Char_lcd('*');
			//Str_lcd(str);
			k--;
		}
		else
		Char_lcd('*');
	}while(isdigit(str[k++]));
	str[--k]='\0';
	//Str_lcd(str);
	return str;
}		
void menu(void)
{
		char ch;
		Cmd_lcd(GOTO_LINE1_POS0);
		Str_lcd("1.PAN CARD");
		Cmd_lcd(GOTO_LINE2_POS0);
		Str_lcd("2.ATM CARD");
		Cmd_lcd(GOTO_LINE3_POS0);
		Str_lcd("3.VOTING CARD");
		Cmd_lcd(GOTO_LINE4_POS0);
		Str_lcd("4.Driving license");
		Cmd_lcd(GOTO_LINE1_POS0+13);
		Str_lcd("5.EXIT");
		ch=keyscan();
	    Cmd_lcd(CLEAR_LCD);
		switch(ch)
		{
			case '1': funpancard();
					  break;
			case '2': funatmcard();
					  break;
			case '3': funvotingcard();
					  break;
			case '4': fundrivinglicense();
					  break;
			case '5':exit(0);
			default :menu();
		} 
}
void balenq(void)
{	
	Cmd_lcd(CLEAR_LCD);
	n=((i2c_eeprom_read(0x50,0x45))|(i2c_eeprom_read(0x50,0x46)<<8));
	Cmd_lcd(GOTO_LINE1_POS0);
	Itoa_lcd(n);
	delay_s(5);
	menu();
}
void withdraw(void)
{
	u16 a;
	n=((i2c_eeprom_read(0x50,0x45)|(i2c_eeprom_read(0x50,0x46)<<8)));
	if(n>500)
	{
	win:
	Cmd_lcd(CLEAR_LCD);
	Cmd_lcd(GOTO_LINE1_POS0);
	Str_lcd("enter the amount");
	p=Read_num1();
	a=atoi((const char*)p);
	if((a%100)!=0)
	{
		Cmd_lcd(0x01);
		Cmd_lcd(0x80);
		Str_lcd("error");
		Cmd_lcd(0xc0);
		Str_lcd("enter multipl 100");
		delay_s(2);
		goto win;
	}

	else
	{
	//b=(i2c_eeprom_read(0x50,0x45)|(i2c_eeprom_read(0x50,0x46)<<8));
	if(a<n)
	{
		n=n-a;
		i2c_eeprom_write(0x50,0x45,n);
		i2c_eeprom_write(0x50,0x46,n>>8);
		Cmd_lcd(CLEAR_LCD);
		Cmd_lcd(GOTO_LINE1_POS0);
		Str_lcd("WITHDRAWN SUCCESS");
		delay_s(2);
		Cmd_lcd(CLEAR_LCD);
		menu();
	}
	else
	{
		Cmd_lcd(0x01);
		Cmd_lcd(0x80);
		Str_lcd("max amount");
		delay_s(2);
		menu();

	}
	}
	}
	else
	{
		Cmd_lcd(CLEAR_LCD);
		Cmd_lcd(GOTO_LINE1_POS0);
		Str_lcd("INSUFFICIENT AMOUNT");
		delay_s(2);
		Cmd_lcd(CLEAR_LCD);
		menu();
	}
	
}
void deposit(void)
{
	u16 a;
	bal:
	Cmd_lcd(CLEAR_LCD);
	Cmd_lcd(GOTO_LINE1_POS0);
	Str_lcd("ENTER AMOUNT");
	p=Read_num1();
	a=atoi((const char*)p);
	n=n+a;
	if(n>=65000)
	{
	Cmd_lcd(0x01);
	Cmd_lcd(0x80);
	Str_lcd("insufficient funds");
	delay_s(2);
	goto bal;
	}
	else
	{
	i2c_eeprom_write(0x50,0x45,n);
	i2c_eeprom_write(0x50,0x46,n>>8);
	Cmd_lcd(CLEAR_LCD);
	Str_lcd("SUCCESSFULL");
	delay_s(2);
	Cmd_lcd(CLEAR_LCD);
	menu();
	}
}
void funvotingcard(void)
{
	char ch;
	u16 n=96;
	if(flagreg==1)
	{
		n=69;
		i2c_eeprom_write(0x50,0x75,n);
		i2c_eeprom_write(0x50,0x76,n>>8);
		Cmd_lcd(0x01);
		Cmd_lcd(0x80);
		Str_lcd("reset done");
		delay_s(2);
		Cmd_lcd(0x01);
		menu();
	}
	else
	{
	pass:
	Cmd_lcd(CLEAR_LCD);
	Cmd_lcd(GOTO_LINE1_POS0);
	Str_lcd("ENTER THE PASSWORD");
	p=Read_num();
	a=atoi((const char*)p);
	z=(i2c_eeprom_read(0x50,0x55)|(i2c_eeprom_read(0x50,0x56)<<8));
	if(a==z)
	{
	x=((i2c_eeprom_read(0x50,0x75))|(i2c_eeprom_read(0x50,0x76)<<8));
	if(x==69)
	{
		Cmd_lcd(GOTO_LINE1_POS0);
		Str_lcd("NOT VOTED");
		Cmd_lcd(GOTO_LINE2_POS0);
		Str_lcd("CAST YOUR VOTE");
		delay_s(1);
		Cmd_lcd(CLEAR_LCD);
		Cmd_lcd(GOTO_LINE1_POS0);
		Str_lcd("PARTY 1");
		Cmd_lcd(GOTO_LINE1_POS0+9);
		Char_lcd(0);
		Cmd_lcd(GOTO_LINE2_POS0);
		Str_lcd("PARTY 2");
		Cmd_lcd(GOTO_LINE2_POS0+9);
		Char_lcd(1);
		Cmd_lcd(GOTO_LINE3_POS0);
		Str_lcd("PARTY 3");
		Cmd_lcd(GOTO_LINE3_POS0+9);
		Char_lcd(2);
		Cmd_lcd(GOTO_LINE4_POS0);
		Str_lcd("PARTY 4");
		Cmd_lcd(GOTO_LINE4_POS0+9);
		Char_lcd(3);
		ch=keyscan();
		Cmd_lcd(CLEAR_LCD);
		switch(ch)
		{
			case '1':i2c_eeprom_write(0x50,0x75,n);
					 i2c_eeprom_write(0x50,0x76,n>>8);
					 break;
			case '2':i2c_eeprom_write(0x50,0x75,n);
					 i2c_eeprom_write(0x50,0x76,n>>8);
					 break;
			case '3':i2c_eeprom_write(0x50,0x75,n);
					 i2c_eeprom_write(0x50,0x76,n>>8);
					 break;
			case '4':i2c_eeprom_write(0x50,0x75,n);
					 i2c_eeprom_write(0x50,0x76,n>>8);
					 break;
		}
			Cmd_lcd(0x01);
			Cmd_lcd(0x80);
			Str_lcd("vote validated");
			delay_s(2);
			Cmd_lcd(0x01);
			menu();

	}
	else
	{
		Cmd_lcd(CLEAR_LCD);
		Cmd_lcd(GOTO_LINE1_POS0);
		Str_lcd("ALREADY VOTED");
		delay_s(2);
		Cmd_lcd(CLEAR_LCD);
		menu();
		
	}
	}
	else
	{
		Cmd_lcd(CLEAR_LCD);
		Cmd_lcd(GOTO_LINE1_POS0);
		Str_lcd("invalid password");
		delay_s(2);
		goto pass;
	}
		
}
}
void fundrivinglicense(void)
{
	
	Cmd_lcd(CLEAR_LCD);
	Cmd_lcd(GOTO_LINE1_POS0);
	Str_lcd("sai kiran reddy");
	Cmd_lcd(GOTO_LINE2_POS0);
	Str_lcd("TWO WHEELER");
	if(year>YEAR)
	{
		Cmd_lcd(GOTO_LINE3_POS0);
		Str_lcd("LICENSE VALID");
	}
	else if(YEAR==year)
	{
		if(month>MONTH)
		{
			Cmd_lcd(GOTO_LINE3_POS0);
			Str_lcd("LICENSE VALID");
		}	
		else if(month==MONTH)
		{
			if(date>=DOM)
			{
			Cmd_lcd(0X94);
			Str_lcd("VAILD LICENSE");		
			}
			else
			{
				Cmd_lcd(0X94);
				Str_lcd("INVAILD LICENSE");
			}
				
		}
	}
	else
	{
		Cmd_lcd(GOTO_LINE3_POS0);
		Str_lcd("LICENSE INVALID");
	}
	Cmd_lcd(GOTO_LINE4_POS0);
	Str_lcd("REDLABAZAR GUNTUR");
	delay_s(2);
	Cmd_lcd(CLEAR_LCD);
	menu();
}
void funpancard(void)
{
	pass:
	Cmd_lcd(CLEAR_LCD);
	Cmd_lcd(GOTO_LINE1_POS0);
	Str_lcd("ENTER THE PASSWORD");
	p=Read_num();
	a=atoi((const char*)p);
	z=(i2c_eeprom_read(0x50,0x55)|(i2c_eeprom_read(0x50,0x56)<<8));
	if(a==z)
	{
		Cmd_lcd(CLEAR_LCD);
		Cmd_lcd(GOTO_LINE1_POS0);
		Str_lcd("sai kiran");
		Cmd_lcd(GOTO_LINE2_POS0);
		Str_lcd("12/01/2002");
		Cmd_lcd(GOTO_LINE3_POS0);
		Str_lcd("AB00XE23KU");
		delay_s(5);
		Cmd_lcd(CLEAR_LCD);
		menu();
	}
	else
	{
		Cmd_lcd(CLEAR_LCD);
		Cmd_lcd(GOTO_LINE1_POS0);
		Str_lcd("invalid password");
		delay_s(2);
		goto pass;
	}
}
/*u8 menu1(void)
{
	u8 ch;
		Cmd_lcd(CLEAR_LCD);
		Cmd_lcd(GOTO_LINE1_POS0);
		Str_lcd("1.Bal enquiry");
		Cmd_lcd(GOTO_LINE2_POS0);
		Str_lcd("2.With drawn");
		Cmd_lcd(GOTO_LINE3_POS0);
		Str_lcd("3.Deposit");
		Cmd_lcd(GOTO_LINE4_POS0);
		Str_lcd("4.exit");
		ch=keyscan();
		Cmd_lcd(CLEAR_LCD);	
		return ch;
}*/
void funatmcard(void)
{
	char ch;
	bal:
	Cmd_lcd(CLEAR_LCD);
	Cmd_lcd(GOTO_LINE1_POS0);
	Str_lcd("ENTER THE PASSWORD");
	p=Read_num();
	a=atoi((const char*)p);
	z=(i2c_eeprom_read(0x50,0x55)|(i2c_eeprom_read(0x50,0x56)<<8));
	if(a==z)
	{  //ch=menu1();
		Cmd_lcd(CLEAR_LCD);
		Cmd_lcd(GOTO_LINE1_POS0);
		Str_lcd("1.Bal enquiry");
		Cmd_lcd(GOTO_LINE2_POS0);
		Str_lcd("2.With drawn");
		Cmd_lcd(GOTO_LINE3_POS0);
		Str_lcd("3.Deposit");
		Cmd_lcd(GOTO_LINE4_POS0);
		Str_lcd("4.exit");
		ch=keyscan();
		Cmd_lcd(CLEAR_LCD);
		switch(ch)
		{
		case '1': balenq();
				  break;
		case '2': withdraw();
				  break;
		case '3': deposit();
				  break;
		default: menu();
		}
	}
	else
	{
		Cmd_lcd(CLEAR_LCD);
		Cmd_lcd(GOTO_LINE1_POS0);
		Str_lcd("invalid password");
		delay_ms(1000);
		goto bal;
	}

}
main()
{
	char arr[3][15]={{"12533660"},{"00312472"},{"00307190"}};
	INIT_lcd();
	Init_kpm();
	init_i2c();
	rtc_init();
	InitUART0();
	Build_CG_ram((u8*)prolut,32);
/*	n=5500;
	i2c_eeprom_write(0x50,0x45,n);
	i2c_eeprom_write(0x50,0x46,n>>8);
	//VOTING
	n=69;
	i2c_eeprom_write(0x50,0x75,n);
	i2c_eeprom_write(0x50,0x76,n>>8);
	//password
	n=1234;
	i2c_eeprom_write(0x50,0x55,n);
	i2c_eeprom_write(0x50,0x56,n>>8);
	*/
									  


	Cmd_lcd(0x01);
	Cmd_lcd(GOTO_LINE1_POS0);
	Str_lcd("MULTI PURPOSE E-CARD SYSTEM");
	delay_s(3);
	Cmd_lcd(CLEAR_LCD);
	card:
	Cmd_lcd(GOTO_LINE1_POS0);
	Str_lcd("Waiting for the card");
	delay_s(3);
	i=0;
	r_flag=0;
	while(r_flag==0);
	if(strcmp((const char *)buff+1,arr[0])==0)
	{
		Cmd_lcd(0x01);
		Str_lcd("user card");
		delay_s(2);
		menu();
	}
	else if(strcmp((const char*)buff+1,arr[1])==0)
	{
		flagreg=1;
		Cmd_lcd(0x01);
		Str_lcd("officer card");
		delay_s(2);
		menu();
	}

	else
	{
		Cmd_lcd(CLEAR_LCD);
		Cmd_lcd(GOTO_LINE1_POS0);
		Str_lcd("Invalid card");
		delay_s(2);
		Cmd_lcd(CLEAR_LCD);
		goto card;
	}
while(1);
}

		
