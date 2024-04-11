#include <LPC21xx.H>  /* LPC21xx definitions         */


#include <string.h>

#include"types.h"

#include"defines.h"

#include<lpc21xx.h>

#define FUNC2 1
#define DR_BIT 0

#define TEMT_BIT 6

#define DLAB_BIT 7

#define WORDLENGTH_8 3

#define FOSC 12000000

#define CCLK FOSC*5

#define PCLK CCLK/4

#define BAUD 9600

#define DIVISIOR (PCLK/(16*BAUD))





#define UART_INT_ENABLE 1




void InitUART0 (void); /* Initialize Serial Interface       */ 

  

void u0_txchar(u8 dat);


u8 u0_rxchar(void);  




char buff[11],dummy;


unsigned char i=0,ch,r_flag;




void UART0_isr(void) __irq


{


  if((U0IIR & 0x04)) //check if receive interrupt


  {


		ch = U0RBR;	/* Read to Clear Receive Interrupt */


		if(ch != 0x03)


			buff[i++] = ch; 


		else


		{


			buff[i] = '\0';


			i=0;


			r_flag = 1;


		}


  }


  else


  {


      dummy=U0IIR; //Read to Clear transmit interrupt


  


  }


   VICVectAddr = 0; /* dummy write */


}




void InitUART0 (void) /* Initialize Serial Interface       */ 


{  


            		


  PINSEL0 |= 0x00000005; /* Enable RxD0 and TxD0              */


  U0LCR = 0x83;         /* 8 bits, no Parity, 1 Stop bit     */


  U0DLL = 97;           /* 9600 Baud Rate @ CCLK/4 VPB Clock  */


  U0LCR = 0x03;         /* DLAB = 0  */


  


  #if UART_INT_ENABLE > 0




  VICIntSelect = 0x00000000; // IRQ


  VICVectAddr0 = (unsigned)UART0_isr;


  VICVectCntl0 = 0x20 | 6; /* UART0 Interrupt */


  VICIntEnable = 1 << 6;   /* Enable UART0 Interrupt */


 


 // U0IIR = 0xc0;


 // U0FCR = 0xc7;


  U0IER = 0x03;       /* Enable UART0 RX and THRE Interrupts */   


             


  #endif


						


}




void u0_txchar(u8 dat)

{

U0THR=dat;

while(READBIT(U0LSR,TEMT_BIT)==0);

}

u8 u0_rxchar(void)

{

	while(READBIT(U0LSR,DR_BIT)==0);

	return U0RBR;

}


void u0_txstr(u8 *str)

{

	while(*str)

	{

	u0_txchar(*str++);

	}

	//*str='\0';

}

u8 *u0_rxstr(void)

{

	static u8 buf[30];

	u32 i=0;

	do

	{

		buf[i]=u0_rxchar();

		if(buf[i]=='\n')

		i--;

	}while(buf[i++]!='\r');

	buf[--i]='\0';

	return buf;

}

void u0_txU32(u32 n)

{

u8 a[10];

s32 i=0;

	if(n==0)

	{	  u0_txchar(0+48);

	

	}

	else

	{

	while(n)

	{

			 a[i++]=(n%10)+48;

			 n=n/10;

	}

	}

	for(--i;i>=0;i--)

	{

	u0_txchar(a[i]);	

	}

	//u0_txchar('\r');

}

u32 u0_rxU32(void)

{

	u8 *p;

	p=u0_rxstr();

	return atoi((const char *)p);

}

void u0_txF32(f32 n,u32 nbd)

{

u32 i,j;

	if(n<0.0)

	{

	u0_txchar('-');

	n=-n;

	}

	i=n;

	u0_txU32(i);

	u0_txchar('.');

	for(j=0;j<nbd;j++)

	{

	n=(n-i)*10;

	i=n;

	u0_txchar(i+48);

	}

}

f32 u0_rxF32(void)

{

	u8 *p;

	p=u0_rxstr();

	return atof((const char *)p);

}

void u0_txHEX(u32 n)

{

	s32 t=0,i=0;

	u8 a[10];

	if(n==0)

	u0_txchar('0');

	else

	{

	while(n)

	{

		t=n%16;

		a[i++]=t<10?t+48:((t-10)+'A');

		n=n/16;

	}

	}

	for(--i;i>=0;i--)

	{

		u0_txchar(a[i]);

	}

}

u32 u0_rxHEX(void)

{

u8 *p;

u32 n=0,cnt=0;

s32 i;

p=u0_rxstr();

i=strlen((const char *)p)-1;

for(;i>=0;i--)

{

n=n+(p[i]-48)*pow(16,cnt);

cnt++;

}

return n;

}




							