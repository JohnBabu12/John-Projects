#include<LPC214x.h>


#include "uart0.h"

#include "delay.h"


#define FOSC 12000000

#define CCLK 5*FOSC

#define PCLK CCLK/4

#define PREINT_VAL ((PCLK/32768)-1)

#define PREFRAC_VAL (PCLK - ((PREINT_VAL + 1) * 32768))


void rtc_init(void); 
void rtc_init()

{
  CCR=0X01;
  SEC=20; //Initialized seconds

  MIN=23; //Initialized minutes

  HOUR=8;//Initialized hour
  MONTH=1;
  YEAR=26;
  DOM=3;
  CCR=0X00;

}
