#include<lpc21xx.h>

#include"types.h"

#include"defines.h"

#include"delay.h"
#include"keypad.h"
#define ROW0 16

#define ROW1 17

#define ROW2 18

#define ROW3 19

#define COL0 20

#define COL1 21

#define COL2 22

#define COL3 23


extern u8 kpmlut[4][4];

void Init_kpm(void)

{

WRITENIBBLE(IODIR1,ROW0,15);

}

u8 ColScan(void)

{

u32 t;

t=READNIBBLE(IOPIN1,COL0);

return t<15?0:1;

}

u8 RowCheck(void)

{

u8 rno;

for(rno=0;rno<4;rno++)

{

WRITENIBBLE(IOPIN1,ROW0,~(1<<rno));

if(!ColScan())

{

break;

}

}

WRITENIBBLE(IOPIN1,ROW0,0X0);

return rno;


}

u8 ColCheck(void)

{

u32 colno;

for(colno=0;colno<4;colno++)

{

if(READBIT(IOPIN1,COL0+colno)==0)

break;

}

return colno;

}

u8 keyscan(void)

{

u8 rno,colno,keyv;

while(ColScan());

rno=RowCheck();

colno=ColCheck();

keyv=kpmlut[rno][colno];

while(ColScan()==0);
delay_ms(500);

return keyv;

}

