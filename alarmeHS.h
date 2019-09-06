#ifndef ALARMEHS_H
#define ALARMEHS_H

#include <LiquidCrystal.h>
#include <virtuabotixRTC.h>

//pin definitions

#define LCDRESET 8
#define LCDENABLE 9
#define LCDD4 4
#define LCDD5 5
#define LCDD6 6
#define LCDD7 7
#define LCDCHAR 16
#define LCDLINES 2
#define RTCCLK 0
#define RTCDATA 1
#define RTCRESET 2
#define RELAYCONTROL 3

#define ALARMD 60000 //Em milisegundos

//Definitions for keyboad LCD shield integrated buttons
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

//constant definitions
#define SECOND 1000
#define MINUTE 60000
#define POWERTEN 100

class alarmeHS{

public:

    //Defining common variables of the class
    int Alarms[50];
    uint8_t numberAlarms;

    //Defining public methods
    alarmeHS(int Alarms[], uint8_t numberAlarms);
    void LCDShowCurrentTime();    
    void setTime(int hourNowSet, int minuteNowSet, int secondNowSet);
    void checkAlarm();
    void keyFunction();
    uint8_t sendKey();

private:
    void soundAlarm ();
    void delAlarm();
    void LCDprintTime(int horas, int minutos, int segundos, uint8_t col, uint8_t lin);
    void LCDprintHourMin(uint8_t horas, uint8_t minutos, uint8_t col, uint8_t lin);
    void pushAlarm(int hour, int min);
    void popAlarm(uint8_t index);
    void keySoundAlarm();
    void insertionSort();
    void keysetTime(uint8_t key);

};

#endif
