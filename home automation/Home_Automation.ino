/*************************************************************
  Title         :   Home automation using blynk
  Description   :   To control light's brigntness with brightness,monitor temperature , monitor water level in the tank through blynk app
  Pheripherals  :   Arduino UNO , Temperature system, LED, LDR module, Serial Tank, Blynk cloud, Blynk App.
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPL6uiTvxHN"
#define BLYNK_DEVICE_NAME "HOME AUTOMATION"
#define BLYNK_AUTH_TOKEN "116Om37nTV0F1dTiiX1jTJt2BqFgzZzH"

/* Comment this out to disable prints
  #define BLYNK_PRINT Serial*/

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "main.h"
#include "temperature_system.h"
#include "ldr.h"
#include "serial_tank.h"

char auth[] = BLYNK_AUTH_TOKEN;
bool heaterval, coolerval, inletval, outletval;
unsigned int tankvol;

BlynkTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

/*This function is called every time the Virtual Pin 0 state changes
  To turn ON and OFF cooler based virtual PIN value*/
BLYNK_WRITE(COOLER_V_PIN)
{
  coolerval = param.asInt();
  if (coolerval)
  {
    cooler_control(ON);
    lcd.setCursor(8, 0);
    lcd.print("CooLR_ON");

    heater_control(OFF);
    //To turnoff the heater widget button
    Blynk.virtualWrite(HEATER_V_PIN, 0);
  }
  else
  {
    cooler_control(OFF);
    lcd.setCursor(8, 0);
    lcd.print("CoLR_OFF");
  }
}
/*To turn ON and OFF heater based virtual PIN value*/
BLYNK_WRITE(HEATER_V_PIN )
{
  heaterval = param.asInt();
  if (heaterval)
  {
    heater_control(ON);
    lcd.setCursor(8, 0);
    lcd.print("HeatR_ON");

    cooler_control(OFF);
    //To turnoff the cooler widget button
    Blynk.virtualWrite(COOLER_V_PIN, 0);
  }
  else
  {
    heater_control(OFF);
    lcd.setCursor(8, 0);
    lcd.print("HetR_OFF");
  }
}

/* To display temperature and water volume as gauge on the Blynk App*/
void update_temperature_reading()
{
  /* You can send any value at any time.
     Please don't send more that 10 values per second.

  To Display Tank Temperature Change in the Gauge */
  Blynk.virtualWrite(TEMPERATURE_GAUGE, read_temperature());

/*To Display Tank Volume Change in the Gauge */
  Blynk.virtualWrite(WATER_VOL_GAUGE, volume());
}

/*To turn off the heater if the temperature raises above 35 deg C*/
void handle_temp(void)
{
  if ((read_temperature() > float(40)) && heaterval)
  {
    heaterval = 0;
    heater_control(OFF);
    lcd.setCursor(8, 0);
    lcd.print("HetR_OFF");

    //Send Notification to app through terminal
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Temperature is Greater than 40 Degree Celsius,");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Turning OFF the Heater");

    //To turnoff the heater widget button
    Blynk.virtualWrite(HEATER_V_PIN, 0);
  }
 
  if ((read_temperature() < float(28)) && coolerval)
  {
    coolerval = 0;
    cooler_control(OFF);
    lcd.setCursor(8, 0);
    lcd.print("CoLR_OFF");

    //Send Notification to app through terminal
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Temperature is Normal");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Turning OFF the COOLER");

    //To turnoff the heater widget button
    Blynk.virtualWrite(COOLER_V_PIN, 0);
  }

}

/*To turn ON and OFF inlet vale based virtual PIN value*/
BLYNK_WRITE(INLET_V_PIN)
{
  inletval = param.asInt();
  if (inletval)
  {
    enable_inlet();
    lcd.setCursor(7, 1);
    lcd.print(" INFlo_ON");
  }
  else
  {
    disable_inlet();
    lcd.setCursor(7, 1);
    lcd.print("INFlo_OFF");
  }
}

/*To turn ON and OFF outlet value based virtual switch value*/
BLYNK_WRITE(OUTLET_V_PIN)
{
  outletval = param.asInt();
  if (outletval)
  {
    enable_outlet();
    lcd.setCursor(7, 1);
    lcd.print(" OUTFl_ON");
  }
  else
  {
    disable_outlet();
    lcd.setCursor(7, 1);
    lcd.print("OUTFl_OFF");
  }
}

/*To control water volume above 1000ltrs*/
void handle_tank(void)
{
  if ((tankvol < 1000) && (inletval == 0))
  {
    enable_inlet();
    inletval = 1;
    lcd.setCursor(7, 1);
    lcd.print(" INFlo_ON");

    //Send Notification to app through terminal
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Water level is less than 1000L");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Water inflow Enabled");

    //To turnon the inlet widget button
    Blynk.virtualWrite(INLET_V_PIN, 1);
  }

  if ((tankvol == 3000) && (inletval == 1))
  {
    disable_inlet();
    inletval = 0;
    lcd.setCursor(7, 1);
    lcd.print("INFlo_OFF");

    //Send Notification to app through terminal
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Water level is 3000L");
    Blynk.virtualWrite(BLYNK_TERMINAL_V_PIN, "Water inflow Disabled");

    //To turnoff the inlet widget button
    Blynk.virtualWrite(INLET_V_PIN, 0);
    
  }

}

void setup(void)
{
  Blynk.begin(auth);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.home();

  lcd.setCursor(0, 0);
  lcd.print("T=");

  lcd.setCursor(0, 1);
  lcd.print("V=");

  init_temperature_system();
  init_ldr();

  timer.setInterval(500, update_temperature_reading);
  init_serial_tank();
}

void loop(void)
{
  Blynk.run();

  timer.run();

  String temp;
  temp = String (read_temperature(), 1); //27.xyz --> 27.x
  lcd.setCursor(2, 0);
  lcd.print(temp);

  tankvol = volume();
  lcd.setCursor(2, 1);
  lcd.print(tankvol);

  brightness_control();
  handle_temp();
  handle_tank();
}
