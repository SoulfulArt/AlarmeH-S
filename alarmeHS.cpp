#include "alarmeHS.h"
#include "Arduino.h"

//Modules setup
    LiquidCrystal LCD(LCDRESET, LCDENABLE, LCDD4, LCDD5, LCDD6, LCDD7);
    virtuabotixRTC RTCCLOCK(RTCCLK, RTCDATA, RTCRESET);

alarmeHS::alarmeHS(int Alarms[], uint8_t numberAlarms){

    //Hardware Setup
    int i;

    LCD.begin(LCDCHAR, LCDLINES);
    pinMode(RELAYCONTROL, OUTPUT);
    digitalWrite(RELAYCONTROL, LOW);
    this->numberAlarms = numberAlarms;
        
    for (i=0;i<numberAlarms;i++){
            
        this->Alarms[i] = Alarms[i];

    }

    this->insertionSort();

}

void alarmeHS::LCDShowCurrentTime(){

    RTCCLOCK.updateTime();        
    this->LCDprintTime(RTCCLOCK.hours, RTCCLOCK.minutes, RTCCLOCK.seconds, 0, 0);
        
}
    
void alarmeHS::setTime(int hourNowSet, int minuteNowSet, int secondNowSet){

    RTCCLOCK.setDS1302Time(secondNowSet, minuteNowSet, hourNowSet, 4, 8, 8, 2018);

}

void alarmeHS::checkAlarm(){

    uint8_t i = 0;
      
    for (i = 0; i < this->numberAlarms; i++)

        if (RTCCLOCK.hours*POWERTEN + RTCCLOCK.minutes == this->Alarms[i] 
        &&  RTCCLOCK.seconds == 0){

                soundAlarm();
                break;

            }

}

void alarmeHS::soundAlarm(){
        
    digitalWrite(RELAYCONTROL, HIGH);
    LCD.setCursor(0, 0);
    LCD.print("ALARME TOCANDO");    
    delay(ALARMD);
    digitalWrite(RELAYCONTROL, LOW);
    LCD.clear();

}

void alarmeHS::LCDprintTime(int horas, int minutos, int segundos, uint8_t col, uint8_t lin){

        
    LCD.setCursor(col, lin);
    
    if (horas < 10)
        LCD.print("0");
    
    LCD.print(horas);
    LCD.print(":");

    if (minutos < 10)
        LCD.print("0");

    LCD.print(minutos);
    LCD.print(":");

    if (segundos < 10)
        LCD.print("0");

    LCD.print(segundos);

}

void alarmeHS::LCDprintHourMin(uint8_t horas, uint8_t minutos, uint8_t col, uint8_t lin){

        
    LCD.setCursor(col, lin);
    
    if (horas < 10)
        LCD.print("0");
    
    LCD.print(horas);
    LCD.print(":");

    if (minutos < 10)
        LCD.print("0");

    LCD.print(minutos);

}

uint8_t alarmeHS::sendKey(){
     
    if (analogRead(0) < 50)   return btnRIGHT;  
    if (analogRead(0) < 195)  return btnUP; 
    if (analogRead(0) < 380)  return btnDOWN; 
    if (analogRead(0) < 555)  return btnLEFT; 
    if (analogRead(0) < 790)  return btnSELECT;
    else return btnNONE;   

}

void alarmeHS::delAlarm(){

    uint8_t countDel = 0;
    uint8_t countExit = 0;
    
    if (this->numberAlarms == 0){
    
        LCD.setCursor(0,0);
        LCD.print("SEM ALARMES!");   
        delay(SECOND);     
    
    }

    while (countExit < 20 && this->numberAlarms > 0){
        
        LCD.setCursor(0, 0);
        LCD.print("APAGAR HORARIO:");
        this->LCDprintTime(Alarms[countDel]/POWERTEN, Alarms[countDel] - (Alarms[countDel]/POWERTEN)*POWERTEN, 0, 0, 1);
    
        if ( this->sendKey() == btnUP ){

            countDel++;
            countExit = 0;

        }

        if ( this->sendKey() == btnDOWN ){

            if (countDel == 0)        
                countDel = this->numberAlarms-1;
    
            else
                countDel--;

            countExit = 0;

        }
    

        if ( this->sendKey() == btnSELECT ){

            this->popAlarm (countDel);
            this->insertionSort();
            LCD.setCursor(0, 0);
            LCD.print("HORARIO APAGADO!");
            delay(SECOND);
            break;

        }

        if (countDel == this->numberAlarms)
            countDel = 0;
        
        countExit++;
        delay(SECOND/4);

        if (sendKey() == btnLEFT)
            break;

    }
     
    LCD.clear();
    
}

void alarmeHS::keysetTime(uint8_t key){

    uint8_t hour = 0, min = 0, curpos = 1, countExit = 0;

    LCD.setCursor(0,1);
    LCDprintHourMin(hour, min, 0, 1);
    LCD.setCursor(curpos,1);
    LCD.cursor(); 

    while(countExit < 20){

        delay(SECOND/4);  

        if (sendKey() == btnLEFT){

            if (curpos == 1)
                curpos = 4;

            else
                curpos = 1;

            LCD.setCursor(curpos,1);
            countExit = 0;

        }

        if (sendKey() == btnUP){

            countExit = 0;

            if (curpos == 1){

                if (hour == 23)
                    hour = 0;
                
                else
                    hour++;

            LCDprintHourMin(hour, min, 0, 1);
            LCD.setCursor(curpos,1);

        }

            if (curpos == 4){

                if (min == 59)
                    min = 0;
                
                else
                    min++;

                LCDprintHourMin(hour, min, 0, 1);
                LCD.setCursor(curpos,1);

            }

        }

        if (sendKey() == btnDOWN){

            countExit = 0;

            if (curpos == 1){

                if (hour == 0)
                    hour = 23;
                
                else
                    hour--;

                  LCDprintHourMin(hour, min, 0, 1);
                  LCD.setCursor(curpos,1);

            }

            if (curpos == 4){

                if (min == 0)
                    min = 59;
                
                else
                    min--;

                LCDprintHourMin(hour, min, 0, 1);
                LCD.setCursor(curpos,1);

            }

        }


        if (sendKey() == btnRIGHT && key == btnSELECT){
        
            setTime(hour, min, RTCCLOCK.seconds);
            delay(SECOND/1.5);
            break;
        
        }

        if (sendKey() == btnSELECT && key == btnRIGHT){
    
            LCD.setCursor(0,0);
            LCD.clear();
            LCD.print("ALARME ADD!");
            delay(SECOND/4);       
            pushAlarm(hour, min);
            delay(SECOND/1.5);
            break;
        
        }

        if (sendKey() == btnNONE)
            countExit++;  

    }
    
    LCD.noCursor();
    LCD.clear();

}

void alarmeHS::keyFunction(){

    if (sendKey() == btnLEFT){

        LCD.setCursor(0,1);
        delAlarm();

    }

    if (sendKey() == btnDOWN){

        LCD.setCursor(0,1);
        keySoundAlarm();

    }


    if (sendKey() == btnSELECT){

        LCD.setCursor(0,0);    
        LCD.print("DEFINIR HORARIO");
        keysetTime(btnSELECT);

    }

    if (sendKey() == btnRIGHT){

        LCD.setCursor(0,0);    
        LCD.print("ADD HORARIO");
        keysetTime(btnRIGHT);

    }

}

void alarmeHS::keySoundAlarm(){
    
    uint8_t i = 0;
 
    while (i<=5){

        this->LCDShowCurrentTime();

        if (this->sendKey() == btnDOWN)

            i++;

        else break;

        if (i == 5){
        
            soundAlarm();
            break;
        }

        delay(SECOND);        
    
    }
    
}

void alarmeHS::popAlarm(uint8_t index){
    
    uint8_t i = index;
 
    for (; i < numberAlarms; i++){
        
        this-> Alarms[i] = this-> Alarms[i+1];

    }

    numberAlarms--;
    
}

void alarmeHS::pushAlarm(int hour, int min){
    
    uint8_t i, find = 0;
    
    for (i=0; i < this->numberAlarms; i++)
        if (this->Alarms[i] == hour*POWERTEN + min){

            find = 1;

        }
    
    if (find == 0){

        this->numberAlarms++;
        this->Alarms[numberAlarms-1] = hour*POWERTEN + min;
        insertionSort();

    }
          
}


/* Function to sort an array using insertion sort. Source:
https://www.geeksforgeeks.org/insertion-sort/
*/

void alarmeHS::insertionSort(){

   int i, key, j;

   for (i = 1; i < this->numberAlarms; i++)
   {

       key = this->Alarms[i];
       j = i-1;
 
       /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
       while (j >= 0 && this->Alarms[j] > key)
       {
           this->Alarms[j+1] = this->Alarms[j];
           j = j-1;
       }

       this->Alarms[j+1] = key;

   }

}
