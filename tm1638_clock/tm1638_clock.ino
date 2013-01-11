#include <TM1638.h>

TM1638 tm (8, 9, 7);

unsigned long waitcheckTime=0;
unsigned long waitcheckButtons=0;

unsigned long totalSecond=0;
unsigned long gapSecond=0;
boolean dots=0;


void setup(){

}

void loop(){
  checkButtons();
  checkTime();
}


void checkTime(){
  if (millis() >= waitcheckTime) {
    dots = !dots;
    drawToModule();
    waitcheckTime += 1000;
  }
}

void checkButtons(){
  if (millis() >= waitcheckButtons) {
    tm.setLEDs(0);
    byte buttons=tm.getButtons();
    if(buttons>0){
      for (byte i=0;i<6;i++){
        if ((buttons & 1<<i) != 0) {
          buttonEvent(i);
          waitcheckButtons +=100;
          drawToModule();
        }
      }
    }
    waitcheckButtons +=100;
  }
}

void drawToModule(){
  unsigned long elapSecond = floor(millis() / 1000);
  totalSecond = gapSecond + elapSecond;
  tm.setDisplayToString(formatTime(totalSecond),(dots * 80));

}


void buttonEvent(byte inp){
  tm.setLED((inp % 2 ) + 1,inp);
  switch (inp) {
  case 0:
    if (hour(gapSecond) != 23)    {  
      gapSecond += 3600    ; 
    }
    else      {
      gapSecond -= 82800;
    }
    break;
  case 1:
    if (hour(gapSecond) != 0){
      gapSecond -= 3600;
    }
    else{
      gapSecond += 82800;
    }
    break;
  case 2:
    if (minute(gapSecond) != 59){
      gapSecond += 60;
    }
    else{
      gapSecond -= 3540;
    }
    break;
  case 3:
    if (minute(gapSecond) != 0){
      gapSecond -= 60;
    }
    else{
      gapSecond += 3540;
    }
    break;
  case 4:
    if (second(gapSecond) != 59){
      gapSecond += 1;
    }
    else{
      gapSecond -= 59;
    }
    break; 
  case 5:
    if (second(gapSecond) != 0){
      gapSecond -= 1;
    }
    else{
      gapSecond += 59;
    }
    break;
  case 6:
    break;
  case 7:
    break;
  }
}


String formatTime(unsigned long inp){
  return hourStr(inp) + minuteStr(inp) + secondStr(inp);
}

String formatNumber(String inp){
  if (inp.length()==1) {
    return "0" + inp;
  }
  else{
    return inp;
  }
}

String hourStr(unsigned long inp){
  return  formatNumber(String(hour(inp)));
}

String minuteStr(unsigned long inp){
  return  formatNumber(String(minute(inp)));
}

String secondStr(unsigned long inp){
  return  formatNumber(String(second(inp)));
}

byte hour(unsigned long inp){
  return  byte((inp/3600) % 24);
}

byte minute(unsigned long inp){
  return  byte((inp/60) % 60);
}

byte second(unsigned long inp){
  return  byte(inp % 60);
}


