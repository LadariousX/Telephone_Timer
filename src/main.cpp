#include <env_build.h>
#include <runtime_tools.h>

LinkedList<char> inputList;
LinkedList<int> alarm_time_set;

bool timer_active = false;
bool alarm_active = false;

long epochTime;
long timer_time_set = 0;

int rotarIn;
bool clkMode = false; // timer init
bool freshSettings = false;

void reset_list(){
  inputList.clear();
  for (int i = 0; i < 5; i++) {
    inputList.add('_');
  }
}



void setup(){
  pinMode(PIN_bell, OUTPUT);
  pinMode(PIN_switch, INPUT);
  pinMode(PIN_rotary, INPUT);

  Serial.begin(115200);
  buildDisplay();
  buildWifi();
  buildClocks();

  reset_list();
  
  display.clearDisplay();
}

void loop() {
  epochTime = getEpochFromRTC();
  rotarIn = getRotar();

  if (!digitalRead(PIN_switch)){  // handset up, edit mode
    if(rotarIn != -1){ // add to list
      inputList.remove(0);
      inputList.add(4, ('0' + (rotarIn)));
    }
    if (inputList.get(4) == '0' && inputList.get(3) == '_'){  // change mode
      clkMode = !clkMode;  
      reset_list();
    }
    display_edit_mode(inputList, clkMode);
  } 



  else{  // handset down, display mode
    Serial.print(timer_time_set);
    Serial.println(" <set ");
    Serial.print(epochTime);
    Serial.println(" <epoch ");


    if(!clkMode){ // timer mode
      if (inputList.get(4) != '_' && timer_time_set == 0){
        timer_time_set = set_timer_time(inputList);
        timer_active = true;
      }
      if (timer_active && timer_time_set <= epochTime){
        ringBell();
        timer_time_set = 0;
        timer_active = false;
      }
    } else {
      if (inputList.get(4) != '_' && alarm_time_set.get(0) == -1){
        set_alarm_time(inputList);
        alarm_active = true;
      }
      if (alarm_active){
        DateTime now = rtc.now();
        if (alarm_active && alarm_time_set.get(0) >= now.hour() && alarm_time_set.get(1) >= now.minute()){
          ringBell();
          alarm_time_set.clear();
          alarm_time_set.set(0,-1);
          alarm_time_set.set(1,-1);
          alarm_active = false;
        }
      }
    }

    if (rotarIn == 0){ // allow for prievew set times 
      if (clkMode){
        Serial.print("show alarm set time real quick");
      }
      else{
        Serial.print("show clock time real quick");
      }
      delay(3);
    }
    print_Clock(clkMode);
    
  }
}

