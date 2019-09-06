#include <alarmeHS.h>

int alarms[] = {0,  15,  150, 715, 900, 915, 930,  945, 1000, 1145, 1200, 1245, 1300, 1400, 1500, 1515, 1530, 1545, 1600, 1645, 1700, 1900, 1915, 2100, 2200};
 
alarmeHS Alarme(alarms, sizeof(alarms)/sizeof(*alarms));

void setup() {

  Alarme.setTime(5, 04, 00); //HH:MM:SS

}

void loop() {
  
  Alarme.keyFunction(); //Execute current key pressed function
  Alarme.LCDShowCurrentTime(); //Print the current time on the LCD
  Alarme.checkAlarm();
  delay(SECOND);
  Alarme.keyFunction(); //Execute current key pressed function

}
