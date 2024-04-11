void delay_us(unsigned int dlyUS)

{

	dlyUS*=12;

	while(dlyUS--);

}

void delay_ms(unsigned int dlyMS)

{

	dlyMS*=12000;

	while(dlyMS--);

}
void delay_s(unsigned int dlyS)
{
	dlyS*=12000000;
	while(dlyS--);
}
