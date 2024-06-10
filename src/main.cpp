#include <env_build.h>
#include <runtime_tools.h>

LinkedList<char> inputList;

bool timer_active = false;
bool alarm_active = false;

long epochTime;
long timer_time_set = 0;
int alarm_set_hour = -1;
int alarm_set_minute = -1;

int rotarIn;
bool clkMode = false; // timer init
bool handle = false;
bool prevHandle = false; //prev state bool for tracking and clearing changes

void reset_list(){
  inputList.clear();
  for (int i = 0; i < 5; i++) {
    inputList.add('_');
  }
}

void set_alarm_time(LinkedList<char> &inputList){ // alarm
  LinkedList<int> alarm_time_set;
  int alarm_set_hour = 0;
  int alarm_set_minute = 0;
  
  if (inputList.get(2) != '_') { // _ : HH : MM
    alarm_set_hour += (inputList.get(2) - '0');
    if (inputList.get(1) != '_') {
      alarm_set_hour += (inputList.get(1) - '0') * 10;
    }
    alarm_set_minute += (inputList.get(4) - '0');
    alarm_set_minute += (inputList.get(3) - '0') * 10;  
  }
  
  Serial.print(alarm_set_hour);
  Serial.print(" , ");
  Serial.print(alarm_set_minute);
  Serial.println(" alarm has been set");
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
  handle = digitalRead(PIN_switch);

  if (handle == false && prevHandle == true){
    // trans from display to edit mode
    reset_list();
  }

  if (!handle){  // handset up, edit mode
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
    } else { // alarm mode
      if (inputList.get(4) != '_' && alarm_set_hour == -1){
        set_alarm_time(inputList); 
        alarm_active = true;
        Serial.print("set time --> ");
        Serial.print(alarm_set_hour);
        Serial.print(" : ");
        Serial.println(alarm_set_minute);
      }
      if (alarm_active){
        DateTime now = rtc.now();
        
        Serial.print("cur time --> ");
        Serial.print(now.hour());
        Serial.print(" : ");
        Serial.println(now.minute());

        if (alarm_active && alarm_set_hour >= now.hour() && alarm_set_minute >= now.minute()){
          ringBell();
          alarm_set_hour = -1;
          alarm_set_minute = -1;
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
  prevHandle = handle;
}

