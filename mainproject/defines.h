#define SETBIT(WORD,BITPOS) WORD|=(1<<BITPOS)

#define CLRBIT(WORD,BITPOS) WORD&=(~(1<<BITPOS))

#define SSETBIT(WORD,BITPOS) WORD=1<<BITPOS

#define SCLRBIT(WORD,BITPOS) WORD=1<<BITPOS

#define READBIT(WORD,BITPOS) ((WORD>>BITPOS)&1)

#define CPLBIT(WORD,BITPOS) WORD=WORD^(1<<BITPOS)

#define READWRITEBIT(WORD,DBIT,SBIT) WORD=WORD&(~(1<<DBIT))|(((WORD>>SBIT)&1)<<DBIT)

#define WRITEBIT(WORD,BITPOS,BIT) WORD=(WORD&(~(1<<BITPOS))|(BIT<<BITPOS)) 

#define READWRITEBIT2(DWORD,DBIT,SWORD,SBIT) DWORD=(DWORD&(~(1<<DBIT))|((SWORD>>SBIT)&1)<<DBIT)

#define WRITEBYTE(WORD,STARTPOS,BYTE) WORD=(WORD&(~(255<<STARTPOS))|(BYTE<<STARTPOS))

#define READNIBBLE(WORD,STARTPOS) (WORD>>STARTPOS)&15

#define WRITENIBBLE(WORD,STARTPOS,NIBBLE) WORD=(WORD&(~(15<<STARTPOS)))|(NIBBLE<<STARTPOS)

#define CFGPIN(WORD,PIN,FUNC) WORD=(PIN<16)?(((WORD&~(3<<(PIN*2))))|(FUNC<<(PIN*2))):(((WORD&~(3<<(PIN-16)*2)))|(FUNC<<((PIN-16)*2)))
