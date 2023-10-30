#include "ldr.h"
#include "Arduino.h"
#include "main.h"

void init_ldr(void)
{
   pinMode(GARDEN_LIGHT, OUTPUT);
}
void brightness_control(void)
{
  unsigned int inputval = 0;
  inputval = analogRead(LDR_SENSOR);
  inputval = inputval/4;
  inputval = 255-inputval;
  analogWrite(GARDEN_LIGHT,inputval);
  delay(100);
}
