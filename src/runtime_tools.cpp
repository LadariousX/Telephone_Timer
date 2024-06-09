#include <runtime_tools.h>

int pulses = 0;
bool reading;
bool altSt = 1;
bool prevSt = 1;
bool deliver = 0;
long prevTimestamp = 0;
int outbound;

long getEpochFromRTC() {
  DateTime now = rtc.now();
  struct tm tm;
  tm.tm_year = now.year() - 1900;
  tm.tm_mon = now.month() - 1;
  tm.tm_mday = now.day();
  tm.tm_hour = now.hour();
  tm.tm_min = now.minute();
  tm.tm_sec = now.second();
  return mktime(&tm);
}

int getRotar(){
  outbound = -1;
  reading = digitalRead(PIN_rotary);
  if ((millis() - prevTimestamp) > 100) {
    if (deliver) {
      outbound = (pulses % 10);
      deliver = 0;
      pulses = 0;
    }
  }

  if (reading != prevSt) {
    prevTimestamp = millis();
  }
  if ((millis() - prevTimestamp) > 10) {
    if (reading != altSt) {
      altSt = reading;
      if (altSt == HIGH) {
        pulses++;
        deliver = 1;
      }
    }
  }
  prevSt = reading;
  return outbound;
}

void print_Clock(bool mode){
  DateTime now = rtc.now();
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(10, 6);
  if (now.hour() == 0){
    display.print(12);
  } else {
    display.print(now.hour() % 12);
  }
  display.print(":");
  if(now.minute() < 10){
    display.print(0);
  }
  display.print(now.minute());

  display.setTextSize(2);
  //    display.setCursor(110,13);
  if (now.hour() < 12){
    display.print("AM");
  } else {
    display.print("PM");
  }
  if (mode){
    display.setCursor(2,2);
    display.print("A");
  }
  display.display();
  delay(50);
}

void display_edit_mode(LinkedList<char> &inputList, bool mode){ //print edit mode - add lables for blanks
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(40, 4);
  if (!mode) {
    display.print("Set Timer");
    display.setTextSize(2);
    display.setCursor(27, 15);

    display.print(inputList.get(0));
    display.print(":");
    display.print(inputList.get(1));
    display.print(inputList.get(2));
    display.print(":");
    display.print(inputList.get(3));
    display.print(inputList.get(4));
    display.display();

  } else {
    display.print("Set Alarm");
    display.setTextSize(2);
    display.setCursor(35, 15);

    display.print(inputList.get(1));
    display.print(inputList.get(2));
    display.print(":");
    display.print(inputList.get(3));
    display.print(inputList.get(4));
    display.display();

  }
}

LinkedList<int> set_alarm_time(LinkedList<char> &inputList){ // alarm
  LinkedList<int> alarm_set;
  int hour = 0;
  int minute = 0;
  
  if (inputList.get(2) != '_') { // _ : HH : MM
    hour += (inputList.get(2) - '0');
    if (inputList.get(1) != '_') {
      hour += (inputList.get(1) - '0') * 10;
    }
    minute += (inputList.get(4) - '0');
    minute += (inputList.get(3) - '0') * 10;  
  }

  alarm_set.add(0,hour);
  alarm_set.add(1,minute);
  Serial.println("alarm has been set");
  return alarm_set;
} 

long set_timer_time(LinkedList<char> &inputList){   // timer
  long timerTime = getEpochFromRTC(); 
  long output = 0;
  if (inputList.get(4) != '_') { 
    timerTime += (inputList.get(4) - '0');
    if (inputList.get(3) != '_') {
      timerTime += (inputList.get(3) - '0') * 10;
      if (inputList.get(2) != '_') {
        timerTime += (inputList.get(2) - '0') * 60;
        if (inputList.get(1) != '_') {
          timerTime += (inputList.get(1) - '0') * 600;
          if (inputList.get(0) != '_'){
            timerTime += (inputList.get(0) - '0') * 3600;
          }
        }
      }
    }
    output = timerTime;
    
  }
  return output;
}

void ringBell(){
  Serial.println("ringing bell!");
  bool state = true;
  int i = 0;

  while (state && i < 20) {
      digitalWrite(PIN_bell, HIGH);
      delay(2000);
      digitalWrite(PIN_bell, LOW);
      delay(3000);

      state = digitalRead(PIN_switch); // true = handset down
      i++;
    }
    digitalWrite(PIN_bell, LOW);
    Serial.println("done ringing");
}