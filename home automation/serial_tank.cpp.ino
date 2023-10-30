#include "serial_tank.h"
#include "Arduino.h"
#include "main.h"

unsigned int volumeval;
unsigned char valhigh, vallow;

void init_serial_tank(void) 
{
    Serial.begin(19200);
    Serial.write(0xFF); //sychronisation
    Serial.write(0xFF);
//    Serial.write(0xFF);   
}

unsigned int volume(void)
{
    Serial.write(VOLUME);
    
    while(!Serial.available());
    valhigh = Serial.read();
    
    while(!Serial.available());
    vallow = Serial.read();
    
    volumeval = (valhigh<<8)|vallow;
    return volumeval;
}
void enable_inlet(void)
{
   Serial.write(INLET_VALVE); 
   Serial.write(ENABLE);
}  
void disable_inlet(void)
{
   Serial.write(INLET_VALVE); 
   Serial.write(DISABLE);  
}  
void enable_outlet(void)
{  
   Serial.write(OUTLET_VALVE); 
   Serial.write(ENABLE);
}
void disable_outlet(void)
{  
   Serial.write(OUTLET_VALVE); 
   Serial.write(DISABLE);
}
