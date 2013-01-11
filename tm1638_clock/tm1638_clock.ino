#include <TM1638.h>

TM1638 tm (8, 9, 7);


unsigned long elap;
unsigned long time;
unsigned long gap;
boolean dots;
byte buttons;

void setup(){
  gap=0;
  dots=0;
  buttons=0;
}

void loop(){
  checkButtons();
  checkTime();
  delay (100);
}


void checkTime(){
  elap=floor(millis() / 1000);
  if (elap + gap != time) {
    time=gap + elap;
    tm.setDisplayToString(formatDate(time),(dots * 80));
    dots = !dots;
  }
}


void checkButtons(){
  buttons=tm.getButtons();
  if(buttons>0){
    for (byte i=0;i<8;i++){
      if ((buttons & 1<<i) != 0) buttonEvent(i);
    }
    delay(100);
    tm.setLEDs(0);
  }
}


void buttonEvent(byte inp){
  tm.setLED(1,inp);
  switch (inp) {
  case 0:
    if (hour(gap) != 23)    {  
      gap += 3600    ; 
    }
    else      {
      gap -= 82800;
    }
    break;
  case 1:
    if (hour(gap) != 0){
      gap -= 3600;
    }
    else{
      gap += 82800;
    }
    break;
  case 2:
    if (minute(gap) != 59){
      gap += 60;
    }
    else{
      gap -= 3540;
    }
    break;
  case 3:
    if (minute(gap) != 0){
      gap -= 60;
    }
    else{
      gap += 3540;
    }
    break;
  case 4:
    if (second(gap) != 59){
      gap += 1;
    }
    else{
      gap -= 59;
    }
    break; 

  case 5:
    if (second(gap) != 0){
      gap -= 1;
    }
    else{
      gap += 59;
    }
    break;
  case 6:
    delay(400);
    break;
  case 7:
    gap=0;
    break;
  }
}


String formatDate(unsigned long inp){
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
